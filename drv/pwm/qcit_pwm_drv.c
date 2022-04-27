 /************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_pwm_drv.c
 *文件功能: pwm驱动文件
 *作		者：吴庭永
 *创建日期：2020-4-24 10:43:12
 *修改历史：Ver1.0，无

 *
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "nrf_drv_pwm.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "app_pwm.h"

#include "qcit_pwm_drv.h"
#include "qcit_gpio_drv.h"
#include "qcit_saadc_drv.h"


static volatile bool s_b_ready_flag;            //指示PWM状态的一个标志位
APP_PWM_INSTANCE(PWM1,1);             		    //使用TIMER1创建PWM实例 "PWM1"

/**************************************************************************
 *函数名称：pwm_ready_callback
 *函数功能：PWM回调函数，官方demo参考
 *输入参数：无
 *返 回 值：0
 *作	    者：吴庭永
 *创建日期：2020年6月20日21:09:53
 *修改历史：Ver1.0，无
 **************************************************************************/
void pwm_ready_callback(uint32_t pwm_id)    // PWM回调函数
{
    s_b_ready_flag = true;
}

/**************************************************************************
 *函数名称：pwm_init
 *函数功能：pwm的初始化
 *输入参数：无
 *返 回 值：0
 *作	    者：吴庭永
 *创建日期：2020年6月20日21:11:15
 *修改历史：Ver1.0，无
 **************************************************************************/
void pwm_init(void)    
{
	ret_code_t u32_err_code;
	//APP_PWM_DEFAULT_CONFIG_1CH配置周期以微秒为单位，L后缀表示为长整型，此周期和占空比应该是有由定时器1产生
	app_pwm_config_t pwm1_cfg = 
	{                                                                                  \
        .pins            = {LED_CHARGE_STATUS, APP_PWM_NOPIN},                         \
        .pin_polarity    = {APP_PWM_POLARITY_ACTIVE_LOW, APP_PWM_POLARITY_ACTIVE_LOW}, \
        .num_of_channels = 1,                                                          \
        .period_us       = 5000L                                                       \
    };
	
	u32_err_code = app_pwm_init(&PWM1,&pwm1_cfg,NULL);
	APP_ERROR_CHECK(u32_err_code);
}

/**************************************************************************
 *函数名称：pwm_uninit
 *函数功能：pwm取消初始化,并释放相应的资源
 *输入参数：无
 *返 回 值：0
 *作	    者：吴庭永
 *创建日期：2020年6月24日11:33:43
 *修改历史：Ver1.0，无
 **************************************************************************/
void pwm_uninit(void)    
{

	ret_code_t u32_err_code;
	//app_pwm_disable(&PWM1);
	u32_err_code = app_pwm_uninit(&PWM1);
	APP_ERROR_CHECK(u32_err_code);
	
}
bool g_b_pwm_status = QCIT_FALSE;
/**************************************************************************
 *函数名称：pwm_uninit
 *函数功能：pwm使能
 *输入参数：无
 *返 回 值：0
 *作	    者：吴庭永
 *创建日期：2020年6月24日11:33:43
 *修改历史：Ver1.0，无
 **************************************************************************/
void pwm_enable(void)    
{
	if(!g_b_pwm_status)
	{
		app_pwm_enable(&PWM1);
		g_b_pwm_status = QCIT_TRUE;
	}
	
}


/**************************************************************************
 *函数名称：pwm_uninit
 *函数功能：pwm失效
 *输入参数：无
 *返 回 值：0
 *作	    者：吴庭永
 *创建日期：2020年6月24日11:33:43
 *修改历史：Ver1.0，无
 **************************************************************************/
void pwm_disable(void)    
{
	if(g_b_pwm_status)	 //让pwm_disable只执行一次，否则会造成不正常
	{
		app_pwm_disable(&PWM1);
		g_b_pwm_status = QCIT_FALSE;
	}
	
}

/**************************************************************************
 *函数名称：ble_breath_led
 *函数功能：呼吸灯
 *输入参数：无
 *返 回 值：0
 *作	    者：吴庭永
 *创建日期：2020年6月18日12:23:03
 *修改历史：Ver1.0，无
 **************************************************************************/
void ble_breath_led(void)
{
    uint8 u8_value;
	for (uint8 i = 0; i < 100; ++i)
	{
		u8_value = (i < 50) ? (i * 2) : (100 - (i - 50) * 2);
	    //s_b_ready_flag = false;
	    //设置PWM的输出周期占空比，一直尝试直到PWM准备好
	    while(app_pwm_channel_duty_set(&PWM1, 0, u8_value) == NRF_ERROR_BUSY)
	    {
	    }
	    nrf_delay_ms(20);
	}
}


