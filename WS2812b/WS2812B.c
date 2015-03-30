/*
* Author: George Sapp
* Created: 2014-05-05
* Last Modified: 2014-05-05
* Reason: Creating initial version of driver
*/

#include "UART_Ext.h"
#include "WS2812b.h"

//Length of the WS2812b LED strip
#define LED_STRIP_LEN 36

//Define value of true and false... Why MikroC... Why!?!?!??!
#define TRUE 1
#define FALSE 0

#define RED 0
#define GREEN 0
#define BLUE 0

//Must always be positive and between 0-255
//#define RAMP_AMOUNT 1

#define ws2811_zero 0b11100000
#define ws2811_one 0b111111110

unsigned short RAMP_AMOUNT;

unsigned short aRed[LED_STRIP_LEN];
unsigned short aGreen[LED_STRIP_LEN];
unsigned short aBlue[LED_STRIP_LEN];

unsigned short dRed[LED_STRIP_LEN];
unsigned short dGreen[LED_STRIP_LEN];
unsigned short dBlue[LED_STRIP_LEN];

unsigned short updateEnabled;

void sendColor(unsigned short this_color);
void sendFrame();
void rampColor(unsigned short saColor[], unsigned short sdColor[], unsigned short sRampAmount);
unsigned short rampOneColor(unsigned short saColor, unsigned short sdColor, unsigned short sRampAmount);
void WS2812b_setColor(unsigned short index, unsigned short sR, unsigned short sG, unsigned short sB);
void checkRampComplete(unsigned short sColor, int sDifference);
void RotateArrayCW(uint8_t array[], unsigned short length);
void reverse(uint8_t a[], uint8_t sz);
void rotate(uint8_t array[], uint8_t size, uint8_t amt);


void write1();
void write0();
void InitTimer2();
void Timer2Interrupt();

void WS2812b_init(){

     unsigned short i;
     
     //set pin to output
     TRISD.B4 = 0;
     
     //set pin to 0
     RD4_bit = 0;
     //SPI2_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 10, _SPI_SS_DISABLE,  _SPI_DATA_SAMPLE_MIDDLE ,_SPI_CLK_IDLE_HIGH , _SPI_IDLE_2_ACTIVE);
     //SPI2_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 10, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_ACTIVE_2_IDLE);
     //SPI_Set_Active(SPI2_Read, SPI2_Write);

     for (i=0; i<LED_STRIP_LEN; i++){
         dRed[i] = 0;
         dGreen[i] = 0;
         dBlue[i] = 0;
         
         aRed[i] = 0;
         aGreen[i] = 0;
         aBlue[i] = 0;
     }


/*for (i=0; i<LED_STRIP_LEN; i++){
         if (i%3 == 0)
            dRed[i] = 64;
         else
             dRed[i] = 0;

         if ((i+1)%3 == 0)
            dGreen[i] = 64;
         else
             dGreen[i] = 0;

         if ((i+2)%3 == 0)
           dBlue[i] = 64;
         else
           dBlue[i] = 0;

         aRed[i] = 0;
         aGreen[i] = 0;
         aBlue[i] = 0;
     }*/
     
     RAMP_AMOUNT = 1;
     
     //init timer
     InitTimer2();
     
     updateEnabled = TRUE;
}

void Timer2Interrupt() iv IVT_TIMER_1 ilevel 7 ics ICS_SRS {
    T1IF_bit = 0;

    WS2812b_Update();

}

// Init 1ms timer interrupt.
void InitTimer2(){
  T1CON         = 0x8030;
  T1IE_bit         = 1;
  T1IF_bit         = 0;
  T1IP0_bit         = 1;
  T1IP1_bit         = 1;
  T1IP2_bit         = 1;
  PR1                 = 13000;
  TMR1                 = 0;
}

void WS2812b_setRampAmount(unsigned short time){
    RAMP_AMOUNT = time;
}

void WS2812b_WaitRampComplete(){
    while (WS2812b_RampComplete() == 0){}

}

void WS2812b_update(){

  if (updateEnabled == TRUE){

    //ramp RGB
    rampColor(aRed, dRed, RAMP_AMOUNT);
    rampColor(aGreen, dGreen, RAMP_AMOUNT);
    rampColor(aBlue, dBlue, RAMP_AMOUNT);


    //send the frame to update the LEDs
    sendFrame();

  }

}

unsigned short WS2812b_RampComplete(){
  unsigned short i;
  
  for (i = 0; i < LED_STRIP_LEN; i++){
      //return 0 if any one color is not complete
      if ((aRed[i] - dRed[i]) != 0)
         return 0;
  
      if ((aGreen[i] - dGreen[i]) != 0)
         return 0;
         
      if ((aBlue[i] - dBlue[i]) != 0)
         return 0;
  }
  
  return 1;
}

void WS2812b_setColorRange(unsigned short low, unsigned short high, unsigned short sR, unsigned short sG, unsigned short sB){
     unsigned short i;
     
     if ((low >= 0) && (high < LED_STRIP_LEN) && (low < high)){
     
       for (i = low; i <= high; i++){
          WS2812b_setColor(i, sR, sG, sB);
       }
       
     }
}

void WS2812b_setColor(unsigned short index, unsigned short sR, unsigned short sG, unsigned short sB){

     dRed[index] = sR;
     dGreen[index] = sG;
     dBlue[index] = sB;
     
}

void reverse(uint8_t *a, int sz) {
  int i, j;
  uint8_t tmp;
  
  for (i = 0, j = sz; i < j; i++, j--) {
    tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
  }
}

void rotate(uint8_t *array, int size, int amt) {
  if (amt < 0)
    amt = size + amt;
    
  reverse(array, size-amt-1);
  reverse(array+size-amt, amt-1);
  reverse(array, size-1);
}


void WS2812b_rotate(int positions){

    uint8_t red[LED_STRIP_LEN];
    uint8_t green[LED_STRIP_LEN];
    uint8_t blue[LED_STRIP_LEN];
    
    memcpy(red, dRed, LED_STRIP_LEN);
    memcpy(green, dGreen, LED_STRIP_LEN);
    memcpy(blue, dBlue, LED_STRIP_LEN);

    rotate(red, LED_STRIP_LEN, positions);
    rotate(green, LED_STRIP_LEN, positions);
    rotate(blue, LED_STRIP_LEN, positions);
    
    memcpy(dRed, red, LED_STRIP_LEN);
    memcpy(dGreen, green, LED_STRIP_LEN);
    memcpy(dBlue, blue, LED_STRIP_LEN);

}

void WS2812b_rotate_range(uint8_t start, uint8_t end, int positions){

    uint8_t red[LED_STRIP_LEN];
    uint8_t green[LED_STRIP_LEN];
    uint8_t blue[LED_STRIP_LEN];

    memcpy(red, dRed, LED_STRIP_LEN);
    memcpy(green, dGreen, LED_STRIP_LEN);
    memcpy(blue, dBlue, LED_STRIP_LEN);

    rotate(&red[start], abs(start-end), positions);
    rotate(&green[start], abs(start-end), positions);
    rotate(&blue[start], abs(start-end), positions);

    memcpy(dRed, red, LED_STRIP_LEN);
    memcpy(dGreen, green, LED_STRIP_LEN);
    memcpy(dBlue, blue, LED_STRIP_LEN);

}

void sendFrame() {
  unsigned short i;

  for(i=0;i<LED_STRIP_LEN;i++) {
    sendColor(aGreen[i]);
    sendColor(aRed[i]);
    sendColor(aBlue[i]);

  }
  delay_us(50);                             // wait 50us to latch the string
}

void sendColor(unsigned short this_color){

  unsigned short bit_num;
  
  for(bit_num=0; bit_num < 8; bit_num++) {
    if(this_color.B7 == 1) {
      //SPI2_Write(ws2811_one);
      write1();
    }
    else {
      //SPI2_Write(ws2811_zero);
      write0();
    }
    //shift left 1
    this_color*=2;
  }

}

void write1(){
    RD4_bit= 1;
    Delay_Cyc(6);
    RD4_bit = 0;
    Delay_Cyc(2);
}

void write0(){
    RD4_bit = 1;
    Delay_Cyc(2);
    RD4_bit = 0;
    Delay_Cyc(6);
}

void rampColor(unsigned short saColor[], unsigned short sdColor[], unsigned short sRampAmount){

  unsigned short i;

  for (i=0; i < LED_STRIP_LEN; i++){
      saColor[i] = rampOneColor(saColor[i], sdColor[i], sRampAmount);
  }
  
}

unsigned short rampOneColor(unsigned short saColor, unsigned short sdColor, unsigned short sRampAmount){
  //changes at a rate of 37 / second
  //27ms update time

  int sDifference = sdColor - saColor;
  //int srampAmount = abs(sDifference) / (rampTimemS / 27);

  if (srampAmount < 1)
    srampAmount = 1;

  if (sDifference > srampAmount)
     sDifference = srampAmount;

  if (sDifference < -srampAmount)
     sDifference = -srampAmount;

  return saColor + sDifference;

}