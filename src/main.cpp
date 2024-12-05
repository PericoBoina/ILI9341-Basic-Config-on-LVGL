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
lv_obj_t *button_1;
lv_obj_t *label_0;
lv_obj_t *label_1;
lv_obj_t *label_2;
lv_obj_t *scale;
lv_obj_t *needle;

////////////////////// leer el touchpad ///////////////////////////

void touchread(lv_indev_t *indev, lv_indev_data_t *indevData)
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
///////////////////////////////////////////////////////////////////

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
  while (!Serial)
    ;
  Serial.println("Touchscreen listo");

  lv_init();
  lv_tick_set_cb(my_tick);

  lv_display_t *disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(255, 255, 255), 0);

  button = lv_button_create(lv_scr_act());
  lv_obj_align(button, LV_ALIGN_TOP_MID, 0, 30);
  lv_obj_set_size(button, 180, 50);
  lv_obj_set_style_bg_color(button, lv_color_make(0, 50, 180), 0);
  lv_obj_set_style_bg_opa(button, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(button, lv_color_make(0, 0, 0), 0);
  lv_obj_set_style_border_width(button, 5, 0);

  button_1 = lv_button_create(lv_scr_act());
  lv_obj_align(button_1, LV_ALIGN_TOP_MID, 0, 90);
  lv_obj_set_size(button_1, 180, 50);
  // Cambiar el color de fondo del botón
  lv_obj_set_style_bg_color(button_1, lv_color_make(0, 50, 180), 0);
  lv_obj_set_style_bg_opa(button_1, LV_OPA_COVER, 0); // Hacer el fondo opaco
  // Cambiar el color del borde a lila
  lv_obj_set_style_border_color(button_1, lv_color_make(0, 0, 0), 0);
  lv_obj_set_style_border_width(button_1, 5, 0); // Ancho del borde

  label_2 = lv_label_create(lv_screen_active());
  lv_label_set_text(label_2, "Hi! @Perico_197 I'm LVGL");
  lv_obj_align(label_2, LV_ALIGN_TOP_MID, 0, 5);
  lv_obj_set_style_text_color(label_2, lv_color_make(160, 0, 80), 0);

  label_0 = lv_label_create(button);
  lv_label_set_text(label_0, "Pulsador 1");
  lv_obj_center(label_0);
  lv_obj_set_style_text_color(label_0, lv_color_make(0, 0, 0), 0);

  label_1 = lv_label_create(button_1);
  lv_label_set_text(label_1, "Pulsador 2");
  lv_obj_center(label_1);
  lv_obj_set_style_text_color(label_1, lv_color_make(0, 0, 0), 0);

  scale = lv_scale_create(lv_screen_active());
  lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_INNER);
  lv_obj_set_style_bg_opa(scale, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(scale, lv_palette_lighten(LV_PALETTE_RED, 5), 0);
  lv_obj_set_style_radius(scale, LV_RADIUS_CIRCLE, 0);
  lv_obj_align(scale, LV_ALIGN_TOP_MID, 0, 175);

  //  agregar cada evento a su pulsador

  lv_obj_add_event_cb(button, button_event, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(button_1, button1_event, LV_EVENT_CLICKED, NULL);

  // Implementacion del tactil en lvgl //

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchread); 


}

void loop()
{
  lv_task_handler();
  lv_tick_inc(5);
  delay(5);
}
