#ifndef WIND_CALCU_H
#define WIND_CALCU_H

#include <stdint.h>
#include <math.h>
#include "esp_log.h"

#define PARAMATER_A    0.2    
#define PARAMATER_B    0.05  
#define PARAMATER_n    2  
#define CURRENT       18.6   // 铂电阻恒流驱动电流 (mA)
#define Vn_quie       2146// 静态电压 N
#define Vs_quie       2143// 静态电压 S
#define Ve_quie       2143// 静态电压 E
#define Vw_quie       2144// 静态电压 W

// 定义方向枚举
typedef enum {
    WIND_DIR_N = 0,
    WIND_DIR_NE,
    WIND_DIR_E,
    WIND_DIR_SE,
    WIND_DIR_S,
    WIND_DIR_SW,
    WIND_DIR_W,
    WIND_DIR_NW,
    WIND_DIR_UNKNOWN
} wind_direction_t;

// 定义风向计算结果结构体
typedef struct {
    wind_direction_t direction;
    float speed_ratio;  // 相对风速比例
} wind_result_t;

// 定义风向计算结果结构体
typedef struct {
    float direction;
    float speed;  // 风速
} wind_result_t_2;

// 函数声明
wind_result_t calculate_wind_direction(float Vn, float Vs, float Ve, float Vw);
wind_result_t_2 calculate_wind_direction_2(float Vn, float Vs, float Ve, float Vw);

#endif // WIND_CALCU_H