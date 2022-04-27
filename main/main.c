/************************************************************************
 *
 *Copyright 2022 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: main.c
 *文件功能: 主函数c文件
 *作    者: 吴庭永
 *编写日期: 2022年4月14日17:41:20
 *修改历史: 无
 *
 ************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "nrf_ble_scan.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "FreeRTOS.h"
#include "task.h"

#include "qcit_gpio_drv.h"
#include "qcit_hardware_init.h"
#include "qcit_types.h"
#include "qcit_ble_drv.h"
#include "qcit_led_drv.h"
#include "qcit_flash_drv.h"
#include "qcit_saadc_drv.h"
#include "qcit_timer_drv.h"
#include "qcit_exit_drv.h"
#include "qcit_pwm_drv.h"
   
void vApplicationIdleHook()
{
}

#if NRF_LOG_ENABLED
static TaskHandle_t m_logger_thread;                                /**< Definition of Logger thread. */
#endif

uint32 g_u32_cnt = 0;
/**************************************************************************
 *函数名称：main
 *函数功能：main函数
 *输入参数：无
 *返 回 值：0
 *作	    者：吴庭永
 *创建日期：2022年4月14日17:42:52
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
int  main(void)
{
	log_init();
	hardware_init();
	ble_stack_init();
	
	#if NRF_LOG_ENABLED
    // Start execution.
    if (pdPASS != xTaskCreate(logger_thread, "LOGGER", 256, NULL, 1, &m_logger_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
#endif
	//nrf_sdh_freertos_init(advertising_start, NULL);
	NRF_LOG_INFO("FreeRTOS example started.");

	//激活深度睡眠模式
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	vTaskStartScheduler();
	while(true)
    {
        APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    }
}
