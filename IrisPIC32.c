#include "IrisHub.h"
#include "WS2812B.h"
#include "stdint.h"

#define LED_STRIP_LEN 36

unsigned long timerIterations2;

void main() {
     int i;
     //Delay_ms(1000);
     //set pin to output
     TRISB.B6 = 0;

     //set pin to 0
     RB6_bit = 0;

     WS2812b_Init();
     
     for (i=0; i<LED_STRIP_LEN; i++){
         //WS2812b_setColor(i,i/3,0,(LED_STRIP_LEN - i)/3);
     }
     
     WS2812b_setColorRange(0,35, 32,32,32);
     
     while(1){
         WS2812b_Update();
     }
}