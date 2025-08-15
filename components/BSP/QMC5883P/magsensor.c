#include"magsensor.h"

#define TAG "QMC5883P"  // 日志 TAG

/**
 * @brief 通过 I2C 向 QMC5883P 写入寄存器值
 * @param dev_addr  设备 I2C 地址（默认 0x48）
 * @param reg       目标寄存器地址
 * @param value      需要写入的 8 位数据
 * @return esp_err_t ESP_OK 表示成功，其他值表示失败
 */
static esp_err_t qmc5883p_write_register(uint8_t reg, uint8_t value) 
{
    uint8_t data[] = {reg, value};
    return i2c_master_write_to_device(I2C_MASTER_NUM_MAG, QMC5883P_ADDR, data, sizeof(data), 1000 / portTICK_PERIOD_MS);
}

/**
 * @brief 通过 I2C 从 QMC5883P 读取寄存器值
 * @param dev_addr  设备 I2C 地址（默认 0x48）
 * @param reg       目标寄存器地址
 * @param data      读取到的 8 位数据指针
 * @param len       数据长度
 * @return esp_err_t ESP_OK 表示成功，其他值表示失败
 */
static esp_err_t qmc5883p_read_registers(uint8_t reg, uint8_t *data, size_t len) 
{
    return i2c_master_write_read_device(I2C_MASTER_NUM_MAG, QMC5883P_ADDR, &reg, 1, data, len, 1000 / portTICK_PERIOD_MS);
}

/**
 * @brief 初始化 I2C 设备
 */
void qmc5883p_init() 
{

    ESP_LOGI(TAG, "Initializing QMC5883P......");

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM_MAG, &conf);
    i2c_driver_install(I2C_MASTER_NUM_MAG, conf.mode, 0, 0, 0);
    qmc5883p_write_register(0x29, 0x06); // Define the sign for X Y and Z axis

}

/**
 * @brief 读取三轴地磁方向数据
 * @param x x轴数据
 * @param y y轴数据
 * @param z z轴数据
 */
void qmc5883p_read_raw(int16_t *x, int16_t *y, int16_t *z) 
{
    qmc5883p_write_register(CONTROL_2_REG_ADDR, 0x00|RNG_8_GUASS); // Define Set/Reset mode, with Set/Reset On, Field Range 8Guass
    qmc5883p_write_register(CONTROL_1_REG_ADDR, 0x00|DOWN_SAMPLE_RATE_1|OVER_SAMPLE_RATE_8|OUTPUT_RATE_100Hz|NORMAL_MODE); // set normal mode, set ODR=200Hz
    uint8_t data[6];
    uint8_t status;
    // 读取状态寄存器 0x09，检查 bit[0] 是否为 1
    qmc5883p_read_registers(0x09, &status,1);
    if ( (status & 0x01) == 0x01 )
    {
        qmc5883p_read_registers(0x01, data, 6);
        // qmc5883p_read_registers(0x01, &data[0], 1);
        // qmc5883p_read_registers(0x02, &data[1], 1);
        // qmc5883p_read_registers(0x03, &data[2], 1);
        // qmc5883p_read_registers(0x04, &data[3], 1);
        // qmc5883p_read_registers(0x05, &data[4], 1);
        // qmc5883p_read_registers(0x06, &data[5], 1);
        *x = (int16_t)(data[1] << 8 | data[0]);
        *y = (int16_t)(data[3] << 8 | data[2]);
        *z = (int16_t)(data[5] << 8 | data[4]);
        ESP_LOGI(TAG, "x:%d, y:%d, z:%d", *x, *y, *z);

    }
    

}
/**
 * @brief 读取三轴地磁方向数据,并计算方位角
 * @param heading_deg 方位角角度
 */
void qmc5883p_get_heading_deg(float *heading_deg) 
{
    qmc5883p_write_register(CONTROL_2_REG_ADDR, 0x00|RNG_8_GUASS); // Define Set/Reset mode, with Set/Reset On, Field Range 8Guass
    qmc5883p_write_register(CONTROL_1_REG_ADDR, 0x00|DOWN_SAMPLE_RATE_1|OVER_SAMPLE_RATE_8|OUTPUT_RATE_100Hz|NORMAL_MODE); // set normal mode, set ODR=200Hz
    int16_t x, y, z;
    uint8_t data[6];
    uint8_t status;
    // 读取状态寄存器 0x09，检查 bit[0] 是否为 1
    qmc5883p_read_registers(0x09, &status,1);
    if ( (status & 0x01) == 0x01 )
    {
        qmc5883p_read_registers(0x01, data, 6);
        // qmc5883p_read_registers(0x01, &data[0], 1);
        // qmc5883p_read_registers(0x02, &data[1], 1);
        // qmc5883p_read_registers(0x03, &data[2], 1);
        // qmc5883p_read_registers(0x04, &data[3], 1);
        // qmc5883p_read_registers(0x05, &data[4], 1);
        // qmc5883p_read_registers(0x06, &data[5], 1);
        x = (int16_t)(data[1] << 8 | data[0]);
        y = (int16_t)(data[3] << 8 | data[2]);
        z = (int16_t)(data[5] << 8 | data[4]);
        // ESP_LOGI(TAG, "x:%d, y:%d, z:%d", x, y, z);
        
        // 计算方位角（弧度）
        float heading_rad = atan2(y, x);
        // 转换为角度（0~360°）
        *heading_deg = heading_rad * 180 / M_PI + magnetic_declination;
        if(*heading_deg < 0) *heading_deg += 360;

        ESP_LOGI(TAG, "heading_deg:%.2f", *heading_deg);
    }

}
