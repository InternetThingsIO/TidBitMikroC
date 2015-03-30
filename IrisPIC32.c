//#include "IrisHub.h"
#include "TScriptEngine.h"
#include "stdint.h"
#include "SDCard.h"
#include "WS2812b.h"

#define FILE_BODY_LENGTH 5000

static uint8_t fileBody[FILE_BODY_LENGTH];

//#define Yellow_LED             LATD.RD6


sbit DATA at LATD6_bit;
//sbit STAT at LATG6_bit;

//creates a delay of the specified time
void delayS(unsigned int S){
    unsigned int i;
    
    for (i = 0; i < S; i++){
        delay_ms(1000);
    }
}



void main() {

  //init any devices
  TEngine_InitDevices();

/*while(1){
       SDCard_test();
  }*/
//do the following every 30 seconds or so
  while(1){
      DATA = 0;

      //clear out file body
      memset(fileBody, '\0', FILE_BODY_LENGTH);

      SDCard_getTestScript(&fileBody);

      A2_ProcessScript(&fileBody);


      DATA = 1;
      delayS(5);
  }
}