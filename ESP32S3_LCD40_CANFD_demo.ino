/*
For use with this board:
https://www.skpang.co.uk/collections/esp32-boards/products/esp32s3-4-lcd-with-can-fd-and-can-bus

Ensure these libraries are installed:
https://github.com/pierremolinaro/acan2517FD
https://github.com/handmade0octopus/ESP32-TWAI-CAN


*/

#include <lvgl.h>
#include <ESP32-TWAI-CAN.hpp>
#include "esp_flash.h"
#include "canfd.h"
#include "LVGL_Driver.h"
#include "Display_ST7701.h"  
#include "ui.h"
#include "ui_helpers.h"

#define CAN_TX   15  // Connects to CTX
#define CAN_RX   16  // Connects to CRX
String frame_count_str;
String can2_data;
int led  = 18;
int bl = 17;
uint8_t brightness;
uint32_t frame_count = 0;
CanFrame rxFrame; 
uint8_t can_start = 0;

void canReceiver() {
  char buff[5];

  // try to parse packet
  if(ESP32Can.readFrame(rxFrame, 0)) { // 1000 is the timeout value
    // Communicate that a packet was recieved
    sprintf(buff,"%02X",rxFrame.identifier);
    can2_data = String("ID: ") +buff + String(" Len: ") +  rxFrame.data_length_code + " Data: ";
    Serial.printf("Classic CAN received ID: %03X Len:%d  Data: ", rxFrame.identifier,rxFrame.data_length_code);

    // Communicate packet information
    for(int i = 0; i <= rxFrame.data_length_code - 1; i ++) {
      Serial.printf("%02x ",rxFrame.data[i]); // Transmit value from the frame 
      sprintf(buff,"%02X",rxFrame.data[i]);
      can2_data += String(" ") + buff; 
    }
    Serial.println(" ");
    lv_label_set_text(ui_ClassicCANdata, can2_data.c_str());
  } 
}

void canSender() {
  static uint8_t i=0;
  digitalWrite(led, HIGH);

  CanFrame testFrame = { 0 };
  testFrame.identifier = 0x7df;  // Sets the ID
  testFrame.extd = 0; // Set extended frame to false
  testFrame.data_length_code = 8; // Set length of data - change depending on data sent
  testFrame.data[0] = i++; // Write data to buffer. data is not sent until writeFrame() is called.
  testFrame.data[1] = 0x12;
  testFrame.data[2] = 0x34;
  testFrame.data[3] = 0x56;
  testFrame.data[4] = 0x78;
  testFrame.data[5] = 0x9a;
  testFrame.data[6] = 0xbc;
  testFrame.data[7] = 0xde;

  ESP32Can.writeFrame(testFrame); // transmit frame
 
  frame_count++;
  digitalWrite(led, LOW);
}

void Driver_Loop(void *parameter)
{
  while(1)
  {
      if(can_start == 1)
      {
        canSender();  // call function to send data through CAN
        canfd_sendframe();
      }
      vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void CAN_init(void)
{
  ESP32Can.setPins(CAN_TX, CAN_RX);
  // Start the CAN bus at 500 kbps
  if(ESP32Can.begin(ESP32Can.convertSpeed(500))) {
      Serial.println("CAN bus started!");
  } else {
      Serial.println("CAN bus failed!");
  }
}

void Driver_Init()
{
  I2C_Init();
  xTaskCreatePinnedToCore(
    Driver_Loop,     
    "Other Driver task",   
    4096,                
    NULL,                 
    3,                    
    NULL,                
    0                    
  );
}

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(bl, OUTPUT);
  digitalWrite(bl, HIGH); 
  Serial.begin(115200);
  digitalWrite(led, HIGH);  
  delay(100);                  
  digitalWrite(led, LOW);   
  delay(100);
  digitalWrite(led, HIGH);  
  delay(100);
  Serial.printf("\n\n\n##################################################\n");
  Serial.flush();
  Serial.println("ESP32 S3 LCD 4 Starting....");

  brightness = 10;

  Driver_Init();
  LCD_Init();    

  Serial.println("ESP32S3 4.0in LCD with LVGL skpang.co.uk 05/2025");
  Serial.println((String)"Memory available in PSRAM (after LCD init): " +ESP.getFreePsram());

  uint32_t flash_size;
  esp_err_t ret = esp_flash_get_size(NULL, &flash_size);
  Serial.printf("Total flash size: %d bytes\n", flash_size);

  String LVGL_Arduino = "LVGL ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println( LVGL_Arduino );

  Lvgl_Init();
  ui_init();
  digitalWrite(led, LOW); 
  lv_label_set_text(ui_version,LVGL_Arduino.c_str());
  
  String bright = "Brightness : ";
  bright += brightness;
  lv_label_set_text(ui_brightness,bright.c_str());

  frame_count_str = String(frame_count);
  lv_label_set_text(ui_framecount,frame_count_str.c_str());

  CAN_init();
  canfd_init();
  can_start = 1;
}

void loop()
{
  Lvgl_Loop();
  vTaskDelay(pdMS_TO_TICKS(5));
  canReceiver();
  canfd_receiveframe();
}