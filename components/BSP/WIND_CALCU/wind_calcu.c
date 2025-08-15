#include "wind_calcu.h"

#define TAG "WIND_CALCU"


/**
 * @brief 风速和8风向计算
 * @param Vn
 * @param Vs
 * @param Ve
 * @param Vw
 * @return wind_result_t 风速风向信息
 */
wind_result_t calculate_wind_direction(float Vn, float Vs, float Ve, float Vw)
{
    wind_result_t result;
    
    float Speed_N = powf((CURRENT * (Vn_quie - Vn) / 1000 - PARAMATER_A ) / PARAMATER_B, PARAMATER_n );
    float Speed_S = powf((CURRENT * (Vs_quie - Vs) / 1000 - PARAMATER_A ) / PARAMATER_B, PARAMATER_n );
    float Speed_E = powf((CURRENT * (Ve_quie - Ve) / 1000 - PARAMATER_A ) / PARAMATER_B, PARAMATER_n );
    float Speed_W = powf((CURRENT * (Vw_quie - Vw) / 1000 - PARAMATER_A ) / PARAMATER_B, PARAMATER_n );

    float Speed_NS =  Speed_N - Speed_S;
    float Speed_EW =  Speed_E - Speed_W;
    result.speed_ratio = sqrtf(Speed_NS*Speed_NS + Speed_EW*Speed_EW);

    float angle_rad = atan2f(Speed_NS, Speed_EW);
    float angle_deg = angle_rad * 180.0 / M_PI;
    if (angle_deg < 0) 
    {
        angle_deg += 360.0;
    }
    
    // 将角度转换为方向
    if (result.speed_ratio < 1) // 假设小于一定数值为无风
    {  
        result.direction = WIND_DIR_UNKNOWN;
    } 
    else 
    {
        int sector = (int)((angle_deg + 22.5) / 45) % 8;
        switch (sector) 
        {
            case 0: result.direction = WIND_DIR_E; break;
            case 1: result.direction = WIND_DIR_NE; break;
            case 2: result.direction = WIND_DIR_N; break;
            case 3: result.direction = WIND_DIR_NW; break;
            case 4: result.direction = WIND_DIR_W; break;
            case 5: result.direction = WIND_DIR_SW; break;
            case 6: result.direction = WIND_DIR_S; break;
            case 7: result.direction = WIND_DIR_SE; break;
            default: result.direction = WIND_DIR_UNKNOWN;
        }
    }
    
    return result;
}

/**
 * @brief 风速和风向计算
 * @param Vn
 * @param Vs
 * @param Ve
 * @param Vw
 * @return wind_result_t_2 风速风向信息
 */
wind_result_t_2 calculate_wind_direction_2(float Vn, float Vs, float Ve, float Vw)
{
    wind_result_t_2 result;
    
    float Speed_N = powf((CURRENT * (Vn_quie - Vn) / 1000 - PARAMATER_A ) / PARAMATER_B, PARAMATER_n );
    float Speed_S = powf((CURRENT * (Vs_quie - Vs) / 1000 - PARAMATER_A ) / PARAMATER_B, PARAMATER_n );
    float Speed_E = powf((CURRENT * (Ve_quie - Ve) / 1000 - PARAMATER_A ) / PARAMATER_B, PARAMATER_n );
    float Speed_W = powf((CURRENT * (Vw_quie - Vw) / 1000 - PARAMATER_A ) / PARAMATER_B, PARAMATER_n );

    float Speed_NS =  Speed_N - Speed_S;
    float Speed_EW =  Speed_E - Speed_W;
    result.speed = sqrtf(Speed_NS*Speed_NS + Speed_EW*Speed_EW);

    float angle_rad = atan2f(-Speed_NS, -Speed_EW);
    float angle_deg = angle_rad * 180.0 / M_PI;
    if (angle_deg < 0) 
    {
        angle_deg += 360.0;
    }
    
    // 将角度转换为方向
    if (result.speed < 1) // 假设小于一定数值为无风
    {  
        result.direction = -1;
    } 
    else 
    {
        result.direction = angle_deg;
    }
    
    return result;
}