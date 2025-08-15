#include "SWITCH.h"


/**
 * @brief       初始化控制模拟开关的GPIO
 * @param       无
 * @retval      无
 */
void switch_init(void)
{
    gpio_config_t gpio_init_struct_a1 = {0};

    gpio_init_struct_a1.intr_type = GPIO_INTR_DISABLE;         /* 失能引脚中断 */
    gpio_init_struct_a1.mode = GPIO_MODE_INPUT_OUTPUT;         /* 输入输出模式 */
    gpio_init_struct_a1.pull_up_en = GPIO_PULLUP_ENABLE;       /* 使能上拉 */
    gpio_init_struct_a1.pull_down_en = GPIO_PULLDOWN_DISABLE;  /* 失能下拉 */
    gpio_init_struct_a1.pin_bit_mask = 1ull << SWITCH_ADC1_GPIO;   /* 设置的引脚的位掩码 选择 GPIO 号*/
    gpio_config(&gpio_init_struct_a1);                         /* 配置GPIO */

    gpio_config_t gpio_init_struct_a2 = {0};

    gpio_init_struct_a2.intr_type = GPIO_INTR_DISABLE;         /* 失能引脚中断 */
    gpio_init_struct_a2.mode = GPIO_MODE_INPUT_OUTPUT;         /* 输入输出模式 */
    gpio_init_struct_a2.pull_up_en = GPIO_PULLUP_ENABLE;       /* 使能上拉 */
    gpio_init_struct_a2.pull_down_en = GPIO_PULLDOWN_DISABLE;  /* 失能下拉 */
    gpio_init_struct_a2.pin_bit_mask = 1ull << SWITCH_ADC2_GPIO;   /* 设置的引脚的位掩码 选择 GPIO 号*/
    gpio_config(&gpio_init_struct_a2);  

    gpio_config_t gpio_init_struct_c1 = {0};

    gpio_init_struct_c1.intr_type = GPIO_INTR_DISABLE;         /* 失能引脚中断 */
    gpio_init_struct_c1.mode = GPIO_MODE_INPUT_OUTPUT;         /* 输入输出模式 */
    gpio_init_struct_c1.pull_up_en = GPIO_PULLUP_ENABLE;       /* 使能上拉 */
    gpio_init_struct_c1.pull_down_en = GPIO_PULLDOWN_DISABLE;  /* 失能下拉 */
    gpio_init_struct_c1.pin_bit_mask = 1ull << SWITCH_CURRENT1_GPIO;   /* 设置的引脚的位掩码 选择 GPIO 号*/
    gpio_config(&gpio_init_struct_c1);  

    gpio_config_t gpio_init_struct_c2 = {0};

    gpio_init_struct_c2.intr_type = GPIO_INTR_DISABLE;         /* 失能引脚中断 */
    gpio_init_struct_c2.mode = GPIO_MODE_INPUT_OUTPUT;         /* 输入输出模式 */
    gpio_init_struct_c2.pull_up_en = GPIO_PULLUP_ENABLE;       /* 使能上拉 */
    gpio_init_struct_c2.pull_down_en = GPIO_PULLDOWN_DISABLE;  /* 失能下拉 */
    gpio_init_struct_c2.pin_bit_mask = 1ull << SWITCH_CURRENT2_GPIO;   /* 设置的引脚的位掩码 选择 GPIO 号*/
    gpio_config(&gpio_init_struct_c2);  



}