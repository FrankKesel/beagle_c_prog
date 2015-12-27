/**
 * @file main.c
 * @author Frank Kesel
 * @date 15 Dec 2015
 * @version 1.0
 * @brief Read temperature and pressure from BMP180 sensor
 * @details Print to stdout the values, such that a
 * 		Python script can extract the values.
*/
#include "i2c.h"
#include "bmp180.h"

#define I2C_ADDR 0x77

int main()
{

	float temperature;
	int32_t pressurePascal;
	float pressureHPa;

    bmp180Initialize();

    temperature = bmp180ReadTemperature();
    pressurePascal = bmp180ReadPressure();
    pressureHPa = (float)(pressurePascal)/100;
	printf("%.2f %.2f ", temperature, pressureHPa);

    return 0;
}
