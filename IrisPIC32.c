#include "IrisHub.h"
#include "TScriptEngine.h"
#include "UART_Ext.h"
#include "stdint.h"

const uint8_t device_id[] = "A38A66D2-62C2-456F-9866-2070CBA77F41";

//#define Yellow_LED             LATD.RD6


sbit DATA at LATD6_bit;
//sbit STAT at LATG6_bit;

tNetappIpconfigRetArgs ipconfig;

unsigned char* MACtoStr(uint8_t uaMacAddr[6]);
unsigned char* IPtoStr(uint8_t aucIP[4]);

void printConnectionInfo(){

     //get IP Address
     Net_Wireless_CC3000_GetIpConfig(&ipconfig);
     
     UART2_Write(0xD);
     UART2_Write_Text("Get IP Config CC3000");
     UART2_Write(0xD);
     
     UART2_Write_text("SSID: ");
     UART2_Write_text(ipconfig.uaSSID);
     UART2_Write(0xD);
     
     UART2_Write_text("MAC: ");
     UART2_Write_text(MACtoStr(ipconfig.uaMacAddr));
     UART2_Write(0xD);
     
     UART2_Write_text("IP: ");
     UART2_Write_text(IPtoStr(ipconfig.aucIP));
     UART2_Write(0xD);
     
     UART2_Write_text("Subnet Mask: ");
     UART2_Write_text(IPtoStr(ipconfig.aucSubnetMask));
     UART2_Write(0xD);
     
     UART2_Write_text("DHCP Server: ");
     UART2_Write_text(IPtoStr(ipconfig.aucDHCPServer));
     UART2_Write(0xD);
     
     UART2_Write_text("DNS: ");
     UART2_Write_text(IPtoStr(ipconfig.aucDNSServer));
     UART2_Write(0xD);

}

//creates a delay of the specified time
void delayS(unsigned int S){
    unsigned int i;
    
    for (i = 0; i < S; i++){
        delay_ms(1000);
        writeInt(i);
    }
}



void main() {
  uint8_t *ptrAction;

  startUART();

  //init any devices
  TEngine_InitDevices();

  TRISD.B6 = 0;                                 //set direction to be output
  DATA = 0;

  UART2_Write_Text("Init CC3000");
  UART2_Write(0xD);

  Wifi_Config();

  UART2_Write_Text("Connect to AP CC3000");
  UART2_Write(0xD);
  WiFi_ConnectToAp();

  printConnectionInfo();

  UART2_Write_Text("Finish Init CC3000");
  UART2_Write(0xD);

  //do the following every 30 seconds or so
  while(1){
    DATA = 0;
    
    ptrAction = checkConnectors(device_id);

    //if we have some action script, we should execute it
    if (ptrAction != 0){
      //writeStr(ptrAction);
      A2_ProcessScript(ptrAction);
    }
    
    DATA = 1;
    writeStr("waiting to check again");
    delayS(5);
  }
}