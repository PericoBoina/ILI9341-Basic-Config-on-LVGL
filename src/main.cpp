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

static lv_obj_t *spinner;
static lv_obj_t *switch_obj;
static lv_obj_t *switch_label; 
static lv_obj_t *roller_label; // Etiqueta para mostrar el valor del roller
static lv_obj_t *roller;      // El componente roller

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

static void switch_event_cb(lv_event_t *e)
{
  lv_obj_t *sw = (lv_obj_t *)lv_event_get_target(e); 

  if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
    lv_label_set_text(switch_label, "ON");
    lv_obj_clear_flag(spinner, LV_OBJ_FLAG_HIDDEN); 
  } else {
    lv_label_set_text(switch_label, "OFF");
    lv_obj_add_flag(spinner, LV_OBJ_FLAG_HIDDEN); 
  }
}

static void roller_event_cb(lv_event_t *e)
{
  lv_obj_t *roller = (lv_obj_t *)lv_event_get_target(e); // Conversión explícita
  char buf[4];
  lv_roller_get_selected_str(roller, buf, sizeof(buf)); // Obtiene el texto seleccionado
  lv_label_set_text(roller_label, buf); // Actualiza la etiqueta con el texto seleccionado
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

  ///////////////////// Spinner //////////////////////////////////////////////

  spinner = lv_spinner_create(lv_screen_active());
  lv_obj_set_size(spinner, 50, 50);
  lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 0);
  lv_spinner_set_anim_params(spinner, 1000, 200);
  lv_obj_set_style_arc_color(spinner, lv_color_make(0, 255, 0), LV_PART_INDICATOR);
  lv_obj_add_flag(spinner, LV_OBJ_FLAG_HIDDEN);

  ///////////////////// Switch y etiqueta //////////////////////////

  static lv_style_t style_switch;
  lv_style_init(&style_switch);
  lv_style_set_bg_color(&style_switch, lv_color_make(40, 40, 40));  
  lv_style_set_bg_opa(&style_switch, LV_OPA_COVER); 
  lv_style_set_border_color(&style_switch, lv_color_make(255, 255, 255)); 
  lv_style_set_border_width(&style_switch, 2); 
  lv_style_set_radius(&style_switch, 20); 

  switch_obj = lv_switch_create(lv_screen_active());
  lv_obj_add_style(switch_obj, &style_switch, 0);  
  lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 70); 
  lv_obj_add_event_cb(switch_obj, switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  static lv_style_t style_label;
  lv_style_init(&style_label);
  lv_style_set_text_color(&style_label, lv_color_white());  
  lv_style_set_text_font(&style_label, &lv_font_montserrat_14); 

  switch_label = lv_label_create(lv_screen_active());
  lv_label_set_text(switch_label, "OFF");
  lv_obj_add_style(switch_label, &style_label, 0); 
  lv_obj_align_to(switch_label, switch_obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 10); 

  ///////////////////// Roller y etiqueta //////////////////////////

  roller = lv_roller_create(lv_screen_active());
  lv_roller_set_options(roller, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
  lv_roller_set_visible_row_count(roller, 3);
  lv_obj_align(roller, LV_ALIGN_LEFT_MID, 0, -90); 
  lv_obj_add_event_cb(roller, roller_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  roller_label = lv_label_create(lv_screen_active());
  lv_label_set_text(roller_label, "0"); // Inicializa con "0"
  lv_obj_add_style(roller_label, &style_label, 0); 
  lv_obj_align_to(roller_label, roller, LV_ALIGN_OUT_BOTTOM_MID, 0, 10); 

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
