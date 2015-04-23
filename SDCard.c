#include "SDCard.h"
#include "stdint.h"
#include "WS2812B.h"

sbit Mmc_Chip_Select at LATE0_bit;
sbit Mmc_Chip_Select_Direction at TRISE0_bit;

void SDCard_Init(){
  unsigned char initResult;
  SPI2_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 64, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH, _SPI_ACTIVE_2_IDLE);
  initResult = Mmc_Fat_Init();

  if (initResult == 255)
    WS2812B_setColor(1, 32,0,0);
  else if(initResult == 1)
    WS2812B_setColor(1, 32,32,0);
  else if (initResult == 0)
    WS2812B_setColor(1, 0,32,0);
 
}

void SDCard_getTestScript(uint8_t *ptrFileBuffer){

  unsigned long fileSize;
  unsigned long i;
  unsigned char result;

  result = Mmc_Fat_Assign("test.txt", 0x01);
     
  if (result == 2)
    WS2812B_setColor(2, 32,32,0);
  else if(result == 1)
    WS2812B_setColor(2, 0,32,0);
  else if (result == 0)
    WS2812B_setColor(2, 32,0,0);

  Mmc_Fat_Reset(&fileSize);

  Mmc_Fat_ReadN(ptrFileBuffer, fileSize);

}
 
void SDCard_test(){

     unsigned long fileSize;
     unsigned long i;
     uint8_t currData;

     Mmc_Fat_Assign("phone.txt", 0x01);
     Mmc_Fat_Reset(&fileSize);

     for (i=0; i <fileSize; i++){
         Mmc_Fat_Read(&currData);
     }
     
}