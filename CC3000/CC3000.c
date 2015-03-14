#define  _NET_WIRELESS_CC3000_SEC_UNSEC                      0
#define  _NET_WIRELESS_CC3000_SEC_WEP                        1
#define  _NET_WIRELESS_CC3000_SEC_WPA                        2
#define  _NET_WIRELESS_CC3000_SEC_WPA2                       3

#define  _NET_WIRELESS_CC3000_DISABLE                        0
#define  _NET_WIRELESS_CC3000_ENABLE                         1


#define  _NET_WIRELESS_CC3000_SCAN_STATUS_VALID              1
#define  _NET_WIRELESS_CC3000_SCAN_STATUS_INVALID            0

#define  _NET_WIRELESS_CC3000_SCAN_SECMODE_OPEN              2
#define  _NET_WIRELESS_CC3000_SCAN_SECMODE_WEP               3
#define  _NET_WIRELESS_CC3000_SCAN_SECMODE_WPA               4
#define  _NET_WIRELESS_CC3000_SCAN_SECMODE_WPA2              5

#define _NET_WIRELESS_CC3000_IPCONFIG_IP_OFFSET              0
#define _NET_WIRELESS_CC3000_IPCONFIG_SUBNET_OFFSET          4
#define _NET_WIRELESS_CC3000_IPCONFIG_GW_OFFSET              8
#define _NET_WIRELESS_CC3000_IPCONFIG_DHCP_OFFSET            12
#define _NET_WIRELESS_CC3000_IPCONFIG_DNS_OFFSET             16
#define _NET_WIRELESS_CC3000_IPCONFIG_MAC_OFFSET             20
#define _NET_WIRELESS_CC3000_IPCONFIG_SSID_OFFSET            26

#define _NET_WIRELESS_CC3000_EVNT_UNSOL_CONNECT              0x8001
#define _NET_WIRELESS_CC3000_EVNT_UNSOL_DISCONNECT           0x8002
#define _NET_WIRELESS_CC3000_EVNT_UNSOL_INIT                 0x8004
#define _NET_WIRELESS_CC3000_EVNT_TX_COMPLETE                0x8008
#define _NET_WIRELESS_CC3000_EVNT_UNSOL_DHCP                 0x8010
#define _NET_WIRELESS_CC3000_EVNT_ASYNC_PING                 0x8040
#define _NET_WIRELESS_CC3000_EVNT_ASYNC_SIMPLE_CFG_DONE      0x8080
#define _NET_WIRELESS_CC3000_EVNT_KEEPALIVE                  0x8200
#define _NET_WIRELESS_CC3000_EVNT_BSD_TCP_CLOSE_WAIT         0x8800
#define _NET_WIRELESS_CC3000_EVENT_CAN_SHUT_DOWN             0x0099

#define _NET_WIRELESS_CC3000_STATUS_DISCONNECTED             0
#define _NET_WIRELESS_CC3000_STATUS_CONNECTED                3

#define _NET_WIRELESS_PATCHES_REQUEST_DEFAULT                0
#define _NET_WIRELESS_PATCHES_REQUEST_FORCE_HOST             1
#define _NET_WIRELESS_PATCHES_REQUEST_FORCE_NONE             2

sbit Net_Wireless_CC3000_CS  at LATD.B9;
sbit Net_Wireless_CC3000_Int at LATD.B8;
sbit Net_Wireless_CC3000_En  at LATE.B7;

sbit Net_Wireless_CC3000_CS_Direction  at TRISD.B9;
sbit Net_Wireless_CC3000_Int_Direction at TRISD.B8;
sbit Net_Wireless_CC3000_En_Direction  at TRISE.B7;

sbit Net_Wireless_CC3000_Int_En at IEC0.B7;
sbit Net_Wireless_CC3000_Int_Flag at IFS0.B7;
sbit Net_Wireless_CC3000_Int_Status at PORTD.B8;

typedef signed long int int32_t; 
typedef unsigned int uint16_t;

extern int32_t Net_Wireless_CC3000_WaitHandler();

void CC3000_Init(){
  uint16_t patches = 0;
  
  Delay_ms(1000);
  SPI3_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 8, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_ACTIVE_2_IDLE);

  Net_Wireless_CC3000_Init();
  Delay_ms(1000);
  Net_Wireless_CC3000_ExtIntDisable();
  Net_Wireless_CC3000_Start(patches);


}

int32_t Net_Wireless_CC3000_WaitHandler()
{
  //Net_Wireless_CC3000_Time += 1;
  //if(Net_Wireless_CC3000_Time > 2000)
  //{
    // No response from wifi module.
    //return -1;
 // }
  return 0;
}