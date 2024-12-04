#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();


#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];


XPT2046_Touchscreen touchscreen(TOUCH_CS, TOUCH_IRQ);


static uint32_t my_tick(void)
{
  return millis();
}


lv_obj_t *button;
lv_obj_t *label_0;

void your_indev_read_cb(lv_indev_t *indev, lv_indev_data_t *indevData)
{
  if (touchscreen.touched())
  {
    TS_Point p = touchscreen.getPoint();

    indevData->point.x = map(p.x, 230, 3920, 1, TFT_WIDTH);
    indevData->point.y = map(p.y, 400, 3905, 1, TFT_HEIGHT);

    indevData->state = LV_INDEV_STATE_PRESSED;
  }
  else
  {
    indevData->state = LV_INDEV_STATE_RELEASED;
  }
}

static void btn_event_cb(lv_event_t *e)
{
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  if (lv_event_get_code(e) == LV_EVENT_CLICKED)
  {

    Serial.println("¡Botón Presionado!");
  }
}

void setup()
{
  Serial.begin(115200);
  SPI.begin(TOUCH_SCK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
  touchscreen.begin();
  touchscreen.setRotation(0);
  while (!Serial)
    ;
  Serial.println("Touchscreen listo");

  lv_init();
  lv_tick_set_cb(my_tick);

  lv_display_t *disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(0, 0, 0), 0);

  button = lv_button_create(lv_scr_act());
  lv_obj_align(button, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(button, 180, 50);

  // Crear la etiqueta dentro del botón
  label_0 = lv_label_create(button);
  lv_label_set_text(label_0, "Prees Me!");
  lv_obj_center(label_0); 


   lv_obj_add_event_cb(button, btn_event_cb, LV_EVENT_CLICKED, NULL);

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, your_indev_read_cb); 


}

void loop()
{
  lv_task_handler();
  lv_tick_inc(5);
  delay(5);
}
