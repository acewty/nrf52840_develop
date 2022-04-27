/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_exit_drv.c
 *作     者: 吴庭永
 *编写日期: 2020-4-2 17:03:05
 *修改历史: ver1.0，无
 *
 ************************************************************************/

#include <stdbool.h>
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_drv_ppi.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "qcit_exit_drv.h"
#include "qcit_gpio_drv.h"
#include "qcit_led_drv.h"
#include "qcit_pwm_drv.h"
#include "qcit_saadc_drv.h" 
#include "qcit_variable_def.h"


bool g_b_charger_detect_pin_interrupt = QCIT_FALSE;  //充电器检测引脚中断
bool g_b_charger_insert_flag = QCIT_FALSE;           //充电器插入标志,移除为false

extern E_TAG_SLEEP_TYPE g_e_tag_sleep_status;

/**************************************************************************
*函数名称：exit_charger_detect_pin_init
*函数功能：充电器检测引脚外部中断初始化
*输入参数：无
*返 回 值：无
*作 	   者：吴庭永
*创建日期：2021年3月16日19:25:08
*修改历史：Ver1.0，无
**************************************************************************/ 
void exit_charger_detect_pin_init(void)
{	
	ret_code_t err_code;
	
	if(!nrfx_gpiote_is_init())
	{	
		err_code = nrf_drv_gpiote_init();
		APP_ERROR_CHECK(err_code);
	}
	
	//ture代表使用中断事件，false代表port事件,中断类型为高变低;
	nrf_drv_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
	in_config.pull = NRF_GPIO_PIN_NOPULL;
	
	err_code = nrf_drv_gpiote_in_init(CHARGER_DETECT_PIN, &in_config, exit_charger_detect_pin_handler);
	APP_ERROR_CHECK(err_code);
	
	nrf_drv_gpiote_in_event_enable(CHARGER_DETECT_PIN, true);
} 

/**************************************************************************
*函数名称：exit_charger_detect_pin_handler
*函数功能：充电电压检测引脚外部中断处理函数
*输入参数：pin;    	action 
*返 回 值：无
*作 	   者：吴庭永
*创建日期：2021年3月17日09:46:46
*修改历史：Ver1.0，无
**************************************************************************/ 
void exit_charger_detect_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)             
{
	g_b_charger_detect_pin_interrupt = QCIT_TRUE;
	if(nrf_gpio_pin_read(CHARGER_DETECT_PIN))   //充电器插入
	{
		g_b_charger_insert_flag = QCIT_TRUE;
	}
	else        
	{
		g_b_charger_insert_flag = QCIT_FALSE;
	}
}

/**************************************************************************
*函数名称：charge_process
*函数功能：当充电器检测中断触发后，执行的充电状态判断程序
*输入参数：无
*返 回 值：无
*作 	   者：吴庭永
*创建日期：2021年8月11日22:37:24
*修改历史：Ver1.0，无
**************************************************************************/
void charge_process(void) 
{
	//等待BQ25060判断完成后，输出正确的charge_status电平。*测试插上充电器后，跳变为低电平时间126ms
	nrf_delay_ms(200);	
	//充电器插入状态，执行充电流程；如果在等待延时过程中移除充电器就不会进入充电流程 
	if(g_b_charger_insert_flag)		 
	{
		if(!nrf_gpio_pin_read(CHARGE_STATUS))		   //充电状态指示电平为0，设备正在充电，执行呼吸灯流程
		{
			pwm_enable();
			while(!nrf_gpio_pin_read(CHARGE_STATUS))   //等到充电状态电平为高（移除充电器或者充满），结束呼吸灯流程
			{
				ble_breath_led();					   //呼吸灯,一次两秒
			}
			pwm_disable();
			if(g_b_charger_insert_flag)                //充电器还存在，表示设备充满，指示灯长亮
			{
				 led_open(LED_G);
			}
		}
		else										   //插上充电器时设备是满电状态，指示灯长亮
		{
			led_open(LED_G);						 
		}
	}
	else											   //充电器移除，指示灯关闭
	{
		led_close(LED_G);
	}

}


