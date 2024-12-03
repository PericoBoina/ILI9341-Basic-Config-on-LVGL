#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST -1
#define TFT_BL 21
#define TFT_BACKLIGHT_ON HIGH

#define TOUCH_CS 33
#define TOUCH_IRQ 36
#define TOUCH_DIN 32  // T_DIN
#define TOUCH_OUT 39  // T_OUT
#define TOUCH_CLK 25   // T_CLK

// Configuración SPI para el touchscreen
SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(TOUCH_CS, TOUCH_IRQ);

// Definición de las dimensiones de la pantalla
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

TFT_eSPI tft = TFT_eSPI();

#define DRAW_BUF_SIZE (SCREEN_HEIGHT * SCREEN_WIDTH / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// Variables para los datos táctiles
int x, y, z;

// Función para la actualización del temporizador de LVGL
static uint32_t my_tick(void)
{
    return millis();
}

// Variables para la interfaz de usuario
lv_obj_t *graph;
lv_chart_series_t *temp_series;
lv_obj_t *temp_label;
lv_obj_t *label;  
lv_obj_t *button; 
lv_obj_t *btn_label; 

#define GRAPH_HEIGHT 120
#define GRAPH_POINTS 25
int temperature_values[GRAPH_POINTS] = {0}; 

// Controlador de eventos para el botón
static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Button Clicked");
    }
}

// Función para manejar la entrada táctil
void touchscreen_read(lv_indev_t * indev, lv_indev_data_t * data) {
  if(touchscreen.tirqTouched() && touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 300, 1, SCREEN_HEIGHT);
    z = p.z;
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = x;
    data->point.y = y;
  }
  else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void setup()
{
    Serial.begin(115200);
    lv_init();
    lv_tick_set_cb(my_tick);
    lv_display_t *disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(0, 0, 0), 0);

    // Crear la etiqueta de título
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Temperatura en tiempo real");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), 0);

    // Crear el gráfico de temperatura
    graph = lv_chart_create(lv_scr_act());
    lv_obj_set_size(graph, SCREEN_WIDTH, GRAPH_HEIGHT);
    lv_obj_align(graph, LV_ALIGN_TOP_LEFT, 0, 40);
    lv_chart_set_type(graph, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(graph, GRAPH_POINTS);
    lv_chart_set_range(graph, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    temp_series = lv_chart_add_series(graph, lv_color_make(255, 50, 0), LV_CHART_AXIS_PRIMARY_Y);

    // Etiqueta de temperatura
    temp_label = lv_label_create(lv_scr_act());
    lv_label_set_text(temp_label, "TEMP: xx°C");
    lv_obj_set_pos(temp_label, 85, 170);
    lv_obj_set_style_text_color(temp_label, lv_color_make(255, 255, 255), 0);

    // Crear el botón
    button = lv_btn_create(lv_scr_act());
    lv_obj_set_size(button, 150, 50);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -40);
    lv_obj_add_event_cb(button, event_handler, LV_EVENT_CLICKED, NULL);

    // Etiqueta para el botón
    btn_label = lv_label_create(button);
    lv_label_set_text(btn_label, "Press Me!");
    lv_obj_center(btn_label);
}

void loop()
{
    lv_timer_handler();

    // Si hay datos disponibles desde el puerto serial
    if (Serial.available() > 0)
    {
        String received = Serial.readStringUntil('\n'); // Leer hasta nueva línea
        received.trim();

        int temperature = received.toInt(); // Convertir la cadena recibida a entero

        // Asegurar que la temperatura esté en el rango de 0 a 100
        if (temperature < 0) temperature = 0;
        if (temperature > 100) temperature = 100;

        // Desplazar los valores para actualizar el gráfico
        for (int i = 0; i < GRAPH_POINTS - 1; i++) {
            temperature_values[i] = temperature_values[i + 1];
        }
        temperature_values[GRAPH_POINTS - 1] = temperature;

        // Actualizar el gráfico con los nuevos valores de temperatura
        for (int i = 0; i < GRAPH_POINTS; i++) {
            lv_chart_set_next_value(graph, temp_series, temperature_values[i]);
        }

        // Actualizar la etiqueta de la temperatura
        lv_label_set_text_fmt(temp_label, "TEMP: %d°C", temperature);
    }

    delay(100);
}
