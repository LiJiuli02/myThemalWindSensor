#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_psram.h"
#include "esp_flash.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wifi_config.h"
#include "tcp_client.h"
#include "ads1115.h"
#include "KEY.h"
#include "wind_calcu.h"
#include "magsensor.h"

#define TAG "MAIN" // 日志 TAG

//原始磁传感器结构体变量
typedef struct {
    int16_t raw_x;
    int16_t raw_y;
    int16_t raw_z;
} mag_data;



/*FreeRTOS*********************************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

/* KEY_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
// #define KEY_TASK_PRIO           7          /* 任务优先级 */
// #define KEY_STK_SIZE            2048        /* 任务堆栈大小 */
// TaskHandle_t KEYTask_Handler;               /* 任务句柄 */
// void key_task(void *pvParameters);          /* 任务函数 */

/* ADC_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define ADC_TASK_PRIO           9         /* 任务优先级 */
#define ADC_STK_SIZE            4096        /* 任务堆栈大小 */
TaskHandle_t ADCTask_Handler;               /* 任务句柄 */
void adc_task(void *pvParameters);          /* 任务函数 */

/* SEND_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define SEND_TASK_PRIO           7          /* 任务优先级 */
#define SEND_STK_SIZE            4096        /* 任务堆栈大小 */
TaskHandle_t SENDTask_Handler;               /* 任务句柄 */
void send_task(void *pvParameters);          /* 任务函数 */

/* WIND_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define WIND_TASK_PRIO           7          /* 任务优先级 */
#define WIND_STK_SIZE            4096        /* 任务堆栈大小 */
TaskHandle_t WINDTask_Handler;               /* 任务句柄 */
void wind_task(void *pvParameters);          /* 任务函数 */

/* MAG_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define MAG_TASK_PRIO           5           /* 任务优先级 */
#define MAG_STK_SIZE            4096        /* 任务堆栈大小 */
TaskHandle_t MAGTask_Handler;               /* 任务句柄 */
void mag_task(void *pvParameters);          /* 任务函数 */

//数据队列定义
QueueHandle_t adc_queue;  // 定义adc数据句柄
typedef struct {
    float raw1;
    float raw2;
    float raw3;
    float raw4;
} adc_data_t;

QueueHandle_t wind_queue;  // 定义风向风速数据句柄

/**
* @brief 程序入口
* @param 无
* @retval 无
*/
void app_main(void)
{
    esp_err_t ret;
    ret = nvs_flash_init();

    /* 初始化 NVS */
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES
    || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    //创建存储队列
    adc_queue = xQueueCreate(10, sizeof(adc_data_t));
    if (adc_queue == NULL) 
    {
        ESP_LOGE(TAG, "Failed to create ADC queue");
        return;
    }

    wind_queue = xQueueCreate(10, sizeof(wind_result_t_2));
    if (wind_queue == NULL) 
    {
        ESP_LOGE(TAG, "Failed to create WIND queue");
        return;
    }

    //初始化
    // key_init();
    wifi_sta_init();             /* 网络配置 */
    mYtcp_init();                /* tcp连接 */ 
    ads1115_init();        /* 初始化ADC */
    qmc5883p_init();       /*初始化地磁传感器*/ 

    //创建FreeRTOS Task
    /* key 任务 */
    // xTaskCreate((TaskFunction_t )key_task,
    //             (const char *   )"key_task",
    //             (uint16_t       )KEY_STK_SIZE,
    //             (void *         )NULL,
    //             (UBaseType_t    )KEY_TASK_PRIO,
    //             (TaskHandle_t * )&KEYTask_Handler);
    /* adc 任务 */
    xTaskCreate((TaskFunction_t )adc_task,
                (const char *   )"adc_task",
                (uint16_t       )ADC_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )ADC_TASK_PRIO,
                (TaskHandle_t * )&ADCTask_Handler);
    /* send 任务 */
    xTaskCreate((TaskFunction_t )send_task,
                (const char *   )"send_task",
                (uint16_t       )SEND_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )SEND_TASK_PRIO,
                (TaskHandle_t * )&SENDTask_Handler);
    /* wind calcu 任务 */
    xTaskCreate((TaskFunction_t )wind_task,
                (const char *   )"wind_task",
                (uint16_t       )WIND_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )WIND_TASK_PRIO,
                (TaskHandle_t * )&WINDTask_Handler);
    /* mag 任务 */
                xTaskCreate((TaskFunction_t )mag_task,
                (const char *   )"mag_task",
                (uint16_t       )MAG_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )MAG_TASK_PRIO,
                (TaskHandle_t * )&MAGTask_Handler);

}

/**
 * @brief       key_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
// void key_task(void *pvParameters)
// {
//     pvParameters = pvParameters;
//     uint8_t key;
//     while (1)
//     {
//         key = key_scan(0);
//         if (key)
//         {
            
//         }
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

/**
 * @brief  采集 ADS1115 数据
 */
void adc_task(void *pvParameters) 
{
    float raw1, raw2, raw3, raw4;
    adc_data_t adc_data;
    while (1) 
    {
        raw1 = ads1115_read_adc_avg_remv_extreme(ADS1115_DIFFER_CHANNEL_3, ADS1115_GAIN_4_096V,5);
        vTaskDelay(pdMS_TO_TICKS(10));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
        raw2 = ads1115_read_adc_avg_remv_extreme(ADS1115_CHANNEL_3, ADS1115_GAIN_4_096V,5);
        ESP_LOGI(TAG, "PT1=%.4f, PT2=%.4f", raw1, raw2);
        vTaskDelay(pdMS_TO_TICKS(10)); 
        raw3 = ads1115_read_adc_avg_remv_extreme(ADS1115_DIFFER_CHANNEL_0, ADS1115_GAIN_4_096V,5); 
        vTaskDelay(pdMS_TO_TICKS(10));  
        raw4 = ads1115_read_adc_avg_remv_extreme(ADS1115_CHANNEL_1,ADS1115_GAIN_4_096V,5);
        ESP_LOGI(TAG, "PT3=%.4f, PT4=%.4f", raw3, raw4);
        
        // 将数据存入队列
        adc_data.raw1 = raw1;
        adc_data.raw2 = raw2;
        adc_data.raw3 = raw3;
        adc_data.raw4 = raw4;
        if (xQueueSend(adc_queue, &adc_data, portMAX_DELAY) != pdPASS) 
        {
            ESP_LOGW(TAG, "Failed to send ADC data to queue");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief  北向识别任务
 */
void mag_task(void *pvParameters) 
{
    // mag_data mag_data_raw;
    float heading_deg; //方位角
    while(1)
    {
        //有返回值的函数需要 ret = fun(); 调用
        qmc5883p_get_heading_deg(&heading_deg);

        // ESP_LOGI(TAG, "x:%d, y:%d, z:%d", mag_data_raw.raw_x, mag_data_raw.raw_y, mag_data_raw.raw_z);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

/**
 * @brief  风向计算任务
 */
void wind_task(void *arg)
{
    adc_data_t adc_data;
    wind_result_t_2 wind_result;
    while (1) 
    {
        if (xQueueReceive(adc_queue, &adc_data, portMAX_DELAY) == pdPASS) 
        {
            // 计算风向
            wind_result = calculate_wind_direction_2(
                adc_data.raw1,  // N
                adc_data.raw3,  // S
                adc_data.raw2,  // E
                adc_data.raw4   // W
            );

            ESP_LOGI(TAG, "Wind Direction: %.0f, Speed : %.2f", 
                    wind_result.direction, wind_result.speed);

            if (xQueueSend(wind_queue, &wind_result, portMAX_DELAY) != pdPASS) 
            {
                ESP_LOGW(TAG, "Failed to send WIND data to queue");
            }
        }
    }
}

/**
 * @brief  发送数据任务
 */
void send_task(void *pvParameters) 
{
    wind_result_t_2 rec_wind_result;
    char send_buffer[100];

    while (1) 
    {
    

        if (xQueueReceive(wind_queue, &rec_wind_result, portMAX_DELAY) == pdPASS) 
        {


            snprintf(send_buffer, sizeof(send_buffer), 
                     "Wind Direction: %.0f, Speed: %.2f", rec_wind_result.direction, rec_wind_result.speed);

            /* 发送数据 */
            if (g_lwip_connect_state == 1 && g_sock >= 0) 
            {
                int err = write(g_sock, send_buffer, strlen(send_buffer));
                if (err < 0) 
                {
                    ESP_LOGE(TAG, "Error sending data, closing socket");
                    closesocket(g_sock);
                    g_sock = -1;
                    g_lwip_connect_state = 0;
                }
                else
                {
                    ESP_LOGI(TAG, "SEND:%s", send_buffer);
                }
            }
            else
            {
                mYtcp_init();
            }
            // ESP_LOGI(TAG, "con_state:%d and %d", g_lwip_connect_state,g_sock);
        }

    }
}