#ifndef __SWITCH_H
#define __SWITCH_H

#include "driver/gpio.h"


/* 引脚定义 */
#define SWITCH_ADC1_GPIO GPIO_NUM_42
#define SWITCH_ADC2_GPIO GPIO_NUM_2
#define SWITCH_CURRENT1_GPIO GPIO_NUM_39
#define SWITCH_CURRENT2_GPIO GPIO_NUM_40

/* 函数声明*/
void switch_init(void);     /* 初始化LED */

#endif