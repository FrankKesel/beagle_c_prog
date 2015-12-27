/*
 * i2c.h
 *
 *  Created on: Aug 29, 2014
 *      Author: kesel
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define I2C_DEVICE "/dev/i2c-1"

int i2cWrite(unsigned int i2cAddr, unsigned int nBytes, unsigned char *data);
int i2cRead(unsigned int i2cAddr, unsigned int nBytes, unsigned char *data);
int i2cWriteRead(unsigned int i2cAddr, unsigned int txBytes, unsigned char *txData, unsigned int rxBytes, unsigned char *rxData);

#endif /* I2C_H_ */
