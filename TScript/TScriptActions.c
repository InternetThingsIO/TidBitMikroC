/*
* This is where TScript commands are parsed and sent to the device driver to be executed
*
*/

#include "WS2812b\WS2812B.h"
#include "ST7735R\ST7735R.h"
#include "stdint.h"
#include "UART_Ext.h"
#include "TScriptCommon.h"
#include "SDCard.h"

const uint8_t RED = 0;
const uint8_t GREEN = 1;
const uint8_t BLUE = 2;

uint8_t *getRGB(uint8_t *ptrLine);
void ddelay_ms(unsigned int time);

//must be called first
//initializes any devices available for the action scripts
void TAction_InitDevices(){

     //set pins to output for SPI2 / LCD
     //SCK2
     TRISG.RG6 = 0; //hooked
     //SDO2
     TRISG.RG8 = 0; //hooked

     //so that our timers run
     EnableInterrupts();

     //init WS2812B
     WS2812b_init();

     //init SD Card
     SDCard_init();
     //init LCD
     ST7735R_Init();

}

void TAction_empty(SingleCommand *pCommand){


}

void TAction_animate_bmp(SingleCommand *pCommand){



}

void TAction_show_bmp(SingleCommand *pCommand){
    SingleCommand command;

    command = *pCommand;

    ST7735R_loadBitmapToLCD(command.Parameters[0]);
    LATB.B7 = 1;
}

void TAction_clear_lcd(SingleCommand *pCommand){
    writeStr("Clearing LCD");
    LATB.B7 = 0;
    ST7735_fillScreenRGB(0,0,0);
}

//delays for the specified time
void TAction_delay_ms(SingleCommand *pCommand){
    int delay, i;
    SingleCommand command;
    
    command = *pCommand;
    delay = atoi(command.Parameters[0]);
    delay /= 10;

    for (i = 0; i < delay; i++)
        delay_ms(10);
}

//wait for LED to finish ramping
void TAction_led_fade_complete(SingleCommand *pCommand){
    WS2812b_WaitRampComplete();
}

void TAction_led_set(SingleCommand *pCommand){

    uint8_t led_number , *RGB;
    SingleCommand command;
    
    command = *pCommand;

    led_number = atoi(command.Parameters[0]);
    RGB =   getRGB(command.Parameters[1]);

    WS2812b_setColor(led_number, RGB[RED], RGB[GREEN], RGB[BLUE]);
}

//sets ramp amount of LEDs
void TAction_led_fade(SingleCommand *pCommand){
    uint8_t amount;
    SingleCommand command;

    command = *pCommand;
    
    amount = atoi(command.Parameters[0]);
    
    WS2812b_setRampAmount(amount);
}

void TAction_led_range(SingleCommand *pCommand){
    uint8_t start, end, *RGB;
    SingleCommand command;

    command = *pCommand;
    
    start = atoi(command.Parameters[0]);
    end =   atoi(command.Parameters[1]);
    RGB =   getRGB(command.Parameters[2]);
    
    WS2812b_setColorRange(start, end, RGB[RED], RGB[GREEN], RGB[BLUE]);
}

void TAction_led_rotate_range(SingleCommand *pCommand){

    int i;
    short spaces, absSpaces, start, end;
    SingleCommand command;

    command = *pCommand;
    start = atoi(command.Parameters[0]);
    end = atoi(command.Parameters[1]);
    spaces = atoi(command.Parameters[2]);
    absSpaces = abs(spaces);

    WS2812b_rotate_range(start,end,spaces);

}

void TAction_led_rotate(SingleCommand *pCommand){

    int i;
    short spaces, absSpaces;
    SingleCommand command;

    command = *pCommand;

    spaces = atoi(command.Parameters[0]);

    WS2812b_rotate(spaces);
}

void ddelay_ms(unsigned int time){
    int i;
    time /= 10;

    for (i = 0; i< time; i++)
        delay_ms(10);
}

//Gets RGB value from comma seperated string
uint8_t *getRGB(uint8_t *ptrLine){
    static unsigned short RGB[3];
    uint8_t tmpLine[50];
    uint8_t *ptrEnd;

    //writeStr("Discovered RGB Values");

    memset(tmpLine, '\0', sizeof(tmpLine));

    ptrEnd = strchr(ptrLine, ',');

    memcpy(tmpLine, ptrLine, ptrEnd - ptrLine);
    //writeStr(tmpLine);
    RGB[RED] = atoi(tmpLine);

    memset(tmpLine, '\0', sizeof(tmpLine));
    ptrLine = ptrEnd;
    ptrLine = &ptrLine[1];
    ptrEnd = strchr(ptrLine, ',');
    memcpy(tmpLine, ptrLine, ptrEnd - ptrLine);
    //writeStr(tmpLine);
    RGB[Green] = atoi(tmpLine);

    memset(tmpLine, '\0', sizeof(tmpLine));
    ptrLine = ptrEnd;
    ptrLine = &ptrLine[1];
    //writeStr(ptrLine);
    RGB[Blue] = atoi(ptrLine);

    return RGB;
}