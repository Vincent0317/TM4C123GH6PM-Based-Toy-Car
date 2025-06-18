/*
 * accel_9250.h
 *
 *  Created on: 27 May 2024
 *      Author: frank
 */

#ifndef ACCEL_6050_H_
#define ACCEL_6050_H_
#include <stdint.h>
int init_accel_9250(void);
int16_t read_x_accel(void);
int16_t read_y_accel(void);
int16_t read_z_accel(void);
int16_t read_temperature(void);
#endif /* ACCEL_6050_H_ */
