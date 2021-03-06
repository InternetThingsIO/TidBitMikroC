#include "stdint.h"

#define swap(a, b) { int t = a; a = b; b = t; }


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160


#define ST7735_NOP 0x0
#define ST7735_SWRESET 0x01
#define ST7735_RDDID 0x04
#define ST7735_RDDST 0x09

#define ST7735_SLPIN  0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON  0x12
#define ST7735_NORON  0x13

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E

#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36


#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define RGB565(r,g,b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))

unsigned short ST7735R_Init();
void ST7735R_writeColor(unsigned int color);
void ST7735_fillScreen(unsigned int color);
void ST7735_drawString(unsigned char x, unsigned char y, char *c, unsigned int color, unsigned char size);
void ST7735_drawChar(unsigned char x, unsigned char y, char c, unsigned int color, unsigned char size);
void ST7735_drawPixel(unsigned char x, unsigned char y, unsigned int color);
void ST7735_fillRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned int color);
unsigned short ST7735R_loadBitmapToLCD(char filename[]);
void ST7735_fillScreenRGB(uint8_t R, uint8_t G, uint8_t B);