
#include "Display_ST7701.h"  
#include "lcd_util.h"
#include "Touch_GT911.h"
#include "I2C_Driver.h"

spi_device_handle_t SPI_handle = NULL;     
esp_lcd_panel_handle_t panel_handle = NULL;  
void scan_i2c_device(TwoWire &i2c)
{
  Serial.println("Scanning for I2C devices ...");
  Serial.print("      ");
  for (int i = 0; i < 0x10; i++)
  {
    Serial.printf("0x%02X|", i);
  }
  uint8_t error;
  for (int j = 0; j < 0x80; j += 0x10)
  {
    Serial.println();
    Serial.printf("0x%02X |", j);
    for (int i = 0; i < 0x10; i++)
    {
      Wire.beginTransmission(i | j);
      error = Wire.endTransmission();
      if (error == 0)
        Serial.printf("0x%02X|", i | j);
      else
        Serial.print(" -- |");
    }
  }
  Serial.println();
}

inline void ST7701_WriteCommand(uint8_t c)
{
  bool last_databit = 0;

  Set_EXIO(LCD_MOSI_PIN,last_databit);
  Set_EXIO(LCD_CLK_PIN,0);
  Set_EXIO(LCD_CLK_PIN,1);

  uint8_t bit = 0x80;
  while (bit)
  {
    if (c & bit)
    {
      if (last_databit != 1)
      {
        last_databit = 1;
        Set_EXIO(LCD_MOSI_PIN,last_databit);
      }
    }
    else
    {
      if (last_databit != 0)
      {
        last_databit = 0;
        Set_EXIO(LCD_MOSI_PIN,last_databit);
      }
    }
    Set_EXIO(LCD_CLK_PIN,0);
    bit >>= 1;
    Set_EXIO(LCD_CLK_PIN,1);
  }
}

inline void ST7701_WriteData(uint8_t d)
{
  bool last_databit = 1;

  Set_EXIO(LCD_MOSI_PIN,last_databit);
  Set_EXIO(LCD_CLK_PIN,0);
  Set_EXIO(LCD_CLK_PIN,1);
  
  uint8_t bit = 0x80;
  while (bit)
  {
    if (d & bit)
    {
      if (last_databit != 1)
      {
        last_databit = 1;
        Set_EXIO(LCD_MOSI_PIN,last_databit);
      }
    }
    else
    {
      if (last_databit != 0)
      {
        last_databit = 0;
        Set_EXIO(LCD_MOSI_PIN,last_databit);
      }
    }
    Set_EXIO(LCD_CLK_PIN,0);
    bit >>= 1;
    Set_EXIO(LCD_CLK_PIN,1);
  }
}

inline void digitalWrite_EXIO(uint8_t pin, uint8_t val)
{
    Set_EXIO(pin,val);
}

void ST7701_CS_EN(){
  digitalWrite_EXIO(LCD_CS,LOW);//Set_EXIO(EXIO_PIN3,Low);
}

void ST7701_CS_Dis(){
  digitalWrite_EXIO(LCD_CS,HIGH); //Set_EXIO(EXIO_PIN3,High);
}

void ST7701_Init()
{
	ST7701_CS_EN();
	ST7701_WriteCommand(0xFF);
	ST7701_WriteData(0x77);
	ST7701_WriteData(0x01);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x13);
	ST7701_WriteCommand(0xEF);
	ST7701_WriteData(0x08);
	ST7701_WriteCommand(0xFF);
	ST7701_WriteData(0x77);
	ST7701_WriteData(0x01);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x10);
	ST7701_WriteCommand(0xC0);
	ST7701_WriteData(0x3B);
	ST7701_WriteData(0x00);
	ST7701_WriteCommand(0xC1);
	ST7701_WriteData(0x0D);
	ST7701_WriteData(0x02);
	ST7701_WriteCommand(0xC2);
	ST7701_WriteData(0x21);
	ST7701_WriteData(0x08);
	ST7701_WriteCommand(0xCD);
	ST7701_WriteData(0x08);//18-bit/pixel: MDT=0:D[21:16]=R,D[13:8]=G,D[5:0]=B(CDH=00) ;
	                 //MDT=1:D[17:12]=R,D[11:6]=G,D[5:0]=B(CDH=08) ;
	ST7701_WriteCommand(0xB0);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x11);
	ST7701_WriteData(0x18);
	ST7701_WriteData(0x0E);
	ST7701_WriteData(0x11);
	ST7701_WriteData(0x06);
	ST7701_WriteData(0x07);
	ST7701_WriteData(0x08);
	ST7701_WriteData(0x07);
	ST7701_WriteData(0x22);
	ST7701_WriteData(0x04);
	ST7701_WriteData(0x12);
	ST7701_WriteData(0x0F);
	ST7701_WriteData(0xAA);
	ST7701_WriteData(0x31);
	ST7701_WriteData(0x18);
	ST7701_WriteCommand(0xB1);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x11);
	ST7701_WriteData(0x19);
	ST7701_WriteData(0x0E);
	ST7701_WriteData(0x12);
	ST7701_WriteData(0x07);
	ST7701_WriteData(0x08);
	ST7701_WriteData(0x08);
	ST7701_WriteData(0x08);
	ST7701_WriteData(0x22);
	ST7701_WriteData(0x04);
	ST7701_WriteData(0x11);
	ST7701_WriteData(0x11);
	ST7701_WriteData(0xA9);
	ST7701_WriteData(0x32);
	ST7701_WriteData(0x18);
	ST7701_WriteCommand(0xFF);
	ST7701_WriteData(0x77);
	ST7701_WriteData(0x01);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x11);
	ST7701_WriteCommand(0xB0);
	ST7701_WriteData(0x60);
	ST7701_WriteCommand(0xB1);
	ST7701_WriteData(0x30);
	ST7701_WriteCommand(0xB2);
	ST7701_WriteData(0x87);
	ST7701_WriteCommand(0xB3);
	ST7701_WriteData(0x80);
	ST7701_WriteCommand(0xB5);
	ST7701_WriteData(0x49);
	ST7701_WriteCommand(0xB7);
	ST7701_WriteData(0x85);
	ST7701_WriteCommand(0xB8);
	ST7701_WriteData(0x21);
	ST7701_WriteCommand(0xC1);
	ST7701_WriteData(0x78);
	ST7701_WriteCommand(0xC2);
	ST7701_WriteData(0x78);
	//delay(20);
  delay(5);
	ST7701_WriteCommand(0xE0);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x1B);
	ST7701_WriteData(0x02);
	ST7701_WriteCommand(0xE1);
	ST7701_WriteData(0x08);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x07);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x44);
	ST7701_WriteData(0x44);
	ST7701_WriteCommand(0xE2);
	ST7701_WriteData(0x11);
	ST7701_WriteData(0x11);
	ST7701_WriteData(0x44);
	ST7701_WriteData(0x44);
	ST7701_WriteData(0xED);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0xEC);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteCommand(0xE3);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x11);
	ST7701_WriteData(0x11);
	ST7701_WriteCommand(0xE4);
	ST7701_WriteData(0x44);
	ST7701_WriteData(0x44);
	ST7701_WriteCommand(0xE5);
	ST7701_WriteData(0x0A);
	ST7701_WriteData(0xE9);
	ST7701_WriteData(0xD8);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x0C);
	ST7701_WriteData(0xEB);
	ST7701_WriteData(0xD8);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x0E);
	ST7701_WriteData(0xED);
	ST7701_WriteData(0xD8);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x10);
	ST7701_WriteData(0xEF);
	ST7701_WriteData(0xD8);
	ST7701_WriteData(0xA0);
	ST7701_WriteCommand(0xE6);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x11);
	ST7701_WriteData(0x11);
	ST7701_WriteCommand(0xE7);
	ST7701_WriteData(0x44);
	ST7701_WriteData(0x44);
	ST7701_WriteCommand(0xE8);
	ST7701_WriteData(0x09);
	ST7701_WriteData(0xE8);
	ST7701_WriteData(0xD8);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x0B);
	ST7701_WriteData(0xEA);
	ST7701_WriteData(0xD8);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x0D);
	ST7701_WriteData(0xEC);
	ST7701_WriteData(0xD8);
	ST7701_WriteData(0xA0);
	ST7701_WriteData(0x0F);
	ST7701_WriteData(0xEE);
	ST7701_WriteData(0xD8);
	ST7701_WriteData(0xA0);
	ST7701_WriteCommand(0xEB);
	ST7701_WriteData(0x02);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0xE4);
	ST7701_WriteData(0xE4);
	ST7701_WriteData(0x88);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x40);
	ST7701_WriteCommand(0xEC);
	ST7701_WriteData(0x3C);
	ST7701_WriteData(0x00);
	ST7701_WriteCommand(0xED);
	ST7701_WriteData(0xAB);
	ST7701_WriteData(0x89);
	ST7701_WriteData(0x76);
	ST7701_WriteData(0x54);
	ST7701_WriteData(0x02);
	ST7701_WriteData(0xFF);
	ST7701_WriteData(0xFF);
	ST7701_WriteData(0xFF);
	ST7701_WriteData(0xFF);
	ST7701_WriteData(0xFF);
	ST7701_WriteData(0xFF);
	ST7701_WriteData(0x20);
	ST7701_WriteData(0x45);
	ST7701_WriteData(0x67);
	ST7701_WriteData(0x98);
	ST7701_WriteData(0xBA);
	ST7701_WriteCommand(0xEF);
	ST7701_WriteData(0x10);
	ST7701_WriteData(0x0D);
	ST7701_WriteData(0x04);
	ST7701_WriteData(0x08);
	ST7701_WriteData(0x3F);
	ST7701_WriteData(0x1F);
	ST7701_WriteCommand(0xFF);
	ST7701_WriteData(0x77);
	ST7701_WriteData(0x01);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteCommand(0x3A);
	ST7701_WriteData(0x66);//55/50=16bit(RGB565);66=18bit(RGB666);77或默认不写3AH是=24bit(RGB888)
		
	ST7701_WriteCommand(0xFF);
	ST7701_WriteData(0x77);
	ST7701_WriteData(0x01);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x13);
	ST7701_WriteCommand(0xE8);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x0E);
	ST7701_WriteCommand(0xFF);
	ST7701_WriteData(0x77);
	ST7701_WriteData(0x01);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteCommand(0x11);
	//vTaskDelay(pdMS_TO_TICKS(20));
  vTaskDelay(pdMS_TO_TICKS(5));
	ST7701_WriteCommand(0xFF);
	ST7701_WriteData(0x77);
	ST7701_WriteData(0x01);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x13);
	ST7701_WriteCommand(0xE8);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x0C);
	//vTaskDelay(pdMS_TO_TICKS(10));
  vTaskDelay(pdMS_TO_TICKS(5));
	ST7701_WriteCommand(0xE8);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteCommand(0xFF);
	ST7701_WriteData(0x77);
	ST7701_WriteData(0x01);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	ST7701_WriteData(0x00);
	
	ST7701_WriteCommand(0x36);
	ST7701_WriteData(0x00);
	
	ST7701_WriteCommand(0x3A);
	ST7701_WriteData(0x66);
	
	ST7701_WriteCommand(0x21);
	//vTaskDelay(pdMS_TO_TICKS(480));
//	ST7701_WriteCommand(0x2A);
	ST7701_WriteCommand(0x29);
	//vTaskDelay(pdMS_TO_TICKS(20));
  vTaskDelay(pdMS_TO_TICKS(5));
  ST7701_CS_Dis();

  //  RGB
  esp_lcd_rgb_panel_config_t rgb_config = {
    .clk_src = LCD_CLK_SRC_DEFAULT,
    .timings =  {
      .pclk_hz = ESP_PANEL_LCD_RGB_TIMING_FREQ_HZ,
      .h_res = ESP_PANEL_LCD_HEIGHT,
      .v_res = ESP_PANEL_LCD_WIDTH,
      .hsync_pulse_width = ESP_PANEL_LCD_RGB_TIMING_HPW,
      .hsync_back_porch = ESP_PANEL_LCD_RGB_TIMING_HBP,
      .hsync_front_porch = ESP_PANEL_LCD_RGB_TIMING_HFP,
      .vsync_pulse_width = ESP_PANEL_LCD_RGB_TIMING_VPW,
      .vsync_back_porch = ESP_PANEL_LCD_RGB_TIMING_VBP,
      .vsync_front_porch = ESP_PANEL_LCD_RGB_TIMING_VFP,
      .flags = {
        .hsync_idle_low = 0,  /*!< The hsync signal is low in IDLE state */
        .vsync_idle_low = 0,  /*!< The vsync signal is low in IDLE state */
        .de_idle_high = 0,    /*!< The de signal is high in IDLE state */
        .pclk_active_neg = false,
        .pclk_idle_high = 0,  /*!< The PCLK stays at high level in IDLE phase */
      },
    },
    .data_width = ESP_PANEL_LCD_RGB_DATA_WIDTH,
    .bits_per_pixel = ESP_PANEL_LCD_RGB_PIXEL_BITS,
    .num_fbs = ESP_PANEL_LCD_RGB_FRAME_BUF_NUM,
    .bounce_buffer_size_px = 10 * ESP_PANEL_LCD_HEIGHT,
    .psram_trans_align = 64,
    .hsync_gpio_num = ESP_PANEL_LCD_PIN_NUM_RGB_HSYNC,
    .vsync_gpio_num = ESP_PANEL_LCD_PIN_NUM_RGB_VSYNC,
    .de_gpio_num = ESP_PANEL_LCD_PIN_NUM_RGB_DE,
    .pclk_gpio_num = ESP_PANEL_LCD_PIN_NUM_RGB_PCLK,
    .disp_gpio_num = ESP_PANEL_LCD_PIN_NUM_RGB_DISP,
    .data_gpio_nums = {
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA0,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA1,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA2,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA3,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA4,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA5,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA6,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA7,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA8,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA9,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA10,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA11,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA12,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA13,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA14,
      ESP_PANEL_LCD_PIN_NUM_RGB_DATA15,
    },
    .flags = {
      .disp_active_low = 0,
      .refresh_on_demand = 0,
      .fb_in_psram = true,
      .double_fb = true,
      .no_fb = 0,
      .bb_invalidate_cache = 0,
    },
  };
  esp_lcd_new_rgb_panel(&rgb_config, &panel_handle); 
  // esp_lcd_rgb_panel_event_callbacks_t cbs = {
  //   .on_vsync = example_on_vsync_event,
  // };
  // esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &cbs, &disp_drv);
  esp_lcd_panel_reset(panel_handle);
  esp_lcd_panel_init(panel_handle);
}

bool example_on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *event_data, void *user_data)
{
  BaseType_t high_task_awoken = pdFALSE;
  return high_task_awoken == pdTRUE;
}
void LCD_Init() {

  TCA9554PWR_Init(0x00); // All output
  Set_EXIO(TP_RESET,LOW);
  delay(20);
  Set_EXIO(TP_INT,LOW);
  delay(50);
  Set_EXIO(TP_RESET,HIGH);
  delay(100);
  Mode_EXIO(TP_INT,1);
  //delay(100);
  scan_i2c_device(Wire);

  Serial.println("LCD init");
  unsigned long StartTime = millis();
  // ST7701_Reset();
  ST7701_Init();
  Serial.printf("ST7701 init time :%d\n",millis() - StartTime);
  GT911_init();
  
  Backlight_Init();
 
}

void LCD_addWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,uint8_t* color) {
  Xend = Xend + 1;      // esp_lcd_panel_draw_bitmap: x_end End index on x-axis (x_end not included)
  Yend = Yend + 1;      // esp_lcd_panel_draw_bitmap: y_end End index on y-axis (y_end not included)
  if (Xend >= ESP_PANEL_LCD_WIDTH)
    Xend = ESP_PANEL_LCD_WIDTH;
  if (Yend >= ESP_PANEL_LCD_HEIGHT)
    Yend = ESP_PANEL_LCD_HEIGHT;
   
  esp_lcd_panel_draw_bitmap(panel_handle, Xstart, Ystart, Xend, Yend, color);                     // x_end End index on x-axis (x_end not included)
}


// backlight
uint8_t LCD_Backlight = 100;
void Backlight_Init()
{
  ledcAttach(LCD_Backlight_PIN, Frequency, Resolution);    
  Set_Backlight_480(LCD_Backlight);      //0~100               
}
/*
void Set_Backlight(uint8_t Light)                        //
{
  if(Light > Backlight_MAX || Light < 0)
    printf("Set Backlight parameters in the range of 0 to 100 \r\n");
  else{
    uint32_t Backlight = Light*10;
    if(Backlight == 1000)
      Backlight = 1024;
    ledcWrite(LCD_Backlight_PIN, Backlight);
  }
}
*/

