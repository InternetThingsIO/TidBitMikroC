#include "__Net_Wireless_CC3000_PIC32.h"

extern void Wifi_Config();
extern void Wifi_ConnectToAP();
extern void Wifi_WaitForConnectToAp();


extern uint8_t *Wifi_Get(char host[], char path[], uint16_t port);