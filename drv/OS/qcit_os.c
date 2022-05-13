/************************************************************************
 *
 *Copyright 2022 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_os.c
 *文件功能: FreeRTOS信号量定义与生成等
 *作    者: 吴庭永
 *编写日期: 2022年5月13日10:29:41
 *修改历史: 无
 *
 ************************************************************************/
#include "sdk_config.h"

#include "qcit_os.h"
#include "qcit_variable_def.h"

#include "FreeRTOS.h"
#include "task.h"

#if NRF_LOG_ENABLED
TaskHandle_t logger_task_handle;    //日志任务的操作句柄
#endif


/**************************************************************************
 *函数名称：vApplicationIdleHook
 *函数功能：空闲任务的钩子函数
 *          @brief A function which is hooked to idle task.
 *          @note Idle hook must be enabled in FreeRTOS configuration (configUSE_IDLE_HOOK).
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2022年5月13日10:48:48
 *修改历史：Ver1.0，无
 **************************************************************************/
void vApplicationIdleHook(void)
{
#if NRF_LOG_ENABLED
	 vTaskResume(logger_task_handle);
#endif
}



