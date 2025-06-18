/*
 * accel_6050.c
 *
 *  Created on: 27 May 2024
 *      Author: frank
 */

#include <accel_6050.h>
#include <stdint.h>
#include "i2c.h"
int init_accel_6050(void)
{
    uint8_t who_am_i;
    // initI2C();
    // attempt to read the who_am_i byte to be sure the device is there
    i2c_read_register(0x68,117,&who_am_i);
    if (who_am_i == 0x68)
    {
        i2c_write_register(0x68,107,0); // wake mpu6050
        i2c_write_register(0x68,35,8); // enable FIFO
        i2c_write_register(0x68,29,6); // enable low pass filter

        return 0; // no error
    }
    else
        return -1; // device missing
}
int16_t read_x_accel(void)
{
    uint8_t accel_high,accel_low;
    int16_t accel;
    i2c_read_register(0x68,59,&accel_high);
    i2c_read_register(0x68,60,&accel_low);
    accel = accel_high;
    accel = accel << 8;
    accel = accel + accel_low;
    return accel;
}
int16_t read_y_accel(void)
{
    uint8_t accel_high,accel_low;
    int16_t accel;
    i2c_read_register(0x68,61,&accel_high);
    i2c_read_register(0x68,62,&accel_low);
    accel = accel_high;
    accel = accel << 8;
    accel = accel + accel_low;
    return accel;
}
int16_t read_z_accel(void)
{
    uint8_t accel_high,accel_low;
    int16_t accel;
    i2c_read_register(0x68,63,&accel_high);
    i2c_read_register(0x68,64,&accel_low);
    accel = accel_high;
    accel = accel << 8;
    accel = accel + accel_low;
    return accel;
}
int16_t read_temperature()
{
    uint8_t accel_high,accel_low;
    int16_t accel;
    i2c_read_register(0x68,63,&accel_high);
    i2c_read_register(0x68,64,&accel_low);
    accel = accel_high;
    accel = accel << 8;
    accel = accel + accel_low;
    accel = (accel/340)+37;
    return accel;
}
