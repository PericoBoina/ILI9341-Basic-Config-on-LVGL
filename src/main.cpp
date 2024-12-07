#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define DRAW_BUF_SIZE (TFT_WIDTH * 40)
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

XPT2046_Touchscreen touchscreen(TOUCH_CS, TOUCH_IRQ);

static uint32_t my_tick(void)
{
  return millis();
}

static lv_obj_t * slider_label;
static void slider_event_cb(lv_event_t * e);

/////////////////////////////////////////////////////////////////////////////////////////////////

void touchread(lv_indev_t *indev, lv_indev_data_t *indevData)
{
  if (touchscreen.touched())
  {
    TS_Point p = touchscreen.getPoint();

    indevData->point.x = map(p.x, 0, 4095, 0, TFT_WIDTH);
    indevData->point.y = map(p.y, 0, 4095, 0, TFT_HEIGHT);

    indevData->state = LV_INDEV_STATE_PRESSED;
  }
  else
  {
    indevData->state = LV_INDEV_STATE_RELEASED;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = (lv_obj_t *)lv_event_get_target(e); // Conversión explícita
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
}

/////////////////////////////////////// SETUP ////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  SPI.begin(TOUCH_SCK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);

  touchscreen.begin();
  touchscreen.setRotation(0);

  lv_init();
  lv_tick_set_cb(my_tick);

  lv_display_t *disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));
  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), 0);

  lv_obj_t *slider = lv_slider_create(lv_screen_active());
  lv_obj_align(slider, LV_ALIGN_TOP_LEFT, 20, 40);
  lv_obj_set_width(slider, 150);

  slider_label = lv_label_create(lv_screen_active());
  lv_label_set_text(slider_label, "0%");
  lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

  lv_obj_set_style_bg_color(slider, lv_color_make(255, 0, 0), 0);    

  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchread);
}

/////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

void loop()
{
  lv_task_handler();
  delay(5);
}
