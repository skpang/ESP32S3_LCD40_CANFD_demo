#pragma once
#include "Arduino.h"
#include "TCA9554PWR.h"

struct TouchLocation
{
  uint16_t x;
  uint16_t y;
};

//#define I2C_SCL_PIN       44
//#define I2C_SDA_PIN       43

#define TP_RESET          EXIO_PIN4
#define TP_INT            EXIO_PIN3



void GT911_init(void);
uint8_t readGT911TouchLocation( TouchLocation * pLoc, uint8_t num );