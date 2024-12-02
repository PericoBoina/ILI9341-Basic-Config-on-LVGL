#include <Arduino.h>
#include <lvgl.h>     // Biblioteca LVGL
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();     // Inicializa TFT_eSPI

#define TFT_ROTATION LV_DISPLAY_ROTATION_0
#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
static uint32_t my_tick(void)
{
    return millis();
}

void setup()
{
    String LVGL_Arduino = "Hello @Perico_197! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.begin(115200);
    Serial.println(LVGL_Arduino);
    lv_init();
    lv_tick_set_cb(my_tick);
    lv_display_t *disp;
    disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello @Perico_197, I'm LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    Serial.println("Setup done");
}

void loop()
{
    lv_timer_handler(); // Procesa tareas de LVGL
    delay(5);           // Reduce la carga de la CPU
}