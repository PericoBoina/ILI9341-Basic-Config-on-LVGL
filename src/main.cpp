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

static lv_obj_t *slider_label;
static lv_obj_t *slider_label1;
static lv_obj_t *slider_label2;
static void slider_event_cb(lv_event_t *e);
static void slider1_event_cb(lv_event_t *e);
static void slider2_event_cb(lv_event_t *e);
static lv_obj_t *moving_label;
static int label_x = TFT_WIDTH;
static bool moving_right = false;

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

static void slider_event_cb(lv_event_t *e)
{
  lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e); 
  char buf[8];
  lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
  lv_label_set_text(slider_label, buf);
  lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
}

static void slider1_event_cb(lv_event_t *e)
{
  lv_obj_t *slider1 = (lv_obj_t *)lv_event_get_target(e); 
  char buf[8];
  lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider1));
  lv_label_set_text(slider_label1, buf);
  lv_obj_align_to(slider_label1, slider1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
}

static void slider2_event_cb(lv_event_t *e)
{
  lv_obj_t *slider2 = (lv_obj_t *)lv_event_get_target(e); 
  char buf[8];
  lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider2));
  lv_label_set_text(slider_label2, buf);
  lv_obj_align_to(slider_label2, slider2, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
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

   ///////////////////// Slider 1 //////////////////////////
  lv_obj_t *slider = lv_slider_create(lv_screen_active());
  lv_obj_align(slider, LV_ALIGN_TOP_LEFT, 20, 40); // Alineado a 20px desde la izquierda y 40px desde arriba
  lv_obj_set_width(slider, 150);

  slider_label = lv_label_create(lv_screen_active());
  lv_label_set_text(slider_label, "0%");
  lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

  lv_obj_set_style_bg_color(slider, lv_color_make(255, 0, 0), 0);    

  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  ///////////////////// Slider 2 //////////////////////////
  lv_obj_t *slider1 = lv_slider_create(lv_screen_active());
  lv_obj_align(slider1, LV_ALIGN_TOP_LEFT, 20, 40 + 30 + 20); // 30px debajo del primero (40+altura del slider+30)
  lv_obj_set_width(slider1, 150);

  slider_label1 = lv_label_create(lv_screen_active());
  lv_label_set_text(slider_label1, "0%");
  lv_obj_align_to(slider_label1, slider1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

  lv_obj_set_style_bg_color(slider1, lv_color_make(0, 255, 0), 0);    

  lv_obj_add_event_cb(slider1, slider1_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  ///////////////////// Slider 3 //////////////////////////

  lv_obj_t *slider2 = lv_slider_create(lv_screen_active());
  lv_obj_align(slider2, LV_ALIGN_TOP_LEFT, 20, 40 + 30 + 20 + 30 + 20); // 30px debajo del segundo (40 + 30 + 20 + 30)
  lv_obj_set_width(slider2, 150);

  slider_label2 = lv_label_create(lv_screen_active());
  lv_label_set_text(slider_label2, "0%");
  lv_obj_align_to(slider_label2, slider2, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

  lv_obj_set_style_bg_color(slider2, lv_color_make(0, 0, 255), 0);

  lv_obj_add_event_cb(slider2, slider2_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  ///////////////////// Etiqueta en movimiento //////////////////////////

  moving_label = lv_label_create(lv_screen_active());
  lv_label_set_text(moving_label, "I'm @Perico_197 in LVGL!");
  lv_obj_align(moving_label, LV_ALIGN_BOTTOM_LEFT, 0, -10);

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchread);
}

/////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

void loop()
{
  lv_task_handler();
 if (moving_right) {
    label_x += 1;  // Mover hacia la derecha
    if (label_x + lv_obj_get_width(moving_label) >= 235) {  // Si ha llegado al borde derecho
      moving_right = false;  // Cambiar dirección
    }
  } else {
    label_x -= 1;  // Mover hacia la izquierda
    if (label_x <= 0) {  // Si ha llegado al borde izquierdo
      moving_right = true;  // Cambiar dirección
    }
  }

  lv_obj_set_x(moving_label, label_x); 
  delay(5);
}
