#include "stdint.h"

void WS2812b_init();
void WS2812b_update();
unsigned short WS2812b_RampComplete();
void WS2812b_setColorRange(unsigned short low, unsigned short high, unsigned short sR, unsigned short sG, unsigned short sB);
void WS2812b_setColor(unsigned short index, unsigned short sR, unsigned short sG, unsigned short sB);
void WS2812b_setRampAmount(unsigned short time);
void WS2812b_WaitRampComplete();
void WS2812b_rotate(int positions);
void WS2812b_rotate_range(uint8_t start, uint8_t end, int positions);