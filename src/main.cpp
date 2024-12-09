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
static lv_obj_t *btn_reset;
static lv_obj_t *btn_500;
static lv_obj_t *btn_5000;
static lv_obj_t *btn_50;

 int set_value = 500;

void update_value_display()
{
  char buf[6];
  snprintf(buf, sizeof(buf), "%d", set_value);
  lv_label_set_text(roller_label, buf);
  char value_str[6];
  snprintf(value_str, sizeof(value_str), "%05d", set_value);
  for (int i = 0; i < 5; i++)
  {
    lv_roller_set_selected(roller[i], value_str[i] - '0', LV_ANIM_ON);
  }
}

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
  Serial.print("Set value: ");
  Serial.println(buf);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static void btn_reset_event_cb(lv_event_t *e)
{
  for (int i = 0; i < 5; i++)
  {
    lv_roller_set_selected(roller[i], 0, LV_ANIM_ON);
  }

  lv_label_set_text(roller_label, "00000");
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static void btn_500_event_cb(lv_event_t *e)
{
  set_value = 500;
  update_value_display();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static void btn_5000_event_cb(lv_event_t *e)
{
  set_value = 5000;
  update_value_display();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static void btn_50_event_cb(lv_event_t *e)
{
  set_value = 50;
  update_value_display();
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

  // Rollers
  for (int i = 0; i < 5; i++)
  {
    roller[i] = lv_roller_create(lv_screen_active());
    lv_roller_set_options(roller[i], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller[i], 3);
    lv_obj_align(roller[i], LV_ALIGN_TOP_MID, (i - 2) * 40, 10);
    lv_obj_add_event_cb(roller[i], roller_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    if (i == 0)
      lv_roller_set_selected(roller[i], 5, LV_ANIM_OFF);
    else
      lv_roller_set_selected(roller[i], 0, LV_ANIM_OFF);
  }

  roller_label = lv_label_create(lv_screen_active());
  lv_label_set_text(roller_label, "500");
  lv_obj_align_to(roller_label, roller[2], LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_obj_set_style_text_color(roller_label, lv_color_make(255, 0, 128), 0);

  static lv_style_t style_label_black;
  lv_style_init(&style_label_black);
  lv_style_set_text_color(&style_label_black, lv_color_black());

  // Botón para valor 500
  btn_500 = lv_btn_create(lv_screen_active());
  lv_obj_set_size(btn_500, 60, 40);
  lv_obj_align(btn_500, LV_ALIGN_TOP_LEFT, 20, 160);
  lv_obj_add_event_cb(btn_500, btn_500_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t *label_500 = lv_label_create(btn_500);
  lv_label_set_text(label_500, "500");
  lv_obj_add_style(label_500, &style_label_black, 0);
  lv_obj_align(label_500, LV_ALIGN_CENTER, 0, 0);

  // Botón para valor 5000
  btn_5000 = lv_btn_create(lv_screen_active());
  lv_obj_set_size(btn_5000, 60, 40);
  lv_obj_align_to(btn_5000, btn_500, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_obj_add_event_cb(btn_5000, btn_5000_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t *label_5000 = lv_label_create(btn_5000);
  lv_label_set_text(label_5000, "5000");
  lv_obj_add_style(label_5000, &style_label_black, 0);
  lv_obj_align(label_5000, LV_ALIGN_CENTER, 0, 0);

  // Botón para valor 50
  btn_50 = lv_btn_create(lv_screen_active());
  lv_obj_set_size(btn_50, 60, 40);
  lv_obj_align_to(btn_50, btn_5000, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_obj_add_event_cb(btn_50, btn_50_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t *label_50 = lv_label_create(btn_50);
  lv_label_set_text(label_50, "50");
  lv_obj_add_style(label_50, &style_label_black, 0);
  lv_obj_align(label_50, LV_ALIGN_CENTER, 0, 0);

  // Botón para enviar
  btn_send = lv_btn_create(lv_screen_active());
  lv_obj_set_size(btn_send, 60, 40);
  lv_obj_align(btn_send, LV_ALIGN_TOP_LEFT, 40, 220);
  lv_obj_add_event_cb(btn_send, btn_send_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t *label_send = lv_label_create(btn_send);
  lv_label_set_text(label_send, "Set");
  lv_obj_add_style(label_send, &style_label_black, 0);
  lv_obj_align(label_send, LV_ALIGN_CENTER, 0, 0);

  // Botón para reset
  btn_reset = lv_btn_create(lv_screen_active());
  lv_obj_set_size(btn_reset, 60, 40);
  lv_obj_align_to(btn_reset, btn_send, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_obj_add_event_cb(btn_reset, btn_reset_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t *label_reset = lv_label_create(btn_reset);
  lv_label_set_text(label_reset, "Reset");
  lv_obj_add_style(label_reset, &style_label_black, 0);
  lv_obj_align(label_reset, LV_ALIGN_CENTER, 0, 0);

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
