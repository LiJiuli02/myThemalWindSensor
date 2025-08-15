#ifndef __MAGSENSOR_H_
#define __MAGSENSOR_H_

#include "driver/i2c.h"
#include "esp_log.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>

/* 定义 */
#define QMC5883P_ADDR 0x2C  // I2C address of QMC5883P
#define I2C_MASTER_SCL_IO GPIO_NUM_6  // GPIO for SCL
#define I2C_MASTER_SDA_IO GPIO_NUM_7  // GPIO for SDA
#define I2C_MASTER_NUM_MAG I2C_NUM_1
#define I2C_MASTER_FREQ_HZ 100000
#define CONTROL_1_REG_ADDR 0x0A
#define CONTROL_2_REG_ADDR 0x0B
#define STATUS_REG_ADDR 0x09
#define magnetic_declination -2
//模式选择
typedef enum {
    SUSPEND_MODE  = 0x00, // 0b xxxx xx00
    NORMAL_MODE   = 0x01, 
    SINGLE_MODE   = 0x02, 
    CONTIOUS_MODE = 0x03,
} qmc5883p_mode_t;
//模式选择
typedef enum {
    OUTPUT_RATE_10Hz  = 0x00, // 0b xxxx 00xx
    OUTPUT_RATE_50Hz  = 0x04, // 0b xxxx 01xx
    OUTPUT_RATE_100Hz = 0x08, // 0b xxxx 10xx
    OUTPUT_RATE_200Hz = 0x0C, // 0b xxxx 11xx
} qmc5883p_ODR_t;
//过采样倍率
typedef enum {
    OVER_SAMPLE_RATE_8 = 0x00, // 0b xx00 xxxx
    OVER_SAMPLE_RATE_4 = 0x10, // 0b xx01 xxxx
    OVER_SAMPLE_RATE_2 = 0x20, // 0b xx10 xxxx
    OVER_SAMPLE_RATE_1 = 0x30, // 0b xx11 xxxx
} qmc5883p_OSR1_t;
//降采样倍率
typedef enum {
    DOWN_SAMPLE_RATE_1 = 0x00, // 0b 00xx xxxx
    DOWN_SAMPLE_RATE_2 = 0x40, // 0b 01xx xxxx
    DOWN_SAMPLE_RATE_4 = 0x80, // 0b 10xx xxxx
    DOWN_SAMPLE_RATE_8 = 0xC0, // 0b 11xx xxxx
} qmc5883p_OSR2_t;
//降采样倍率
typedef enum {
    RNG_30_GUASS= 0x00, // 0b xxxx 00xx
    RNG_12_GUASS= 0x04, // 0b xxxx 01xx
    RNG_8_GUASS= 0x08, // 0b xxxx 10xx
    RNG_2_GUASS= 0x0C, // 0b xxxx 11xx
} qmc5883p_RNG_t;

/* 声明 */
void qmc5883p_init(); 

void qmc5883p_read_raw(int16_t *x, int16_t *y, int16_t *z); 
void qmc5883p_get_heading_deg(float *heading_deg) ;

#endif