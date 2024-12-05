#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <esp_now.h>
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();

#define DRAW_BUF_SIZE (TFT_WIDTH * 40) // Reducido a 40 filas
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

XPT2046_Touchscreen touchscreen(TOUCH_CS, TOUCH_IRQ);

static uint32_t my_tick(void)
{
    return millis();
}

lv_obj_t *button;
lv_obj_t *button_1;
lv_obj_t *label_0;
lv_obj_t *label_1;
lv_obj_t *label_2;

const char txt_label_2[] PROGMEM = "Hi! @Perico_197 I'm LVGL";
const char txt_button_0[] PROGMEM = "Pulsador 1";
const char txt_button_1[] PROGMEM = "Pulsador 2";

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

static void button_event(lv_event_t *e)
{
  lv_obj_t *button = (lv_obj_t *)lv_event_get_target(e);
  if (lv_event_get_code(e) == LV_EVENT_CLICKED)
  {
    Serial.println("1");
  }
}

static void button1_event(lv_event_t *e)
{
  lv_obj_t *button_1 = (lv_obj_t *)lv_event_get_target(e);
  if (lv_event_get_code(e) == LV_EVENT_CLICKED)
  {
    Serial.println("2");
  }
}

void setup()
{
    Serial.begin(115200);
    SPI.begin(TOUCH_SCK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
    touchscreen.begin();
    touchscreen.setRotation(0);

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error al inicializar ESP-NOW");
        return;
    }

    lv_init();
    lv_tick_set_cb(my_tick);

    lv_display_t *disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_white(), 0);

    button = lv_button_create(lv_scr_act());
    lv_obj_align(button, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_size(button, 180, 50);
    lv_obj_add_event_cb(button, button_event, LV_EVENT_CLICKED, NULL);

    button_1 = lv_button_create(lv_scr_act());
    lv_obj_align(button_1, LV_ALIGN_TOP_MID, 0, 90);
    lv_obj_set_size(button_1, 180, 50);
    lv_obj_add_event_cb(button_1, button1_event, LV_EVENT_CLICKED, NULL);

    label_2 = lv_label_create(lv_screen_active());
    lv_label_set_text_static(label_2, txt_label_2);
    lv_obj_align(label_2, LV_ALIGN_TOP_MID, 0, 5);

    label_0 = lv_label_create(button);
    lv_label_set_text_static(label_0, txt_button_0);
    lv_obj_center(label_0);

    label_1 = lv_label_create(button_1);
    lv_label_set_text_static(label_1, txt_button_1);
    lv_obj_center(label_1);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touchread);
}

void loop()
{
    lv_task_handler();
    delay(5);
}
