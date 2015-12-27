/**
 * @file bmp180.c
 * @author Frank Kesel
 * @date 12 Feb 2015
 * @version 1.0
 * @brief Driver functions for Bosch BMP180.
 * @details 
*/ 


#ifndef BMP180_H
#define BMP180_H


#include <stdio.h>       
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "i2c.h"

#define I2C_SLAVE_ADDR 0x77 // 7-Bit slave address of Bosch sensor
#define CHIP_ID 0x55  	    //Chip ID

//Define oversampling setting
#define OSS_ULTRA_LOW_POWER 0
#define OSS_STANDARD        1
#define OSS_HIGH_RES        2
#define OSS_ULTRA_HIGH_RES  3
#define OSS OSS_STANDARD   //Set the OSS setting here

//Define command codes 
#define   CMD_READ_TEMPERATURE                 0x2E
#define   CMD_READ_PRESSURE_ULTRA_LOW_POWER    0x34
#define   CMD_READ_PRESSURE_STANDARD           0x74
#define   CMD_READ_PRESSURE_HIGH_RES           0xB4
#define   CMD_READ_PRESSURE_ULTRA_HIGH_RES     0xF4

//#define DATASHEET   //Use calibration data from datasheet

/**
 * @brief The BMP180 registers
*/
 enum {
   BMP085_REGISTER_CAL_AC1            = 0xAA,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_AC2            = 0xAC,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_AC3            = 0xAE,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_AC4            = 0xB0,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_AC5            = 0xB2,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_AC6            = 0xB4,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_B1             = 0xB6,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_B2             = 0xB8,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_MB             = 0xBA,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_MC             = 0xBC,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CAL_MD             = 0xBE,  // R   Calibration data (16 bits)
   BMP085_REGISTER_CHIPID             = 0xD0,
   BMP085_REGISTER_VERSION            = 0xD1,
   BMP085_REGISTER_SOFTRESET          = 0xE0,
   BMP085_REGISTER_CONTROL            = 0xF4,
   BMP085_REGISTER_TEMPDATA           = 0xF6,
   BMP085_REGISTER_PRESSUREDATA       = 0xF6,
   BMP085_REGISTER_PRESSUREDATA_XLSB  = 0xF8,
 };

/**
 * @brief The structure for BMP180 calibration data
*/
 typedef struct {
   int16_t  ac1;
   int16_t  ac2;
   int16_t  ac3;
   uint16_t ac4;
   uint16_t ac5;
   uint16_t ac6;
   int16_t  b1;
   int16_t  b2;
   int16_t  mb;
   int16_t  mc;
   int16_t  md;
 } CALIB_DATA_T;


#ifdef __cplusplus
extern "C" {
#endif
   
/** 
 * @brief Read calibration coefficients from BMP180 
 * @return void
 * @details Read calibration coefficients from BMP180. 
*/
void bmp180ReadCoefficients(void) ;

/** 
 * @brief Initialize BMP180
 * @param i2cStr : Pointer to the management structure for the i2c.
 * @return void
 * @details Read calibration coefficients from BMP180. 
*/
void bmp180Initialize();
 
/** 
 * @brief Read uncompensated temperature from BMP180
 * @return Uncompensated temperature
 * @details Read uncompensated temperature from BMP180. 
*/
int32_t bmp180ReadRawTemperature(void);

int32_t bmp180ReadRawPressure(void);

/** 
 * @brief Calculate true temperature
 * @return True temperature
 * @details Calculate true temperature. 
*/
float bmp180ReadTemperature(void);

int32_t bmp180ReadPressure(void);

void writeCommand(uint8_t reg, uint8_t value);
void read8(uint8_t reg, uint8_t *value);
void read16(uint8_t reg, uint16_t *value);
void readS16(uint8_t reg, int16_t *value);
  
#ifdef __cplusplus
}
#endif   

#endif
