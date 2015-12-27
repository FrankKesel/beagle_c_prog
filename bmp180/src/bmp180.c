/**
 * @file bmp180.c
 * @author Frank Kesel
 * @date 12 Feb 2015
 * @version 1.0
 * @brief Driver functions for Bosch BMP180.
 * @details 
*/ 

#include "bmp180.h"

//Calibration data 
CALIB_DATA_T coeffs = {.ac1 = 408, .ac2 = -72, .ac3 = -14383,\
   .ac4 = 32741, .ac5 = 32757, .ac6 = 23153, .b1  = 6190, .b2  = 4,\
   .mb  = -32768, .mc  = -8711, .md  = 2868};


void writeCommand(uint8_t reg, uint8_t value) {
   uint8_t dataBuffer[2];

   dataBuffer[0] = reg;
   dataBuffer[1] = value;
   i2cWrite(I2C_SLAVE_ADDR, 2, &dataBuffer[0]);
   
}

void read8(uint8_t reg, uint8_t *value) {
   uint8_t regBuf, dataBuf;

   regBuf = reg;
   i2cWriteRead(I2C_SLAVE_ADDR, 1, &regBuf, 1, &dataBuf);

   *value = dataBuf;
}

void read16(uint8_t reg, uint16_t *value) {
   uint8_t dataBuf[2], regBuf;
   uint16_t data;

   regBuf = reg;
   i2cWriteRead(I2C_SLAVE_ADDR, 1, &regBuf, 2, dataBuf);

   data = (uint16_t)(dataBuf[0]<<8);
   data = data + (uint16_t)dataBuf[1];
   *value = data;
}

void readS16(uint8_t reg, int16_t *value) {
  uint16_t data;

  read16(reg, &data);
  *value = (int16_t)data;
}

void bmp180Initialize(){
   uint8_t data;
   read8(BMP085_REGISTER_CHIPID, &data);
   if(data!=CHIP_ID){
	   printf("Error: Can not connect to BMP180, wrong ID.\n");
	   exit(EXIT_FAILURE);
   }
   bmp180ReadCoefficients();
}

void bmp180ReadCoefficients(void) {
#ifdef DATASHEET
   coeffs.ac1 = 408;
   coeffs.ac2 = -72;
   coeffs.ac3 = -14383;
   coeffs.ac4 = 32741;
   coeffs.ac5 = 32757;
   coeffs.ac6 = 23153;
   coeffs.b1 = 6190;
   coeffs.b2 = 4;
   coeffs.mb = -32768;
   coeffs.mc = -8711;
   coeffs.md = 2868;  
#else
   readS16(BMP085_REGISTER_CAL_AC1, &coeffs.ac1);
   readS16(BMP085_REGISTER_CAL_AC2, &coeffs.ac2);
   readS16(BMP085_REGISTER_CAL_AC3, &coeffs.ac3);
   read16(BMP085_REGISTER_CAL_AC4, &coeffs.ac4);
   read16(BMP085_REGISTER_CAL_AC5, &coeffs.ac5);
   read16(BMP085_REGISTER_CAL_AC6, &coeffs.ac6);
   readS16(BMP085_REGISTER_CAL_B1, &coeffs.b1);
   readS16(BMP085_REGISTER_CAL_B2, &coeffs.b2);
   readS16(BMP085_REGISTER_CAL_MB, &coeffs.mb);
   readS16(BMP085_REGISTER_CAL_MC, &coeffs.mc);
   readS16(BMP085_REGISTER_CAL_MD, &coeffs.md);
#endif
}



int32_t bmp180ReadRawTemperature(void){
    uint16_t temp;

   //Start temperature measurement
   writeCommand(BMP085_REGISTER_CONTROL, CMD_READ_TEMPERATURE);
   usleep(4500); //Wait 4500 us
   //Read uncompensated temperature
   read16(BMP085_REGISTER_TEMPDATA, &temp);
   return temp;
}

int32_t bmp180ReadRawPressure(void){
   uint16_t pressureUPPER;
   uint8_t pressureXLSB;
   int32_t pressure;
   uint8_t pressCmd;
   uint32_t time; 
   uint32_t ossSetting = OSS;

   switch (ossSetting) {
      case OSS_ULTRA_LOW_POWER:
         pressCmd = CMD_READ_PRESSURE_ULTRA_LOW_POWER; 
         time = 5000; //Wait time in us
         break;
      case OSS_STANDARD:
         pressCmd = CMD_READ_PRESSURE_STANDARD; 
         time = 7500; //Wait time in us
         break;
      case OSS_HIGH_RES:
         pressCmd = CMD_READ_PRESSURE_HIGH_RES; 
         time = 13500; //Wait time in us
         break;
      case OSS_ULTRA_HIGH_RES:
         pressCmd = CMD_READ_PRESSURE_ULTRA_HIGH_RES; 
         time = 25500; //Wait time in us
         break;
      default: break;
   }  
   //Start pressure measurement
   writeCommand(BMP085_REGISTER_CONTROL, pressCmd);
   usleep(time); //Wait for end of conversion, time in us
   //Read uncompensated pressure
   read16(BMP085_REGISTER_PRESSUREDATA, &pressureUPPER); //MSB and LSB
   read8(BMP085_REGISTER_PRESSUREDATA_XLSB, &pressureXLSB); //XLSB

   pressure = ((int32_t)pressureUPPER << 8) + (int32_t)pressureXLSB;
   pressure = pressure >> (8 - OSS);

   return pressure;
}

float bmp180ReadTemperature(void) {
   int32_t ut, x1, x2, b5;     
   float temp;

   //Calculation of true temperature according to datasheet 
   ut = bmp180ReadRawTemperature();
   x1 = ((ut - (int32_t)coeffs.ac6) * ((int32_t)coeffs.ac5)) >> 15;
   x2 = ((int32_t)coeffs.mc << 11) / (x1+(int32_t)coeffs.md);
   b5 = x1 + x2;
   temp = (float)((b5+8) >> 4);
   temp /= 10;
  
   return temp;
}

int32_t bmp180ReadPressure(void) {
   int32_t ut, x1, x2, b5;     
   int32_t up, b3, b6, x3, p;
   uint32_t b4, b7;

   ut = bmp180ReadRawTemperature();
   up = bmp180ReadRawPressure();
//   ut = 27898;  //Use value from datasheet to verify 
//   up = 23843;


   //Calculation of temperature b5
   x1 = ((ut - (int32_t)coeffs.ac6) * ((int32_t)coeffs.ac5)) >> 15;
   x2 = ((int32_t)coeffs.mc << 11) / (x1+(int32_t)coeffs.md);
   b5 = x1 + x2;

   // do pressure calcs
   b6 = b5 - 4000;
   x1 = ((int32_t)coeffs.b2 * ((b6 * b6) >> 12) ) >> 11;
   x2 = ((int32_t)coeffs.ac2 * b6) >> 11;
   x3 = x1 + x2;
   b3 = ((((int32_t)coeffs.ac1*4 + x3) << OSS) + 2) / 4;
   x1 = ((int32_t)coeffs.ac3 * b6) >> 13;
   x2 = ((int32_t)coeffs.b1 * ((b6 * b6) >> 12)) >> 16;
   x3 = ((x1 + x2) + 2) >> 2;
   b4 = ((uint32_t)coeffs.ac4 * (uint32_t)(x3 + 32768)) >> 15;
   b7 = ((uint32_t)up - b3) * (uint32_t)( 50000UL >> OSS);
   if (b7 < 0x80000000) {
      p = (b7 * 2) / b4;
   } 
   else {
      p = (b7 / b4) * 2;
   }
   x1 = (p >> 8) * (p >> 8);
   x1 = (x1 * 3038) >> 16;
   x2 = (-7357 * p) >> 16;
   p = p + ((x1 + x2 + (int32_t)3791)>>4);

   return p;
}
