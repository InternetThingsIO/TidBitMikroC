#include "SDCard.h"
#include "stdint.h"

sbit Mmc_Chip_Select at LATE0_bit;
sbit Mmc_Chip_Select_Direction at TRISE0_bit;

void SDCard_Init(){
 
 SPI2_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 64, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH, _SPI_ACTIVE_2_IDLE);
 Mmc_Fat_Init();
 
}

void SDCard_getTestScript(uint8_t *ptrFileBuffer){

     unsigned long fileSize;
     unsigned long i;

     Mmc_Fat_Assign("test.txt", 0x01);
     Mmc_Fat_Reset(&fileSize);
     
     Mmc_Fat_ReadN(ptrFileBuffer, fileSize);

}
 
void SDCard_test(){

     unsigned long fileSize;
     unsigned long i;
     uint8_t currData;

     Mmc_Fat_Assign("test.txt", 0x01);
     Mmc_Fat_Reset(&fileSize);

     for (i=0; i <fileSize; i++){
         Mmc_Fat_Read(&currData);
     }
     
}