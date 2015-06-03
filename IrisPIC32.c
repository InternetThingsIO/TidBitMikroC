//#include "IrisHub.h"
#include "TScriptEngine.h"
#include "stdint.h"
#include "SDCard.h"
#include "TScriptCommon.h"

#define FILE_BODY_LENGTH 5000

static uint8_t fileBody[FILE_BODY_LENGTH];

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

//do the following every 30 seconds or so
  while(1){

      //clear out file body
      memset(fileBody, '\0', FILE_BODY_LENGTH);

      SDCard_getTestScript(&fileBody);

      A2_ProcessScript(&fileBody);

      delayS(1);
      
      //if we haven't cleared yet, we should do so now
      TAction_ClearDisplay();
      
  }
}