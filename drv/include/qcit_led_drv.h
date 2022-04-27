/************************************************************************
 *
 *Copyright 2021 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_led_drv.h
 *文件功能: 串口驱动头文件
 *作     者: 吴庭永
 *编写日期: 2021年5月12日18:15:16
 *修改历史: Ver1.0，无
 *
 ************************************************************************/
#ifndef QCIT_LED_DRV_H
#define	QCIT_LED_DRV_H

#include "qcit_types.h"

void led_init(uint8 led_pin);
void led_open(uint8 led_pin);
void led_close(uint8 led_pin);
void led_toggle(uint8 led_pin);
void led_rgb_light_water(void);
void led_all_init(void);

#endif

