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

static lv_obj_t *roller_label;
static lv_obj_t *roller[5];
static lv_obj_t *btn_send;

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

static void roller_event_cb(lv_event_t *e)
{
  char buf[6];
  buf[5] = '\0';

  for (int i = 0; i < 5; i++)
  {
    char digit[2];
    lv_roller_get_selected_str(roller[i], digit, sizeof(digit));
    buf[i] = digit[0];
  }

  lv_label_set_text(roller_label, buf);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static void btn_send_event_cb(lv_event_t *e)
{
  char buf[6];
  buf[5] = '\0';

  for (int i = 0; i < 5; i++)
  {
    char digit[2];
    lv_roller_get_selected_str(roller[i], digit, sizeof(digit));
    buf[i] = digit[0];
  }

  Serial.print("Set: ");
  Serial.println(buf);
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

  for (int i = 0; i < 5; i++)
  {
    roller[i] = lv_roller_create(lv_screen_active());
    lv_roller_set_options(roller[i], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller[i], 3);
    lv_obj_align(roller[i], LV_ALIGN_TOP_MID, (i - 2) * 40, 0);
    lv_obj_add_event_cb(roller[i], roller_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  }

  static lv_style_t style_label;
  lv_style_init(&style_label);
  lv_style_set_text_color(&style_label, lv_color_white());
  lv_style_set_text_font(&style_label, &lv_font_montserrat_14);

  roller_label = lv_label_create(lv_screen_active());
  lv_label_set_text(roller_label, "00000");
  lv_obj_add_style(roller_label, &style_label, 0);
  lv_obj_align_to(roller_label, roller[2], LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

  btn_send = lv_btn_create(lv_screen_active());
  lv_obj_set_size(btn_send, 50, 50);
  lv_obj_align(btn_send, LV_ALIGN_BOTTOM_MID, 0, -20);

  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_bg_color(&style_btn, lv_color_make(255, 0, 0));
  lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
  lv_style_set_radius(&style_btn, 25);
  lv_style_set_border_width(&style_btn, 0);
  lv_style_set_text_color(&style_btn, lv_color_black());
  lv_style_set_text_font(&style_btn, &lv_font_montserrat_14);
  lv_obj_add_style(btn_send, &style_btn, 0);

  lv_obj_t *btn_label = lv_label_create(btn_send);
  lv_label_set_text(btn_label, "Set");
  lv_obj_align(btn_label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_event_cb(btn_send, btn_send_event_cb, LV_EVENT_CLICKED, NULL);

  ///////////////////// Configurar entrada táctil //////////////////////////

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
