#include "__Net_Wireless_CC3000_PIC32.h"

void CC3000_Init();



// _events_code file
extern const uint16_t COLOR_BACKGROUND;
extern const uint16_t COLOR_FONT;
extern const uint8_t  SWITCH_STATE_OFF;
extern const uint8_t  SWITCH_STATE_ON;
extern const uint8_t  SWITCH_NUM_1;
extern const uint8_t  SWITCH_NUM_2;

extern void    GUI_UpdateScanLabel(uint8_t *text);
extern void    GUI_UpdateUdpData(uint8_t *text);
extern void    GUI_ScanNetworks();
extern void    GUI_ConnectToAp();
extern void    GUI_CheckConnStatus();
extern void    GUI_SwitchToggle(uint8_t switchNum);
extern void    GUI_PollingWiFi();
extern uint8_t GUI_SwitchGetState(uint8_t switchNum);

// _server_code file
extern const code uint8_t   ssid[];
extern const code uint8_t   key[];
extern uint8_t   dataBuff[];
extern int32_t   udpHandler, tcpHandler, sockTimeout, response, length;
extern uint16_t  udpLocalPort, tcpLocalPort;
extern tSockAddr localSockAddr, remoteSockAddr;

extern void Wifi_Config();
extern void Wifi_ConnectToAP();
extern void Wifi_WaitForConnectToAp();
extern void Wifi_InitUdpServer();
extern void Wifi_InitTcpServer();
extern void Wifi_ErrorHandle();
extern void Wifi_UdpServer();
extern void Wifi_TcpServer();
extern void Wifi_EmptyDataBuff();
extern void Wifi_AddToDataBuff(uint8_t *str);
extern void Wifi_CreateHttpResponse();