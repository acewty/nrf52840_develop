/************************************************************************
 *
 *Copyright 2021 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_led_drv.c
 *文件功能: led驱动文件
 *作	    者：吴庭永
 *创建日期：2021年5月12日18:25:27
 *修改历史：Ver1.0，无
 *
 ************************************************************************/
#include "nrf52840.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "qcit_gpio_drv.h"
#include "qcit_led_drv.h"

void led_init(uint8 led_pin)
{
    nrf_gpio_cfg_output(led_pin);
	nrf_gpio_pin_set(led_pin);
}

//ver1.3使用的是共阳极RGB
void led_open(uint8 led_pin)
{
	nrf_gpio_pin_clear(led_pin);
}

void led_close(uint8 led_pin)
{
	nrf_gpio_pin_set(led_pin);
}

void led_toggle(uint8 led_pin)
{
    nrf_gpio_pin_toggle(led_pin);
}

void led_all_init(void)
{
    led_init(LED_R);
	led_init(LED_G);
	led_init(LED_B);
}

/**************************************************************************
 *函数名称：led_rgb_light_water
 *函数功能：rgb流水灯
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021-2-19 09:10:04
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
void led_rgb_light_water(void)
{
	led_open(LED_R);
	nrf_delay_ms(1000);
	led_close(LED_R);

	led_open(LED_G);
	nrf_delay_ms(1000);
	led_close(LED_G);

	led_open(LED_B);
	nrf_delay_ms(1000);
	led_close(LED_B);
}

