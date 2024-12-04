#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen touchscreen(TOUCH_CS, TOUCH_IRQ);

static uint32_t my_tick(void)
{
    return millis();
}

lv_obj_t *label_1;
lv_obj_t *button;

// Manejador del evento del botón
void button_event_handler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        Serial.println("Botón pulsado!");
    }
}

// Función para leer los datos táctiles y pasarlos a LVGL
void touchscreen_read(lv_indev_data_t *data)
{
    if (touchscreen.tirqTouched() && touchscreen.touched()) {
        TS_Point p = touchscreen.getPoint();  // Lee los datos táctiles

        // Mapea las coordenadas táctiles a las dimensiones de la pantalla
        int x = map(p.x, 200, 3700, 1, TFT_WIDTH);
        int y = map(p.y, 240, 3800, 1, TFT_HEIGHT);
        int z = p.z;

        data->state = LV_INDEV_STATE_PRESSED;  // Estado de presionado
        data->point.x = x;
        data->point.y = y;

        // Imprimir las coordenadas táctiles y la presión
        Serial.print("X = ");
        Serial.print(x);
        Serial.print(" | Y = ");
        Serial.print(y);
        Serial.print(" | Pressure = ");
        Serial.println(z);
    } else {
        data->state = LV_INDEV_STATE_RELEASED;  // Estado de liberado
    }
}

void setup()
{
    Serial.begin(115200);
    // Inicializa LVGL
    lv_init();
    lv_tick_set_cb(my_tick);

    // Configura el color de fondo de la pantalla
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(51, 47, 44), 0);

    // Crea el botón
    button = lv_btn_create(lv_scr_act());
    lv_obj_align(button, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_set_size(button, 180, 50);
    lv_obj_add_event_cb(button, button_event_handler, LV_EVENT_CLICKED, NULL);

    // Crea la etiqueta dentro del botón
    label_1 = lv_label_create(button);
    lv_label_set_text(label_1, "Pulsador 1");
    lv_obj_center(label_1);
    lv_obj_set_style_text_color(label_1, lv_color_make(0, 0, 0), 0);

}

void loop()
{
    lv_timer_handler();  // Llama al manejador de temporizadores LVGL
    delay(5);  // Retardo pequeño para evitar sobrecargar el procesador
}
