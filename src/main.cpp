#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))

uint32_t draw_buf[DRAW_BUF_SIZE / 4];

static uint32_t my_tick(void)
{
    return millis();
}

lv_obj_t *bar1;
lv_obj_t *bar2;
lv_obj_t *bar1_label;
lv_obj_t *bar2_label;

void setup()
{
    Serial.begin(115200);
    lv_init();
    lv_tick_set_cb(my_tick);
    lv_display_t *disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));

    // Etiqueta principal
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello @Perico_197 in LVGL");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

    // Barra 1
    bar1 = lv_bar_create(lv_screen_active());
    lv_obj_set_size(bar1, 185, 20);
    lv_obj_align(bar1, LV_ALIGN_TOP_LEFT, 10, 50);
    lv_bar_set_value(bar1, 0, LV_ANIM_OFF);

    // Etiqueta para el valor de la barra 1
    bar1_label = lv_label_create(lv_screen_active());
    lv_label_set_text(bar1_label, "0"); // Valor inicial
    lv_obj_align_to(bar1_label, bar1, LV_ALIGN_OUT_RIGHT_MID, 10, 0); // Alineada a la derecha de la barra

    // Barra 2
    bar2 = lv_bar_create(lv_screen_active());
    lv_obj_set_size(bar2, 185, 20);
    lv_obj_align(bar2, LV_ALIGN_TOP_LEFT, 10, 90);
    lv_bar_set_value(bar2, 0, LV_ANIM_OFF);

    // Etiqueta para el valor de la barra 2
    bar2_label = lv_label_create(lv_screen_active());
    lv_label_set_text(bar2_label, "0"); // Valor inicial
    lv_obj_align_to(bar2_label, bar2, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
}

void loop()
{
    lv_timer_handler();

    if (Serial.available() > 0)
    {
        String received = Serial.readString();
        uint32_t data = received.toInt();

        // Actualizar valor de la barra 1
        lv_bar_set_value(bar1, data, LV_ANIM_OFF);
        lv_label_set_text_fmt(bar1_label, "%u", data); // Actualizar etiqueta

        // Actualizar valor de la barra 2 (puedes usar otra lógica si lo necesitas)
        lv_bar_set_value(bar2, data / 2, LV_ANIM_OFF); // Valor relativo
        lv_label_set_text_fmt(bar2_label, "%u", data / 2); // Actualizar etiqueta
    }

    delay(5);
}
