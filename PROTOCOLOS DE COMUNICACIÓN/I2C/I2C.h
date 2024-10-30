#ifndef I2C_H
#define I2C_H
#include "MKL25Z4.h"

void I2C0_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte(uint8_t ack);
void I2C_WriteRegister(uint8_t slaveAdd, uint8_t regAdd, uint8_t data);
uint8_t I2C_ReadRegister(uint8_t slaveAdd, uint8_t regAdd);

#endif 
