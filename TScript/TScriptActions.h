#include "TScriptCommon.h"

void TAction_InitDevices();
void TAction_delay_ms(SingleCommand *pCommand);
void TAction_led_fade_complete(SingleCommand *pCommand);
void TAction_led_fade(SingleCommand *pCommand);
void TAction_led_range(SingleCommand *pCommand);
void TAction_led_rotate(SingleCommand *pCommand);
void TAction_led_rotate_range(SingleCommand *pCommand);
void TAction_empty(SingleCommand *pCommand);
void TAction_led_set(SingleCommand *pCommand);
void TAction_show_bmp(SingleCommand *pCommand);
void TAction_clear_lcd(SingleCommand *pCommand);
void TAction_animate_bmp(SingleCommand *pCommand);
void TAction_rotateColor(SingleCommand *pCommand);