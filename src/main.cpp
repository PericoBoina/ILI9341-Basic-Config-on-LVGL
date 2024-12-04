#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

XPT2046_Touchscreen touchscreen(TOUCH_CS, TOUCH_IRQ);
TS_Point touchPoint;

static uint32_t my_tick(void)
{
  return millis();
}

void setup()
{
  Serial.begin(115200);
  SPI.begin(TOUCH_SCK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);

  touchscreen.begin();
  touchscreen.setRotation(1);

  lv_init();
  lv_tick_set_cb(my_tick);
  lv_display_t *disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(0, 0, 0), 0); // color fondo pantalla

  while (!Serial)
    ;
  Serial.println("Touchscreen listo. Tocando para leer datos...");



}

void loop()
{
  if (touchscreen.touched())
  {
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(0, 0, 80), 0);
  }
  else
  {
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(0, 0, 0), 0);
    delay(5);
  }
  lv_timer_handler();
  delay(2);
}