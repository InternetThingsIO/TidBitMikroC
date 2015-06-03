#include "SDCard.h"
#include "stdint.h"

#define MAX_FILE_NAME_LENGTH 3

sbit Mmc_Chip_Select at LATE0_bit;
sbit Mmc_Chip_Select_Direction at TRISE0_bit;

uint8_t *SDCard_getIndexFileName(long index);

long fileNameIndex = 1;

uint8_t SDCard_Init(){
  unsigned char initResult;
  SPI2_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 64, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH, _SPI_ACTIVE_2_IDLE);
  
  return Mmc_Fat_Init();
 
}

void SDCard_getTestScript(uint8_t *ptrFileBuffer){

  unsigned long fileSize;
  unsigned long i;
  unsigned char result = 0;
  uint8_t *ptrFileName = "testnothing";
  
  while (Mmc_Fat_Exists(ptrFileName) == 0){

    ptrFileName = SDCard_getIndexFileName(fileNameIndex);
    
    fileNameIndex++;

    if (fileNameIndex > 50)
       fileNameIndex = 1;

  }
  
  result = Mmc_Fat_Assign(ptrFileName, 0);

  Mmc_Fat_Reset(&fileSize);

  Mmc_Fat_ReadN(ptrFileBuffer, fileSize);
  
  Mmc_Fat_Close();

}

uint8_t SDCard_fileTest(){

     return Mmc_Fat_Assign("1.txt", 0);

}

uint8_t *SDCard_getIndexFileName(long index){
  static uint8_t fileName[MAX_FILE_NAME_LENGTH];
  uint8_t i;
  uint8_t *ptrFileName;
    //clear out filename
  memset(fileName, '\0', MAX_FILE_NAME_LENGTH);

  LongToStr(index, fileName);
  strcat(fileName, ".txt");
  ptrFileName = &fileName;

  while (ptrFileName[0] == 32){
    ptrFileName += 1;
  }

  return ptrFilename;

}