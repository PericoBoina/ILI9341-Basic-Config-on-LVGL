#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define TFT_ROTATION LV_DISPLAY_ROTATION_0
#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))

uint32_t draw_buf[DRAW_BUF_SIZE / 4];

static uint32_t my_tick(void)
{
    return millis();
}

lv_obj_t *bar1;
lv_obj_t *bar2;

static void set_temp(void *bar, int32_t temp)
{
    lv_bar_set_value((lv_obj_t *)bar, temp, LV_ANIM_ON);
}

void setup()
{
    lv_init();
    lv_tick_set_cb(my_tick);
    lv_display_t *disp;
    disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello @Perico_197 in LVGL");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -140);

    bar1 = lv_bar_create(lv_screen_active());
    lv_obj_set_size(bar1, 150, 20);
    lv_obj_align(bar1, LV_ALIGN_CENTER, 0, -115);
    lv_bar_set_value(bar1, 75, LV_ANIM_ON);

    bar2 = lv_bar_create(lv_screen_active());
    lv_obj_set_size(bar2, 150, 20);
    lv_obj_align(bar2, LV_ALIGN_CENTER, 0, -85);
    lv_bar_set_value(bar2, 60, LV_ANIM_ON);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_temp);
    lv_anim_set_duration(&a, 1500);
    lv_anim_set_playback_duration(&a, 1500);
    lv_anim_set_var(&a, bar2);
    lv_anim_set_values(&a, 0 , 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);


}

void loop()
{
    lv_timer_handler(); // Procesa tareas de LVGL
    delay(5);  
}