/*
* Author: George Sapp
* Created: 2014-05-05
* Last Modified: 2014-05-05
* Reason: Creating initial version of driver
*/

#include "LEDLight.h"

//Length of the WS2812b LED strip
#define LED_STRIP_LEN 36

//Define value of true and false... Why MikroC... Why!?!?!??!
#define TRUE 1
#define FALSE 0

//Must always be positive and between 0-255
#define RAMP_AMOUNT 1

#define ws2811_zero 0b11100000
#define ws2811_one 0b111111110

unsigned short aRed2[LED_STRIP_LEN];
unsigned short aGreen2[LED_STRIP_LEN];
unsigned short aBlue2[LED_STRIP_LEN];

void LEDLight_sendColor(unsigned short this_color);
void LEDLight_sendFrame();
void LEDLight_setColor(unsigned short index, unsigned short sR, unsigned short sG, unsigned short sB);


void LEDLight_init(){

     unsigned short i;
     
     //set pin to output
     TRISG.RG8 = 0;
     SPI2_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 10, _SPI_SS_DISABLE,  _SPI_DATA_SAMPLE_MIDDLE ,_SPI_CLK_IDLE_HIGH , _SPI_IDLE_2_ACTIVE);
     //SPI2_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 20, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_ACTIVE_2_IDLE);


     for (i=0; i<LED_STRIP_LEN; i++){

         aRed2[i] = 0;
         aGreen2[i] = 0;
         aBlue2[i] = 0;
     }
     
     for (i=0; i<17; i++){
         LEDLight_setColor(i, 255,255,200);
     }


     LEDLight_update();
}

void LEDLight_update(){
  //send the frame to update the LEDs
  LEDLight_sendFrame();
  
}

void LEDLight_On(){
     unsigned short i;
     
     SPI_Set_Active(SPI2_Read, SPI2_Write);
     for (i=0; i<17; i++){
         LEDLight_setColor(i, 255,255,200);
     }
     LEDLight_update();
}

void LEDLight_Off(){
     unsigned short i;

     SPI_Set_Active(SPI2_Read, SPI2_Write);
     for (i=0; i<17; i++){
         LEDLight_setColor(i, 0,0,0);
     }
     LEDLight_update();
}

void LEDLight_setColor(unsigned short index, unsigned short sR, unsigned short sG, unsigned short sB){

     aRed2[index] = sR;
     aGreen2[index] = sG;
     aBlue2[index] = sB;
     
}

void LEDLight_sendFrame() {
  unsigned short i;

  for(i=0;i<LED_STRIP_LEN;i++) {
    LEDLight_sendColor(aGreen2[i]);
    LEDLight_sendColor(aRed2[i]);
    LEDLight_sendColor(aBlue2[i]);

  }
  delay_us(50);                             // wait 50us to latch the string
}

void LEDLight_sendColor(unsigned short this_color){

  unsigned short bit_num;
  
  for(bit_num=0; bit_num < 8; bit_num++) {
    if(this_color.B7 == 1) {
      SPI2_Write(ws2811_one);
    }
    else {
      SPI2_Write(ws2811_zero);
    }
    //shift left 1
    this_color*=2;
  }

}