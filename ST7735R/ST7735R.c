//USES SPI #3
#include "ST7735R.h"
#include "font.h"
#include "UART_Ext.h"
////#include "MDD File System/SD-SPI.h"

#define LCD_RESET   LATE.RE3 //hooked
#define LCD_CS      LATD.RD0 //hooked
#define LCD_RS      LATB.RB15 //hooked to DC
#define SD_CS       LATE.RE0 //hooked

sbit Mmc_Chip_Select at LATE0_bit;
sbit Mmc_Chip_Select_Direction at TRISE0_bit;

unsigned short initSDCard();
void initR(void);
void writecommand(unsigned char c);
void writedata(unsigned char c);
void setAddrWindow(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);

char madctl;

//Initalize the LCD
unsigned short ST7735R_Init(){

     unsigned short SDInit;

     //set pins to output
     //SCK2
     TRISG.RG6 = 0; //hooked
     //SS2
     TRISD.RD0 = 0;  //hooked
     //SDO2
     TRISG.RG8 = 0; //hooked
     //RESET
     TRISE.RE3 = 0; //hooked
     //D/C
     TRISB.RB15 = 0; //hooked

     //SPI3_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 2, _SPI_SS_ENABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_IDLE_2_ACTIVE);
     SPI2_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 64, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH, _SPI_ACTIVE_2_IDLE);
     //initialize SD Card
     SDInit = initSDCard();
     
     writeShort(SDInit);
     writeStr(" :initSDCard");
     
     SPI2_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 8, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH, _SPI_ACTIVE_2_IDLE);
     //initialize the LCD
     initR();
     //clear the LCD to black
     ST7735_fillScreen(RGB565(0,0,0));
     
     return SDInit;
}

unsigned short initSDCard(){
     return Mmc_Fat_Init();
}

void initR(void) {
        LCD_RESET = 1;
        delay_us(500);
        LCD_RESET = 0;
        delay_us(500);
        LCD_RESET = 1;
        delay_us(500);

        LCD_CS = 0;

        writecommand(ST7735_SWRESET); // software reset
        delay_us(150);

        writecommand(ST7735_SLPOUT);  // out of sleep mode
        delay_us(500);

        writecommand(ST7735_COLMOD);  // set color mode
        writedata(0x05);        // 16-bit color
        delay_us(10);

        writecommand(ST7735_FRMCTR1);  // frame rate control - normal mode
        writedata(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
        writedata(0x2C);
        writedata(0x2D);

        writecommand(ST7735_FRMCTR2);  // frame rate control - idle mode
        writedata(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
        writedata(0x2C);
        writedata(0x2D);

        writecommand(ST7735_FRMCTR3);  // frame rate control - partial mode
        writedata(0x01); // dot inversion mode
        writedata(0x2C);
        writedata(0x2D);
        writedata(0x01); // line inversion mode
        writedata(0x2C);
        writedata(0x2D);

        writecommand(ST7735_INVCTR);  // display inversion control
        writedata(0x07);  // no inversion

        writecommand(ST7735_PWCTR1);  // power control
        writedata(0xA2);
        writedata(0x02);      // -4.6V
        writedata(0x84);      // AUTO mode

        writecommand(ST7735_PWCTR2);  // power control
        writedata(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

        writecommand(ST7735_PWCTR3);  // power control
        writedata(0x0A);      // Opamp current small
        writedata(0x00);      // Boost frequency

        writecommand(ST7735_PWCTR4);  // power control
        writedata(0x8A);      // BCLK/2, Opamp current small & Medium low
        writedata(0x2A);

        writecommand(ST7735_PWCTR5);  // power control
        writedata(0x8A);
        writedata(0xEE);

        writecommand(ST7735_VMCTR1);  // power control
        writedata(0x0E);

        writecommand(ST7735_INVOFF);    // don't invert display

        writecommand(ST7735_MADCTL);  // memory access control (directions)

        // http://www.adafruit.com/forums/viewtopic.php?f=47&p=180341

        // R and B byte are swapped
        // madctl = 0xC8;

        // normal R G B order
        madctl = 0xC0;
        writedata(madctl);  // row address/col address, bottom to top refresh

        writecommand(ST7735_COLMOD);  // set color mode
        writedata(0x05);        // 16-bit color

        writecommand(ST7735_CASET);  // column addr set
        writedata(0x00);
        writedata(0x00);   // XSTART = 0
        writedata(0x00);
        writedata(0x7F);   // XEND = 127

        writecommand(ST7735_RASET);  // row addr set
        writedata(0x00);
        writedata(0x00);    // XSTART = 0
        writedata(0x00);
        writedata(0x9F);    // XEND = 159

        writecommand(ST7735_GMCTRP1);
        writedata(0x0f);
        writedata(0x1a);
        writedata(0x0f);
        writedata(0x18);
        writedata(0x2f);
        writedata(0x28);
        writedata(0x20);
        writedata(0x22);
        writedata(0x1f);
        writedata(0x1b);
        writedata(0x23);
        writedata(0x37);
        writedata(0x00);
        writedata(0x07);
        writedata(0x02);
        writedata(0x10);
        writecommand(ST7735_GMCTRN1);
        writedata(0x0f);
        writedata(0x1b);
        writedata(0x0f);
        writedata(0x17);
        writedata(0x33);
        writedata(0x2c);
        writedata(0x29);
        writedata(0x2e);
        writedata(0x30);
        writedata(0x30);
        writedata(0x39);
        writedata(0x3f);
        writedata(0x00);
        writedata(0x07);
        writedata(0x03);
        writedata(0x10);

        writecommand(ST7735_DISPON);
        delay_us(100);

        writecommand(ST7735_NORON);  // normal display on
        delay_us(10);

        LCD_CS = 1;
}

//void ST7735R_pushColor(unsigned int color){
//     LATD.RD8 = 1;
//     SPI2_Write(color>>8);
//     SPI2_Write(color);
//}

void setAddrWindow(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
        writecommand(ST7735_CASET);  // column addr set
        writedata(0x00);
        writedata(x0+0);   // XSTART
        writedata(0x00);
        writedata(x1+0);   // XEND

        writecommand(ST7735_RASET);  // row addr set
        writedata(0x00);
        writedata(y0+0);    // YSTART
        writedata(0x00);
        writedata(y1+0);    // YEND

        writecommand(ST7735_RAMWR);  // write to RAM
}

void ST7735_fillScreenRGB(uint8_t R, uint8_t G, uint8_t B){

    ST7735_fillScreen(RGB565(R,G,B));

}

void ST7735_fillScreen(unsigned int color)
{
        unsigned char x;
        unsigned char y;
        unsigned char colorB;

        SPI_Set_Active(SPI2_Read, SPI2_Write);
        
        setAddrWindow(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1);

        // setup for data
        LCD_RS = 1;
        LCD_CS = 0;


        colorB = color >> 8;

        for (x=0; x < SCREEN_WIDTH; x++) {
                for (y=0; y < SCREEN_HEIGHT; y++) {
                        SPI2_write(colorB);
                        SPI2_write(color);
                }
        }

        LCD_CS = 1;
}

void ST7735_drawString(unsigned char x, unsigned char y, char *c, unsigned int color, unsigned char size)
{
        while (c[0] != 0) {
                ST7735_drawChar(x, y, c[0], color, size);
                x += size*6;
                c++;
                if (x + 5 >= SCREEN_WIDTH) {
                        y += 10;
                        x = 0;
                }
        }
}

void ST7735_drawChar(unsigned char x, unsigned char y, char c, unsigned int color, unsigned char size)
{
        unsigned char i, j;

        unsigned char letter = c < 0x52 ? c - 0x20 : c - 0x52;
        for (i =0; i<5; i++ ) {
                unsigned char line = c < 0x52 ? Alpha1[letter*5+i] : Alpha2[letter*5+i];

                for (j = 0; j<8; j++) {
                        if (line & 0x1) {
                                if (size == 1) // default size
                                ST7735_drawPixel(x+i, y+j, color);
                                else {  // big size
                                        ST7735_fillRect(x+i*size, y+j*size, size, size, color);
                                }
                        }
                        line >>= 1;
                }
        }
}

void ST7735_drawPixel(unsigned char x, unsigned char y, unsigned int color)
{
        setAddrWindow(x, y, x+1, y+1);

        LCD_RS = 1;
        LCD_CS = 0;

        SPI2_write(color >> 8);
        SPI2_write(color);

        LCD_CS = 1;
}

void ST7735_fillRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned int color) {
     unsigned char colorB;

        setAddrWindow(x, y, x+w-1, y+h-1);

        // setup for data
        LCD_RS = 1;
        LCD_CS = 0;

        colorB = color >> 8;
        for (x=0; x < w; x++) {
                for (y=0; y < h; y++) {
                        SPI2_write(colorB);
                        SPI2_write(color);
                }
        }

        LCD_CS = 1;
}

void writecommand(unsigned char c)
{
        LCD_RS = 0;
        LCD_CS = 0;
        SPI2_write(c);
        LCD_CS = 1;
}

void writedata(unsigned char c)
{
        LCD_RS = 1;
        LCD_CS = 0;
        SPI2_write(c);
        LCD_CS = 1;
}

unsigned short ST7735R_loadBitmapToLCD(char filename[])
{

    #define LCD_WIDTH 160
    #define LCD_HEIGHT 128

    unsigned char byte0, byte1;
    unsigned char x = 0;
    unsigned char y = 0;
    unsigned short xOffset, yOffset;
    uint8_t tmp[100];

    unsigned int bitmapWidth = 0;
    unsigned int bitmapHeight = 0;
    unsigned int headerSize = 0;
    

    unsigned long size;
    
    SPI_Set_Active(SPI2_Read, SPI2_Write);
    


    writeStr("Check if file exists");
    writeStr(filename);
    if (Mmc_Fat_Exists(filename) == 0){
        writeStr("File does not exist");
        return 1;
    }
    writeStr("File exists");
    

    writeStr("Assign file to SD");

    if (Mmc_Fat_Assign(filename, 0) == 0)
       return 1;
    
    writeStr("Fat_Reset");
    Mmc_Fat_Reset(&size);
    

    //check if this is BM type BMP.  Retur 0 if it is not
    Mmc_Fat_ReadN(tmp, 54);
    if (tmp[0] != 'B' && tmp[1] != 'M')
        return 0;

    //get the header size

    headerSize = tmp[14] + (tmp[15] << 8) + (tmp[16] << 12) + (tmp[17] << 16);
    writeInt(headerSize);

    bitmapWidth =  tmp[18] + (tmp[19] << 8) + (tmp[20] << 12) + (tmp[21] << 16);
    writeInt(bitmapWidth);
    bitmapHeight =  tmp[22] + (tmp[23] << 8) + (tmp[24] << 12) + (tmp[25] << 16);
    writeInt(bitmapHeight);
    
    //writeStr("Fat_Reset");
    //Mmc_Fat_Reset(&size);
    //for (x = 0; x < BMP_HEADER_SIZE; x++) { Mmc_Fat_Read(&byte0); }
    
    xOffset = (LCD_WIDTH / 2) - (bitmapWidth /2);
    yOffset = (LCD_HEIGHT / 2) - (bitmapHeight /2);
    
    writeInt(xOffset);
    writeInt(yOffset);
    
    for (y = 0; y < bitmapHeight; y++){
        for (x = 0; x < bitmapWidth; x++){
            unsigned short red, green, blue;
            unsigned int color;
            Mmc_Fat_Read(&blue);
            Mmc_Fat_Read(&green);
            Mmc_Fat_Read(&red);
        
            color = RGB565(red, green, blue);
            ST7735_drawPixel(bitmapHeight - y-1 + yOffset, bitmapWidth - x-1 + xOffset, color);
            //ST7735_drawPixel(bitmapHeight - x-1 + xOffset, bitmapWidth - y-1 + yOffset, color);
        }
    }

    return 1;

}

//uint8_t* getHeader(short