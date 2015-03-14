#include "CC3000_Driver.h"
#include "__Net_Wireless_CC3000_PIC32.h"

void startUART();
void writeStr(char string[]);
void writeInt(int var);
void writeLong(long var);
void writeBytes(unsigned char bytes[], unsigned int numBytes);
unsigned char* IPtoStr(uint8_t aucIP[4]);
unsigned char* MACtoStr(uint8_t uaMacAddr[6]);
void writeShort(unsigned short var);