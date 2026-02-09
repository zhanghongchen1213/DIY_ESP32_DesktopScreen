#ifndef _DISPLAY_H
#define _DISPLAY_H


#include <stdbool.h>
#include <epd1in54.h>
#include <epdpaint.h>
#include <lvgl.h>

#define LCD_BL_PIN 5         // LCD背光控制引脚
#define LED_BL_PWM_CHANNEL 0 // LCD背光PWM控制

class Display
{
private:
public:
    void init();
    void refresh();
    void flip();
};

#endif
