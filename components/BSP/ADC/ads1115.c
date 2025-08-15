#include "ads1115.h"

#define TAG "ADS1115"  // 日志 TAG

#define I2C_MASTER_TIMEOUT_MS 1000  // I2C 读写超时时间（1秒）

/**
 * @brief 通过 I2C 向 ADS1115 写入寄存器值
 * @param dev_addr  设备 I2C 地址（默认 0x48）
 * @param reg       目标寄存器地址
 * @param data      需要写入的 16 位数据
 * @return esp_err_t ESP_OK 表示成功，其他值表示失败
 */
static esp_err_t ads1115_master_write(uint8_t dev_addr, uint8_t reg, uint16_t data) 
{
    uint8_t write_buf[3] = {reg, (data >> 8) & 0xFF, data & 0xFF};
    return i2c_master_write_to_device(I2C_MASTER_NUM_ADC, dev_addr, write_buf, sizeof(write_buf), 
                                      I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

/**
 * @brief 通过 I2C 从 ADS1115 读取寄存器值
 * @param dev_addr  设备 I2C 地址（默认 0x48）
 * @param reg       目标寄存器地址
 * @param data      读取到的 16 位数据指针
 * @return esp_err_t ESP_OK 表示成功，其他值表示失败
 */
static esp_err_t ads1115_master_read(uint8_t dev_addr, uint8_t reg, uint16_t *data) 
{
    uint8_t read_buf[2];  // 存储读取到的数据
    esp_err_t ret = i2c_master_write_read_device(I2C_MASTER_NUM_ADC, dev_addr, &reg, 1, read_buf, 2, 
                                                 I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    if (ret == ESP_OK) 
    {
        *data = (read_buf[0] << 8) | read_buf[1];  // 高字节在前，低字节在后
    }
    return ret;
}

/**
 * @brief 初始化 I2C 设备
 */
void ads1115_init() 
{
    ESP_LOGI(TAG, "Initializing ADS1115...");

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,        // I2C 主模式
        .sda_io_num = I2C_MASTER_SDA,   // 指定 SDA 引脚
        .scl_io_num = I2C_MASTER_SCL,   // 指定 SCL 引脚
        .sda_pullup_en = GPIO_PULLUP_ENABLE, // 使能上拉
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ // 设置 I2C 频率
    };

    i2c_param_config(I2C_MASTER_NUM_ADC, &conf);
    i2c_driver_install(I2C_MASTER_NUM_ADC, conf.mode, 0, 0, 0); // 安装 I2C 驱动
}

/**
 * @brief 设置 ADS1115 配置（通道、增益、速率）
 * @param channel  选择单端输入通道（0-3）或差分输入通道（0-1）
 * @param gain     选择 PGA 增益（影响满量程范围）
 * @param rate     选择 ADC 采样速率（SPS）
 */
void ads1115_set_config(uint8_t channel, ads1115_gain_t gain, ads1115_data_rate_t rate) 
{
    uint16_t config = 0x8100 | (channel << 12) | (gain << 9) | (rate << 5) | 0x03;  // 组装配置寄存器数据 单次转换模式
    ads1115_master_write(ADS1115_ADDRESS, ADS1115_CONFIG_REG, config);
}

/**
 * @brief 读取 ADS1115 单端通道或差分通道 ADC 值
 * @param channel  选择单端输入通道（0-3）或差分输入通道（0-3）
 * @return float 返回 ADC 采样值 或 电压值mV
 */
float ads1115_read_adc(uint8_t channel, uint8_t gain) 
{
    float ret = 0;
    ads1115_set_config(channel, gain , ADS1115_RATE_128SPS);  // 设置默认增益和速率
    vTaskDelay(pdMS_TO_TICKS(10));  // 等待转换完成（最小 8ms）

    uint16_t value;  // 按无符号数读取，但读取的是补码，需要转换为有符号数
    if (ads1115_master_read(ADS1115_ADDRESS, ADS1115_CONVERSION_REG, &value) == ESP_OK) 
    {
        float scale_factor = 0.0625; // 默认 ADS1115_GAIN_2_048V

        switch (gain)
        {
            case ADS1115_GAIN_0_512V:
                scale_factor = 0.015625;
                break;
            case ADS1115_GAIN_1_024V:
                scale_factor = 0.03125;
                break;
            case ADS1115_GAIN_2_048V:
                scale_factor = 0.0625;
                break;
            case ADS1115_GAIN_4_096V:
                scale_factor = 0.125;
                break;
            case ADS1115_GAIN_6_144V:
                scale_factor = 0.1875;
                break;
            default:
                ESP_LOGW(TAG, "Invalid gain value, using default 2.048V");
                break;
        }
        
        ret = (int16_t)value * scale_factor;
        return ret;  
    }
    else
    {
        ESP_LOGW(TAG, "Failed to read ADC data");
    }
    return 0;
}

/**
 * @brief 多次读取 ADS1115 单端通道或差分通道 ADC 值，并进行均值滤波
 * @param channel  选择单端输入通道（0-3）或差分输入通道（0-3）
 * @param gain FSR选择
 * @param avg_count 次数
 * @return float 电压值mV
 */
float ads1115_read_adc_avg(uint8_t channel, uint8_t gain, uint8_t avg_count)
{
    if (avg_count == 0) avg_count = 1; // 避免除以0

    float ret = 0;
    float sum = 0;

    for (uint8_t i = 0; i < avg_count; i++)
    {
        ads1115_set_config(channel, gain, ADS1115_RATE_128SPS);
        vTaskDelay(pdMS_TO_TICKS(10));  // 等待转换完成（最小 8ms）

        uint16_t value;
        if (ads1115_master_read(ADS1115_ADDRESS, ADS1115_CONVERSION_REG, &value) == ESP_OK)
        {
            float scale_factor = 0.0625; // 默认 ADS1115_GAIN_2_048V

            switch (gain)
            {
                case ADS1115_GAIN_0_512V:
                    scale_factor = 0.015625;
                    break;
                case ADS1115_GAIN_1_024V:
                    scale_factor = 0.03125;
                    break;
                case ADS1115_GAIN_2_048V:
                    scale_factor = 0.0625;
                    break;
                case ADS1115_GAIN_4_096V:
                    scale_factor = 0.125;
                    break;
                case ADS1115_GAIN_6_144V:
                    scale_factor = 0.1875;
                    break;
                default:
                    ESP_LOGW(TAG, "Invalid gain value, using default 2.048V");
                    break;
            }

            sum += (int16_t)value * scale_factor;
        }
        else
        {
            ESP_LOGW(TAG, "Failed to read ADC data at sample %d", i);
        }
    }

    ret = sum / avg_count;
    return ret;
}
/**
 * @brief 多次读取 ADS1115 单端通道或差分通道 ADC 值，并进行去极值的均值滤波
 * @param channel  选择单端输入通道（0-3）或差分输入通道（0-3）
 * @param gain FSR选择
 * @param avg_count 次数
 * @return float 电压值mV
 */
float ads1115_read_adc_avg_remv_extreme(uint8_t channel, uint8_t gain, uint8_t avg_count)
{
    if (avg_count < 3) avg_count = 3; // 至少3次才能去掉最大最小

    float ret = 0;
    float scale_factor = 0.0625; // 默认 ADS1115_GAIN_2_048V
    float readings[avg_count];

    // 获取多次读数
    for (uint8_t i = 0; i < avg_count; i++)
    {
        ads1115_set_config(channel, gain, ADS1115_RATE_128SPS);
        vTaskDelay(pdMS_TO_TICKS(10));  // 等待转换完成

        uint16_t value;
        if (ads1115_master_read(ADS1115_ADDRESS, ADS1115_CONVERSION_REG, &value) == ESP_OK)
        {
            switch (gain)
            {
                case ADS1115_GAIN_0_512V: scale_factor = 0.015625; break;
                case ADS1115_GAIN_1_024V: scale_factor = 0.03125;  break;
                case ADS1115_GAIN_2_048V: scale_factor = 0.0625;   break;
                case ADS1115_GAIN_4_096V: scale_factor = 0.125;    break;
                case ADS1115_GAIN_6_144V: scale_factor = 0.1875;   break;
                default:
                    ESP_LOGW(TAG, "Invalid gain value, using default 2.048V");
                    scale_factor = 0.0625;
                    break;
            }

            readings[i] = (int16_t)value * scale_factor;
        }
        else
        {
            ESP_LOGW(TAG, "Failed to read ADC data at sample %d", i);
            readings[i] = 0; // 读失败就记 0
        }
    }

    // 去除最大最小值
    float max = readings[0];
    float min = readings[0];
    float sum = 0;

    for (uint8_t i = 0; i < avg_count; i++)
    {
        if (readings[i] > max) max = readings[i];
        if (readings[i] < min) min = readings[i];
        sum += readings[i];
    }

    sum -= (max + min);
    ret = sum / (avg_count - 2);

    return ret;
}
