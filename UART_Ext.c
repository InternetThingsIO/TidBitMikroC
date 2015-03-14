#include "__Net_Wireless_CC3000_PIC32.h"

void startUART(){
     UART2_Init(115200);
     UART2_Write(0x0);
     UART2_Write(0x1);
}

void writeStr(unsigned char string[]){
  if (string != 0){
    UART2_Write_Text(string);
    UART2_Write(0xD);
  }
}

void writeInt(int var){
     char str[20];
     memset(str, '\0', sizeof(str));

     inttostr(var, str);

     UART2_Write_Text(str);
     UART2_Write(0xD);
}

void writeShort(unsigned short var){
     char str[20];
     memset(str, '\0', sizeof(str));

     shorttostr(var, str);

     UART2_Write_Text(str);
     UART2_Write(0xD);
}

void writeLong(long int var){
     char str[20];
     memset(str, '\0', sizeof(str));
     
     LongtoStr(var, str);
     
     UART2_Write_Text(str);
     UART2_Write(0xD);
}

void writeBytes(uint8_t bytes[], unsigned int numBytes){
     unsigned int i;
     
     for (i = 0; i < numBytes; i++){
         UART2_Write(bytes[i]);
     }

}

unsigned char* IPtoStr(uint8_t aucIP[4]){

     unsigned char IPStr[50];
     unsigned char IPChar[5];
     unsigned short i;

     memset(IPStr, 0, 50);
     memset(IPChar, 0, 5);

     for (i = 0; i < 4; i++){
         ByteToStr(aucIP[i], IPChar);
         strcat(IPStr, LTrim(RTrim(IPChar)));
         if (i < 3)
            strcat(IPStr, ".");
     }

     return IPStr;

}

unsigned char* MACtoStr(uint8_t uaMacAddr[6]){

     unsigned char MACStr[50];
     unsigned char MACChar[3];
     unsigned short i;

     memset(MACStr, 0, 50);
     memset(MACChar, 0, 3);



     for (i = 0; i < 6; i++){
         ByteToHex(uaMacAddr[i], MACChar);
         strcat(MacStr, LTrim(RTrim(MACChar)));
         if (i < 5)
            strcat(MacStr, ":");
     }

     return MACStr;

}