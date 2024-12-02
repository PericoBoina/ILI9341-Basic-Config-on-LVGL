#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

// Configuración del TFT
TFT_eSPI tft = TFT_eSPI();
#define TFT_ROTATION LV_DISPLAY_ROTATION_0
#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// Definición de objetos LVGL
lv_obj_t *bar1;
lv_obj_t *bar2;

static uint32_t my_tick(void) {
    return millis();  // Función para el manejo del tiempo
}

// Función para establecer el valor de la barra de temperatura
static void set_temp(void *bar, int32_t temp) {
    lv_bar_set_value((lv_obj_t *)bar, temp, LV_ANIM_ON);
}

// Inicialización de las barras
void init_bars() {
    // Crear la barra 1
    bar1 = lv_bar_create(lv_scr_act());
    lv_obj_remove_style_all(bar1);  // Eliminar estilos predeterminados
    lv_obj_set_size(bar1, 150, 20);
    lv_obj_align(bar1, LV_ALIGN_CENTER, 0, -115);
    lv_bar_set_value(bar1, 10, LV_ANIM_ON);

    // Crear la barra 2
    bar2 = lv_bar_create(lv_scr_act());
    lv_obj_remove_style_all(bar2);  // Eliminar estilos predeterminados
    lv_obj_set_size(bar2, 150, 20);
    lv_obj_align(bar2, LV_ALIGN_CENTER, 0, -85);
    lv_bar_set_value(bar2, 60, LV_ANIM_ON);
    
    // Aplicar fondo a las barras
    lv_obj_set_style_bg_color(bar1, lv_color_make(200, 200, 200), LV_PART_MAIN);  // Fondo gris
    lv_obj_set_style_bg_color(bar2, lv_color_make(200, 200, 200), LV_PART_MAIN);  // Fondo gris
    lv_obj_set_style_radius(bar1, 10, LV_PART_MAIN);  // Bordes redondeados
    lv_obj_set_style_radius(bar2, 10, LV_PART_MAIN);  // Bordes redondeados
}

void setup() {
    // Inicialización de LVGL
    lv_init();
    lv_tick_set_cb(my_tick);  // Configurar el callback del temporizador
    lv_display_t *disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));

    // Crear la etiqueta de bienvenida
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello @Perico_197 in LVGL");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -140);

    // Inicializar las barras
    init_bars();

    // Animación en la barra 2
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_temp);
    lv_anim_set_duration(&a, 1500);
    lv_anim_set_playback_duration(&a, 1500);
    lv_anim_set_var(&a, bar2);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

void loop() {
    lv_timer_handler();  // Procesa tareas de LVGL
    delay(5);  // Reduce la carga de la CPU
}
