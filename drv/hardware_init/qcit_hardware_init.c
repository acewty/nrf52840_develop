/************************************************************************
 *
 *Copyright 2021 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_hardware_init.h
 *文件功能: 硬件初始化接口
 *作     者: 吴庭永
 *编写日期: 2020年6月19日14:49:33
 *修改历史: 无
 *
 ************************************************************************/

#include <string.h>


#include "qcit_ble_drv.h"
#include "qcit_hardware_init.h"
#include "qcit_led_drv.h"
#include "qcit_gpio_drv.h"
#include "qcit_saadc_drv.h"
#include "qcit_exit_drv.h"
#include "qcit_pwm_drv.h"
#include "qcit_twi_drv.h"
#include "qcit_timer_drv.h"
#include "qcit_flash_drv.h"
#include "qcit_lis2dh12_drv.h"
#include "qcit_spi_drv.h"
#include "qcit_variable_def.h"

#include "nrf_pwr_mgmt.h"
#include "nrf_clock.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


uint8 g_u8_tag_sn[UWB_TAG_ID_LEN] = {0};
uint32 g_u32_tag_sn; 


static void qcit_read_param_from_flash(void);
static void power_management_init(void);

/**************************************************************************
 *函数名称：hardware_init
 *函数功能：硬件初始化
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年2月20日10:42:16
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
void hardware_init(void)
{	
	power_management_init();
	clock_source_xtal();          //高频时钟源切换为外部晶振
	//timer_ms_init();
	//app_timers_init();
	//app_timer_seconds_cnt_start();
	//timer_ms_start();
	//timer_dw1000_init(1000);
	//timer_dw1000_start();

	led_init(LED_NRF52840_BRACELET);
	
	//exit_charger_detect_pin_init();           //充电器检测引脚中断初始化


}

/**************************************************************************
*函数名称：power_management_init
*函数功能：初始化电源管理
*输入参数：无
*返 回 值：无
*作	   者：李彦
*创建日期：2020年4月28日10:33:09
*修改历史：Ver1.0，无
*          Ver2.0，wty 2021年4月19日19:28:03
*		   修改：将函数定义移到硬件初始化，并改为静态函数
**************************************************************************/
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
	//NRF_POWER->TASKS_CONSTLAT = 1;    //让设备处于idle时，以恒定延迟唤醒
}

/**************************************************************************
 *函数名称：qcit_read_param_from_flash
 *函数功能：从flash读取标签ID参数
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年2月20日10:42:16
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
static void qcit_read_param_from_flash(void)
{
	uint8 u8_read_tag_id[UWB_TAG_ID_LEN] = {0}; //从flash中读取标签ID-zq
        

	//从flash中读取ID-wty-2021年2月22日08:59:29
	flash_read_data(UWB_DEVICE_ID_ADDR, UWB_TAG_ID_LEN, u8_read_tag_id);
	memcpy(g_u8_tag_sn,u8_read_tag_id,4);
        
	g_u32_tag_sn = (uint32)(u8_read_tag_id[3] << 24) + (uint32)(u8_read_tag_id[2] << 16) +
                   (uint32)(u8_read_tag_id[1] << 8) + u8_read_tag_id[0];//标签ID
}

/**************************************************************************
 *函数名称：log_init
 *函数功能：日志打印初始化,来源于SDK，无修改
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2020年12月22日11:44:11
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**************************************************************************
*函数名称：idle_state_handle
*函数功能：处理空闲状态的函数接口，用于主循环中
*输入参数：无
*返 回 值：无
*作	   者：李彦
*创建日期：2020年4月28日10:33:09
*修改历史：Ver1.0，无
*          Ver2.0，wty 2021年4月19日19:34:10
*		   修改：将函数定义移到硬件初始化
**************************************************************************/
void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}




