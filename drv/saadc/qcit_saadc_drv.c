 /************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_saadc_drv.c
 *文件功能: adc函数驱动接口
 *作	    者：吴庭永
 *创建日期：2020-3-31 11:06:04
 *修改历史：Ver1.0，无

 *
 ************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "nrf_delay.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "app_error.h"
#include "app_util_platform.h"
//#include "boards.h"

#include "qcit_saadc_drv.h"
#include "qcit_types.h"

#define VOlTAGE_POWER_TOO_LOW                               3.2    //电量过低阈值
#define VOlTAGE_POWER_LOW                                   3.45
#define VOlTAGE_POWER_NORMAL                                3.5

bool g_b_power_too_low = QCIT_FALSE;  //电压过低标志位
float g_f_voltage_val = 0.0;          //保存SAADC采样数据计算得到的电池实际电压值

/**************************************************************************
 *函数名称：saadc_callback
 *函数功能：adc回调函数
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2020年6月19日14:51:25
 *修改历史：Ver1.0，无
 **************************************************************************/
void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{   
}

/**************************************************************************
 *函数名称：saadc_init
 *函数功能：adc初始化函数,检测引脚为P0.02/AIN0
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年3月16日15:03:35
 *修改历史：Ver1.0，无
 **************************************************************************/
void saadc_init(void)
{
    ret_code_t u32_err_code = 0;
    nrf_saadc_channel_config_t t_channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);
	//t_channel_config .acq_time = SAADC_CH_CONFIG_TACQ_40us;
    u32_err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(u32_err_code);

    u32_err_code = nrf_drv_saadc_channel_init(0, &t_channel_config);
    APP_ERROR_CHECK(u32_err_code);

}

/**************************************************************************
 *函数名称：voltage_detect
 *函数功能：电池电压检测
 *输入参数：无
 *返 回 值：0
 *作	    者：吴庭永
 *创建日期：2020年6月22日09:35:55
 *修改历史：Ver1.0，无
 **************************************************************************/
 void voltage_detect(void)
{
	saadc_init();                             //电量ADC检测初始化
	nrf_saadc_value_t saadc_val = 0;
	float f_val = 0.0;
	
	nrf_drv_saadc_sample_convert(0,&saadc_val);
	f_val = saadc_val * 3.6 /1024;
	g_f_voltage_val = (f_val*126)/75;

	nrf_drv_saadc_uninit();                  //取消saadc初始化
	
}




