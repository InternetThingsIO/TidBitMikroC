#include "WS2812b\WS2812B.h"
#include "ST7735R\ST7735R.h"
#include "stdint.h"
#include "UART_Ext.h"

const unsigned short RED = 0;
const unsigned short GREEN = 1;
const unsigned short BLUE = 2;

uint8_t* getLine(uint8_t *script);
uint8_t* setLEDFade(uint8_t *script);
uint8_t* findCommand(uint8_t *currPos);
uint8_t* advanceLine(uint8_t *currPos);
uint8_t* setLEDRange(uint8_t *currPos);
void delaymS(unsigned int ms);
uint8_t *getRGB(uint8_t *currPos);
uint8_t* setShowBMP(uint8_t *currPos);
uint8_t* setClearLCD(uint8_t *currPos);
uint8_t *rotateLEDs(uint8_t *currPos);

//must be called first
//initializes any devices available for the action scripts
void init_devices(){
    //init LCD
    writeShort(ST7735R_Init());
    //init WS2812B
    WS2812b_init();
}

//Responsible for parsing the script and performing actions
char process_script(uint8_t *script){

    uint8_t *currPos;
    
    currPos = script;

    //loop through commands and do them
    currPos = findCommand(currPos);
    //writeStr(currPos);
    while(currPos != 0){
        //writeStr("Finding command");
        currPos = findCommand(currPos);
    }

    writeStr("end of action script");
    
    return 1;
}

//Responsible for identifying individual commands and executing them
uint8_t* findCommand(uint8_t *currPos){
    uint8_t *command;
    command = getLine(currPos);
    
    if(command == 0)
        return 0;
        
    //add commands here like the ones below
    writeStr(command);

    if (strcmp(command, "led_fade") == 0){
        writeStr("found LED Fade");
        return setLEDFade(currPos);
    }

    if (strcmp(command, "led_range") == 0){
        writeStr("found LED Range");
        return setLEDRange(currPos);
    }

    if (strcmp(command, "show_bmp") == 0){
        writeStr("found show_bmp");
        return setShowBMP(currPos);
    }
    
    if (strcmp(command, "clear_lcd") == 0){
        writeStr("found clear_lcd");
        return setClearLCD(currPos);
    }
    
    if (strcmp(command, "rotate") == 0){
        writeStr("found clear_lcd");
        //return rotateLEDs(currPos);
    }

    //if we didn't find a command, look for it on the next line
    //writeStr("Didn't find command, advancing line");
    return advanceLine(currPos);

}
//returns the value of the text up until the CR LF indicating the start of a new line
uint8_t* getLine(uint8_t *script){
    static uint8_t currLine[50];
    
    uint8_t *begin, *end;
    memset(currLine, '\0', sizeof(currLine));
    
    //writeStr("Get Line");
    
    begin = script;
    end = strstr(script, "\r\n");
    
    if (end != 0){
        return memcpy(currLine, begin, end-begin);
    }else{
        writeStr("didn't find end of line");
        return 0;
    }

}

//returns a pointer to the next line
uint8_t* advanceLine(uint8_t *currPos){
    //writeStr("Advance Line");
    return strstr(currPos, "\r\n") + 2;
}

uint8_t* removeTabs(uint8_t *currPos){
    while(currPos[0] == '\t')
        currPos += 1;
    return currPos;
}

//rotates the LED ring
uint8_t *rotateLEDs(uint8_t *currPos){



}

//sets the ramp rate of the LEDs
uint8_t* setLEDFade(uint8_t *currPos){
    
    uint8_t *ptrLine;
    
    //get the next line
    currPos = advanceLine(currPos);

    ptrLine = getLine(currPos);

    if(ptrLine == 0)
        return 0;

    ptrLine = removeTabs(ptrLine);
    
    //set LED fade here wirh ptrLine!*****************************
    WS2812b_setRampAmount(atoi(ptrLine));

    return advanceLine(currPos);
}

uint8_t* setLEDRange(uint8_t *currPos){

    uint8_t *ptrLine;
    unsigned short start, end, *RGB;

    currPos = advanceLine(currPos);
    ptrLine = getLine(currPos);
    
    if(ptrLine == 0)
        return 0;
    
    //delay specified amount
    writeStr(ptrLine);
    delaymS(atoi(ptrLine));
    
    currPos = advanceLine(currPos);
    ptrLine = getLine(currPos);

    if(ptrLine == 0)
        return 0;

    ptrLine = removeTabs(ptrLine);
    writeStr(ptrLine);
    start = atoi(ptrLine);

    currPos = advanceLine(currPos);
    ptrLine = getLine(currPos);

    if(ptrLine == 0)
        return 0;

    ptrLine = removeTabs(ptrLine);
    writeStr(ptrLine);
    end = atoi(ptrLine);
    
    currPos = advanceLine(currPos);
    ptrLine = getLine(currPos);

    if(ptrLine == 0)
        return 0;

    ptrLine = removeTabs(ptrLine);
    writeStr(ptrLine);
    RGB = getRGB(ptrLine);
    
    writeShort(start);
    writeShort(end);
    writeInt(RGB[RED]);
    writeInt(RGB[GREEN]);
    writeInt(RGB[BLUE]);
    
    WS2812b_setColorRange(start, end, RGB[RED], RGB[GREEN], RGB[BLUE]);
    //WS2812b_setColorRange(0,35, 128, 128, 128);
    //WS2812b_Update();
    WS2812b_WaitRampComplete();
    
   return advanceLine(currPos);
}

uint8_t* setShowBMP(uint8_t *currPos){
    uint8_t *ptrLine;

    currPos = advanceLine(currPos);
    ptrLine = getLine(currPos);

    if(ptrLine == 0)
        return 0;

    //delay specified amount
    writeStr(ptrLine);
    delaymS(atoi(ptrLine));

    currPos = advanceLine(currPos);
    ptrLine = getLine(currPos);

    if(ptrLine == 0)
        return 0;

    ptrLine = removeTabs(ptrLine);
    writeStr(ptrLine);
    
    ST7735_fillScreen(RGB565(0,0,0));
    ST7735R_loadBitmapToLCD(ptrLine);
    
    return advanceLine(currPos);
    
}

uint8_t* setClearLCD(uint8_t *currPos){
    uint8_t *ptrLine;

    currPos = advanceLine(currPos);
    ptrLine = getLine(currPos);

    if(ptrLine == 0)
        return 0;

    //delay specified amount
    writeStr(ptrLine);
    delaymS(atoi(ptrLine));
    
    ST7735_fillScreen(RGB565(0,0,0));
    
    return advanceLine(currPos);
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

//creates a delay of the specified time
void delaymS(unsigned int ms){
    unsigned int i;
    ms /= 10;
    
    for (i = 0; i < ms; i++){
        delay_ms(10);
    }
}