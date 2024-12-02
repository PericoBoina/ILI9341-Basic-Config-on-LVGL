# ST7789 + ESP32 + Platformio + LVGL

This example use a 240x320 screen with an ESP32, using LVGL 9.2 and TFT_eSPI as the graphic driver.

## Configuration

Initialize platformio to download the TFT_eSPI and LVGL library.

Modify `include/User_Setup.h` lines 213-218, and the **move** the `User_Setup.h` to `.pio/libdeps/TFT_eSPI/`

Compile and flash.

## Troubleshooting

### Display noisy

If your display looks like the picture, you should lower your SPI speed in the `User_Setup.h` line 364, and uncomment a lower speed.

![](doc/IMG20241202194453 (1).jpg)
