
#include "CC3000_Driver.h"
#include "UART_Ext.h"
#include "HTTP_Creator.h"
#include "HTTP_Interpreter.h"
#include "__Net_Wireless_CC3000_PIC32.h"
#include "WS2812b.h"

#define SOCKET_INACTIVE  -1
#define BUFFER_SIZE 2000

sbit Net_Wireless_CC3000_CS  at LATD.B9;
sbit Net_Wireless_CC3000_Int at LATD.B8;
sbit Net_Wireless_CC3000_En  at LATE.B7;

sbit Net_Wireless_CC3000_CS_Direction  at TRISD.B9;
sbit Net_Wireless_CC3000_Int_Direction at TRISD.B8;
sbit Net_Wireless_CC3000_En_Direction  at TRISE.B7;

sbit Net_Wireless_CC3000_Int_En at IEC0.B7;
sbit Net_Wireless_CC3000_Int_Flag at IFS0.B7;
sbit Net_Wireless_CC3000_Int_Status at PORTD.B8;

const uint8_t ssid[11]             = "forerunner";
const uint8_t key[9]              = "ms293262";

uint32_t  connectedFlag, dhcpCompleteFlag, closeDataSockFlag;

int32_t Net_Wireless_CC3000_WaitHandler()
{
  if(Net_Wireless_CC3000_Time > 20000)
  {
          // No response from wifi module.
    return -1;
  }
  return 0;
}

// The function handles asynchronous events that come from CC3000 device.
void Net_Wireless_CC3000_UsynchCallback(int32_t eventType, uint8_t *dat, uint8_t length)
{
  SPI_Set_Active(SPI3_Read, SPI3_Write);
  
  if (eventType == _NET_WIRELESS_CC3000_EVNT_UNSOL_CONNECT)
  {
    // Device connected to AP.
    connectedFlag = 1;
  }

  if (eventType == _NET_WIRELESS_CC3000_EVNT_UNSOL_DISCONNECT)
  {
    // Device disconnected from AP.
    connectedFlag    = 0;
    dhcpCompleteFlag = 0;
  }

  if (eventType == _NET_WIRELESS_CC3000_EVNT_UNSOL_DHCP)
  {
    // IP config parameters are received swapped.
    // dat point to buffer which contain:
        // 4 bytes of IP address
        // 4 bytes of subnet
        // 4 bytes of default GW
        // 4 bytes of DHCP server
        // 4 bytes of DNS server
        // 1 bytes of status
    // only if status is OK, addresses are valid
    if ( *(dat + 20) == 0)
    {
      dhcpCompleteFlag = 1;
    }
    else
    {
      dhcpCompleteFlag = 0;
    }
  }
  // Check if FIN was received by remote.
  /*if (eventType == _NET_WIRELESS_CC3000_EVNT_BSD_TCP_CLOSE_WAIT)
  {
    // data[0] represents the socket id, for which FIN was received.
    if(dat[0] == tcpDataSocket)
    {
      closeDataSockFlag = 1;
    }
  }*/
}

// GPIO interrupt handler.
void INT1_Interrupt() iv IVT_EXTERNAL_1 ilevel 7 ics ICS_SRS {
  Net_Wireless_CC3000_IntHandler();
}

// Timer interrupt handler.
void Timer1Interrupt() iv IVT_TIMER_1 ilevel 7 ics ICS_SRS {
  Net_Wireless_CC3000_Time++;  
  T1IF_bit         = 0;
}

// Init 1ms timer interrupt.
void InitTimer1(){
  T1CON         = 0x8010;
  T1IE_bit         = 1;
  T1IF_bit         = 0;
  T1IP0_bit         = 1;
  T1IP1_bit         = 1;
  T1IP2_bit         = 1;
  PR1                 = 10000;
  TMR1                 = 0;
}

//Initialization of External Interrupt
void InitExt(){
  INT1IP0_bit = 1;
  INT1IP1_bit = 1;
  INT1IP2_bit = 1;

  INTCONbits.INT1EP = 0; //falling egde

  Net_Wireless_CC3000_Int_Flag = 0; // clear external interrupt 1 flag
  Net_Wireless_CC3000_Int_En = 0; // enable external interrupt 1
}

// This function configure HW
void Wifi_Config()
{
  Delay_ms(1000);
  SPI3_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 8, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_ACTIVE_2_IDLE);
  
  UART2_Write_Text("Started SPI CC3000");
  UART2_Write(0xD);
  
  Net_Wireless_CC3000_Init();
  Net_Wireless_CC3000_regAsynHandler(Net_Wireless_CC3000_UsynchCallback);

  UART2_Write_Text("Init and regAsynHandler CC3000");
  UART2_Write(0xD);

  //initialize external interrupt
  InitExt();
  //enable all interrupts
  EnableInterrupts();
  
  UART2_Write_Text("Enable Interrupts CC3000");
  UART2_Write(0xD);
  
  Net_Wireless_CC3000_Start(0);
  
  UART2_Write_Text("Start CC3000");
  UART2_Write(0xD);

  InitTimer1();
  
}


void Wifi_ConnectToAP()
{
  Net_Wireless_CC3000_ConnectToAP(_NET_WIRELESS_CC3000_SEC_WPA2, ssid, strlen(ssid), 0, key, strlen(key));
  Wifi_WaitForConnectToAp();
}

void Wifi_WaitForConnectToAp()
{
  while((connectedFlag != 1)||(dhcpCompleteFlag != 1))
  {
    Net_Wireless_CC3000_unsolEventHandler();
    Delay_ms(100);
  }
}

uint8_t* Wifi_Get(char host[], char path[], uint16_t port){
  static uint8_t rBuffer[BUFFER_SIZE];
  uint8_t *HTTPBody;
  //uint32_t hostIPAddress;
  //int32_t dataSocket;
  //uint8_t  cloudIpAddr[4] = {191, 236, 40, 251};
  uint8_t  cloudIpAddr[4] = {192, 168, 0, 110};
  tSockAddr socketAddress;
  int32_t tcpDataSocket = SOCKET_INACTIVE;

  SPI_Set_Active(SPI3_Read, SPI3_Write);

  memset(rBuffer, '\0', BUFFER_SIZE);
  HTTP_Generate_Get(host, path, rBuffer);

  writeStr("Get request");
  writeStr(rBuffer);
  //writeInt(strlen(rBuffer));

  //make an HTTP request then print results
  tcpDataSocket = Net_Wireless_CC3000_SocketCreate(_NET_WIRELESS_CC3000_SOCK_DOMAIN_AF_INET, _NET_WIRELESS_CC3000_SOCK_TYPE_STREAM, _NET_WIRELESS_CC3000_SOCK_PROTO_TCP);


  //get the IP address from the hostname
  //Net_Wireless_CC3000_SocketGetHostByName(host, strlen(host), cloudIpAddr);
  //writeStr("Get IP Address from DNS");
  //set the IP address to be used
  Net_Wireless_CC3000_SocketSetInetAddr(cloudIpAddr, port, &socketAddress);                // Set Socket Address fields.
  writeStr("Set IP Address");
  Net_Wireless_CC3000_SocketConnect(tcpDataSocket, &socketAddress, sizeof(socketAddress));
  writeStr("Connected to socket");
  Net_Wireless_CC3000_SocketSend(tcpDataSocket, rBuffer, strlen(rBuffer), 0);
  writeStr("Send data on socket");

  //empty rBuffer with null
  memset(rBuffer, '\0', BUFFER_SIZE);
  Net_Wireless_CC3000_SocketReceive(tcpDataSocket, rBuffer, BUFFER_SIZE, 0);

  writeStr(" receive result");

  //writeInt(strlen(rBuffer));
  //writeStr(" strlen");

  writeBytes(rBuffer, strlen(rBuffer));

  UART2_Write(0xD);
  UART2_Write(0xD);
  
  Net_Wireless_CC3000_SocketClose(tcpDataSocket);
  
  //if this is a valid HTTP response
  if(HTTP_CheckResult(rBuffer) == 1){
    //get the result body
    HTTPBody = strstr(rBuffer, "\r\n\r\n");
    HTTPBody = &HTTPBody[4];
    writeStr("HTTP Body: ");
    writeInt(strlen(HTTPBody));
    //writeStr(HTTPBody);
    writeBytes(HTTPBody, strlen(HTTPBody));
    //while(1){}
    return HTTPBody;
  }else
    return 0;



}