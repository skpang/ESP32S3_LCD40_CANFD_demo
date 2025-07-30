#pragma once
#include <Wire.h> 

//#define I2C_SCL_PIN       18
//#define I2C_SDA_PIN       8

#define I2C_SCL_PIN       44
#define I2C_SDA_PIN       43

void I2C_Init(void);

bool I2C_Read(uint8_t Driver_addr, uint8_t Reg_addr, uint8_t *Reg_data, uint32_t Length);
bool I2C_Write(uint8_t Driver_addr, uint8_t Reg_addr, const uint8_t *Reg_data, uint32_t Length);