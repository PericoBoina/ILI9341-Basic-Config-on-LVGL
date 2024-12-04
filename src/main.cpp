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

lv_obj_t *label_0;
lv_obj_t *label_1;

void setup()
{
  Serial.begin(115200);
  SPI.begin(TOUCH_SCK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
  touchscreen.begin();
  touchscreen.setRotation(1);
  while (!Serial)
    ;
  Serial.println("Touchscreen listo");

  lv_init();
  lv_tick_set_cb(my_tick);
  lv_display_t *disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(0, 0, 0), 0); // color fondo pantalla
}

void loop()
{
  if (!label_0)
  {
    label_0 = lv_label_create(lv_scr_act());
    lv_obj_align(label_0, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(label_0, lv_color_make(0, 0, 0), 0);
  }
  if (touchscreen.touched())
  {
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(0, 80, 0), 0);
    lv_label_set_text(label_0, "Touch");
  }
  else
  {
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(80, 0, 0), 0);
    lv_label_set_text(label_0, "No Touch");
  }
  lv_timer_handler();
  delay(5);
}