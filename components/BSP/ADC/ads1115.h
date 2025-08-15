#ifndef ADS1115_H
#define ADS1115_H

#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_NUM_ADC I2C_NUM_0
#define I2C_MASTER_SDA GPIO_NUM_12
#define I2C_MASTER_SCL GPIO_NUM_13
#define I2C_MASTER_FREQ_HZ 100000

#define ADS1115_ADDRESS 0x48  // 默认 I2C 地址
#define ADS1115_CONVERSION_REG 0x00
#define ADS1115_CONFIG_REG 0x01

// 通道设置
typedef enum {
    ADS1115_DIFFER_CHANNEL_0 = 0x00, // 差分通道0 AIN0-AIN1
    ADS1115_DIFFER_CHANNEL_1 = 0x01, // 差分通道0 AIN0-AIN3
    ADS1115_DIFFER_CHANNEL_2 = 0x02, // 差分通道0 AIN1-AIN3
    ADS1115_DIFFER_CHANNEL_3 = 0x03, // 差分通道3 AIN2-AIN3
    ADS1115_CHANNEL_0 = 0x04, // 单端通道 AIN0
    ADS1115_CHANNEL_1 = 0x05, // 单端通道 AIN1
    ADS1115_CHANNEL_2 = 0x06, // 单端通道 AIN2
    ADS1115_CHANNEL_3 = 0x07, // 单端通道 AIN3
} ads1115_channel_t;

// 增益设置
typedef enum {
    ADS1115_GAIN_6_144V = 0x00, // ±6.144V
    ADS1115_GAIN_4_096V = 0x01, // ±4.096V
    ADS1115_GAIN_2_048V = 0x02, // ±2.048V（默认）
    ADS1115_GAIN_1_024V = 0x03, // ±1.024V
    ADS1115_GAIN_0_512V = 0x04, // ±0.512V
    ADS1115_GAIN_0_256V = 0x05, // ±0.256V
} ads1115_gain_t;

// 采样率设置
typedef enum {
    ADS1115_RATE_8SPS = 0x00,
    ADS1115_RATE_16SPS = 0x01,
    ADS1115_RATE_32SPS = 0x02,
    ADS1115_RATE_64SPS = 0x03,
    ADS1115_RATE_128SPS = 0x04, // 默认
    ADS1115_RATE_250SPS = 0x05,
    ADS1115_RATE_475SPS = 0x06,
    ADS1115_RATE_860SPS = 0x07,
} ads1115_data_rate_t;

void ads1115_init();
void ads1115_set_config(uint8_t channel, ads1115_gain_t gain, ads1115_data_rate_t rate);
float ads1115_read_adc(uint8_t channel, uint8_t gain);
float ads1115_read_adc_avg(uint8_t channel, uint8_t gain, uint8_t avg_count);
float ads1115_read_adc_avg_remv_extreme(uint8_t channel, uint8_t gain, uint8_t avg_count);
#endif