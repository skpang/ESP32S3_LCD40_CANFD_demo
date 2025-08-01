#pragma once
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_rgb.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/task.h"

#include "TCA9554PWR.h"
#include "LVGL_Driver.h"
#include "Touch_GT911.h"

#define LCD_BL        17
#define LCD_RESET     EXIO_PIN4
#define LCD_CS        EXIO_PIN7
#define LCD_CLK_PIN   EXIO_PIN6
#define LCD_MOSI_PIN  EXIO_PIN5 
#define LCD_Backlight_PIN   17 
// Backlight   
#define PWM_Channel     1       // PWM Channel   
#define Frequency       20000   // PWM frequencyconst         
#define Resolution      10       // PWM resolution ratio     MAX:13
#define Dutyfactor      500     // PWM Dutyfactor      
#define Backlight_MAX   100      

/*
#define ESP_PANEL_LCD_WIDTH                       (480)
#define ESP_PANEL_LCD_HEIGHT                      (480)
#define ESP_PANEL_LCD_COLOR_BITS                  (16)
#define ESP_PANEL_LCD_RGB_PIXEL_BITS              (16)    // 24 | 16
#define ESP_PANEL_LCD_RGB_DATA_WIDTH              (16)
#define ESP_PANEL_LCD_RGB_TIMING_FREQ_HZ          (16 * 1000 * 1000)
#define ESP_PANEL_LCD_RGB_TIMING_HPW              (8) // hsync_pulse_width 
#define ESP_PANEL_LCD_RGB_TIMING_HBP              (10)// hsync_back_porch 
#define ESP_PANEL_LCD_RGB_TIMING_HFP              (50) // hsync_front_porch 
#define ESP_PANEL_LCD_RGB_TIMING_VPW              (3) // vsync_polarity 
#define ESP_PANEL_LCD_RGB_TIMING_VBP              (8)// vsync_back_porch 
#define ESP_PANEL_LCD_RGB_TIMING_VFP              (8)// vsync_front_porch
#define ESP_PANEL_LCD_RGB_FRAME_BUF_NUM           (2)     // 1/2/3
*/
#define ESP_PANEL_LCD_WIDTH                       (480)
#define ESP_PANEL_LCD_HEIGHT                      (480)
#define ESP_PANEL_LCD_COLOR_BITS                  (16)
#define ESP_PANEL_LCD_RGB_PIXEL_BITS              (16)    // 24 | 16
#define ESP_PANEL_LCD_RGB_DATA_WIDTH              (16)
#define ESP_PANEL_LCD_RGB_TIMING_FREQ_HZ          (16 * 1000 * 1000)
#define ESP_PANEL_LCD_RGB_TIMING_HPW              (20) // hsync_pulse_width 
#define ESP_PANEL_LCD_RGB_TIMING_HBP              (20)// hsync_back_porch 
#define ESP_PANEL_LCD_RGB_TIMING_HFP              (20) // hsync_front_porch 
#define ESP_PANEL_LCD_RGB_TIMING_VPW              (3) // vsync_polarity 
#define ESP_PANEL_LCD_RGB_TIMING_VBP              (20)// vsync_back_porch 
#define ESP_PANEL_LCD_RGB_TIMING_VFP              (12)// vsync_front_porch
#define ESP_PANEL_LCD_RGB_FRAME_BUF_NUM           (2)     // 1/2/3
#define ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE         (ESP_PANEL_LCD_WIDTH * 10)     // Bounce buffer size in bytes. This function is used to avoid screen drift.
                                                          // To enable the bounce buffer, set it to a non-zero value. Typically set to `ESP_PANEL_LCD_WIDTH * 10`
                                                          // The size of the Bounce Buffer must satisfy `width_of_lcd * height_of_lcd = size_of_buffer * N`,
                                                          // where N is an even number.


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ESP_PANEL_LCD_PIN_NUM_RGB_HSYNC           (41)
#define ESP_PANEL_LCD_PIN_NUM_RGB_VSYNC           (42)
#define ESP_PANEL_LCD_PIN_NUM_RGB_DE              (2)
#define ESP_PANEL_LCD_PIN_NUM_RGB_PCLK            (1)
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA0           (40) //B1
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA1           (39) //B2
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA2           (38) //B3
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA3           (0)  //B4
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA4           (45) //B5
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA5           (48) //G0
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA6           (47) //G1
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA7           (21) //G2
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA8           (14) //G3
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA9           (13) //G4
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA10          (12) //G5
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA11          (11) //R1
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA12          (10) //R2
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA13          (9)  //R3
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA14          (46) //R4
#define ESP_PANEL_LCD_PIN_NUM_RGB_DATA15          (3)  //R5
#define ESP_PANEL_LCD_PIN_NUM_RGB_DISP            (-1)

#define ESP_PANEL_LCD_BK_LIGHT_ON_LEVEL           (1)
#define ESP_PANEL_LCD_BK_LIGHT_OFF_LEVEL !ESP_PANEL_LCD_BK_LIGHT_ON_LEVEL

#define EXAMPLE_ENABLE_PRINT_LCD_FPS            (0)

extern uint8_t LCD_Backlight;
extern esp_lcd_panel_handle_t panel_handle;   
bool example_on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *event_data, void *user_data);
void ST7701_Init();

void LCD_Init();
void LCD_addWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,uint8_t* color);

// backlight
void Backlight_Init();
//void Set_Backlight(uint8_t Light);  