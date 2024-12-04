#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

XPT2046_Touchscreen touchscreen(TOUCH_CS, TOUCH_IRQ);
TS_Point touchPoint;

TFT_eSPI tft = TFT_eSPI();

static uint32_t my_tick(void)
{
    return millis();
}


lv_obj_t *label_1;  
lv_obj_t *button;


void setup()
{

}

void loop()
{

}
