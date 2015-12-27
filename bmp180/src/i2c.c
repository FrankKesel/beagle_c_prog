/*
 * i2c.c
 *
 *  Created on: Aug 29, 2014
 *      Author: kesel
 */

#include "i2c.h"

int i2cWrite(unsigned int i2cAddr, unsigned int nBytes, unsigned char *data) {
	int fd;

    //An i2c packet contains messages, either to be read or written
    //messages are a number of bytes
    struct i2c_rdwr_ioctl_data   packet;
    struct i2c_msg               messages[1]; //Only one message to write

    messages[0].addr  = i2cAddr; //The I2C address of the device
    messages[0].flags = 0;		  //Write transaction
    messages[0].len   = nBytes;   //number of bytes to transfer
    messages[0].buf   = data;	  //data buffer


	fd = open(I2C_DEVICE, O_RDWR);
	if (fd < 0) {
		printf("Error opening device.\n");
		return -1;
	}
	if (ioctl(fd, I2C_SLAVE, i2cAddr) < 0) {
		printf("ioctl error\n");
		return -1;
	}
    /* Transfer the i2c packets to the kernel and verify it worked */
    packet.msgs  = messages;
    packet.nmsgs = 1;
    if(ioctl(fd, I2C_RDWR, &packet) < 0) {
        printf("Unable to send data");
        return -1;
    }

    close(fd);
    return 0;
}

int i2cRead(unsigned int i2cAddr, unsigned int nBytes, unsigned char *data) {
	int fd;

    //An i2c packet contains messages, either to be read or written
    //messages are a number of bytes
    struct i2c_rdwr_ioctl_data   packet;
    struct i2c_msg               messages[1]; //Only one message to write

    messages[0].addr  = i2cAddr; //The I2C address of the device
    messages[0].flags = I2C_M_RD; //Read transaction
    messages[0].len   = nBytes;   //number of bytes to transfer
    messages[0].buf   = data;	  //data buffer


	fd = open(I2C_DEVICE, O_RDWR);
	if (fd < 0) {
		printf("Error opening device.\n");
		return -1;
	}
	if (ioctl(fd, I2C_SLAVE, i2cAddr) < 0) {
		printf("ioctl error\n");
		return -1;
	}
    /* Transfer the i2c packets to the kernel and verify it worked */
    packet.msgs  = messages;
    packet.nmsgs = 1;
    if(ioctl(fd, I2C_RDWR, &packet) < 0) {
        printf("Unable to send data");
        return -1;
    }

    close(fd);
    return 0;
}

int i2cWriteRead(unsigned int i2cAddr, unsigned int txBytes, unsigned char *txData, unsigned int rxBytes, unsigned char *rxData) {
	int fd;

    //An i2c packet contains messages, either to be read or written
    //messages are a number of bytes
    struct i2c_rdwr_ioctl_data   packet;
    struct i2c_msg               messages[2]; //2 messages, write then read

    messages[0].addr  = i2cAddr; //The I2C address of the device
    messages[0].flags = 0;       //Write transaction
    messages[0].len   = txBytes;  //number of bytes to transfer
    messages[0].buf   = txData;	  //data buffer

    messages[1].addr  = i2cAddr; //The I2C address of the device
    messages[1].flags = I2C_M_RD; //Read transaction with repeated start condition
    messages[1].len   = rxBytes;   //number of bytes to transfer
    messages[1].buf   = rxData;	  //data buffer


	fd = open(I2C_DEVICE, O_RDWR);
	if (fd < 0) {
		printf("Error opening device.\n");
		return -1;
	}
	if (ioctl(fd, I2C_SLAVE, i2cAddr) < 0) {
		printf("ioctl error\n");
		return -1;
	}
    /* Transfer the i2c packets to the kernel and verify it worked */
    packet.msgs  = messages;
    packet.nmsgs = 2;
    if(ioctl(fd, I2C_RDWR, &packet) < 0) {
        printf("Unable to send data");
        return -1;
    }

    close(fd);
    return 0;
}
