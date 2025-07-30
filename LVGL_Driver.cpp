/*****************************************************************************
  | File        :   LVGL_Driver.c
  
  | help        : 
    The provided LVGL library file must be installed first
******************************************************************************/
#include <Arduino.h>
#include "TouchDrvGT911.hpp"
#include <lvgl.h>
#include "ui.h"
#include "ui_helpers.h"
#include "LVGL_Driver.h"
extern uint32_t frame_count;
lv_display_t * disp;

extern TouchLocation touchLocations[5];


/*Set to your screen resolution*/
#define ESP_PANEL_LCD_WIDTH   480
#define ESP_PANEL_LCD_HEIGHT  480

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (ESP_PANEL_LCD_WIDTH * ESP_PANEL_LCD_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

void* buf1 = NULL;
void* buf2 = NULL;
// static lv_color_t buf1[ LVGL_BUF_LEN ];
// static lv_color_t buf2[ LVGL_BUF_LEN ];
// static lv_color_t* buf1 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN, MALLOC_CAP_SPIRAM);
// static lv_color_t* buf2 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN, MALLOC_CAP_SPIRAM);
/* Serial debugging */
void Lvgl_print(const char * buf)
{
    // Serial.printf(buf);
    // Serial.flush();
}

/*  Display flushing 
    Displays LVGL content on the LCD
    This function implements associating LVGL data to the LCD screen
*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
//void Lvgl_Display_LCD( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p )
{
  //uint16_t width = (area->x2 - area->x1 +1);
  //uint16_t height = (area->y2 - area->y1+1);
  // LCD_addWindow(area->x1, area->y1, area->x2, area->y2, ( uint8_t *)&color_p->full);
  LCD_addWindow(area->x1, area->y1, area->x2,area->y2,( uint8_t *)color_p);
  lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev_drv, lv_indev_data_t * data )
//void Lvgl_Touchpad_Read( lv_indev_drv_t * indev_drv, lv_indev_data_t * data )
{
  uint8_t count = readGT911TouchLocation( touchLocations, 5);
  if (count > 0)
  {
    Serial.printf("Count %x x:%d y:%d\n",count,touchLocations[0].x,touchLocations[0].y);
    data->point.x = touchLocations[0].x;
    data->point.y = touchLocations[0].y;
    data->state = LV_INDEV_STATE_PR;
  }else {
    data->state = LV_INDEV_STATE_REL;
  }
   /*
  if (touch_data.points != 0x00) {
    data->point.x = touch_data.x;
    data->point.y = touch_data.y;
    data->state = LV_INDEV_STATE_PR;
  //  printf("LVGL : X=%u Y=%u points=%d\r\n",  touch_data.x , touch_data.y,touch_data.points);
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
  if (touch_data.gesture != NONE ) {    
  }
  */

}


void example_increase_lvgl_tick(void *arg)
{
  //static uint32_t count=0;
  //char buff[100];
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
    //sprintf(buff,  "%d", count++);
    //lv_label_set_text(ui_framecount,buff);
 
}
void Lvgl_Init(void)
{
  lv_init();
  lv_tick_set_cb(xTaskGetTickCount);
  esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, &buf1, &buf2);           

  disp = lv_display_create(ESP_PANEL_LCD_WIDTH, ESP_PANEL_LCD_HEIGHT);   
  lv_display_set_flush_cb(disp, my_disp_flush);                            
  //lv_disp_draw_buf_init( &draw_buf, buf1, buf2, ESP_PANEL_LCD_WIDTH * ESP_PANEL_LCD_HEIGHT);           
  lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  /*Initialize the (dummy) input device driver*/
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(indev, my_touchpad_read);


  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);

  /*Create a white label, set its text and align it to the center*/
  lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello world skp");
  lv_obj_set_style_text_font(label, &lv_font_montserrat_36, LV_PART_MAIN| LV_STATE_DEFAULT);

  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0x0), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

/*
  const esp_timer_create_args_t lvgl_tick_timer_args = {
    .callback = &example_increase_lvgl_tick,
    .name = "lvgl_tick"
  };
  esp_timer_handle_t lvgl_tick_timer = NULL;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);
*/
  // auto_step_timer = lv_timer_create(example1_increase_lvgl_tick, 100, NULL);

}
void Lvgl_Loop(void)
{
    
  char buff[100];
    /* Tell LVGL how many milliseconds has elapsed */
   
  sprintf(buff,  "%d", frame_count);
  lv_label_set_text(ui_framecount,buff);
  
  lv_timer_handler(); /* let the GUI do its work */
  vTaskDelay(pdMS_TO_TICKS(5));
}
