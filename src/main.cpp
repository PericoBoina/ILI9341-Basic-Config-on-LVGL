#include <Arduino.h>
#include <lvgl.h>     // Biblioteca LVGL
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();     // Inicializa TFT_eSPI

/*Set to your screen resolution and rotation*/
#define TFT_ROTATION LV_DISPLAY_ROTATION_0

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];


/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

void setup()
{
    String LVGL_Arduino = "Hello @LabGluon! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.begin(115200);
    Serial.println(LVGL_Arduino);

    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);

    lv_display_t *disp;
    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));


    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello @LabGluon, I'm LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    Serial.println("Setup done");
}

void loop()
{
    lv_timer_handler(); // Procesa tareas de LVGL
    delay(5);           // Reduce la carga de la CPU
}