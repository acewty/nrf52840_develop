/************************************************************************
 *
 *Copyright 2022 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_debug.c
 *文件功能: 调试使用函数
 *作     者: 吴庭永
 *编写日期: 2022年5月9日10:33:31
 *修改历史: 无
 *
 ************************************************************************/
#include <stdio.h>

#include "sdk_config.h"
#include "nordic_common.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "qcit_debug.h"

#include "FreeRTOS.h"
#include "task.h"

#if DEBUG_VARIABLE

uint32 g_u32_dw3000_all_interrupt_cnt = 0;    //dw3000所有中断计时
uint32 g_u32_get_rtc1_cnt= 0;
uint32 g_u32_get_rtc1_cnt_diff= 0;

#endif

/**************************************************************************
 *函数名称：logger_thread
 *函数功能：处理之前延后的日志
 *输入参数：* arg，任务创建时传入的指针
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2022年5月13日15:37:56
 *修改历史：Ver1.0，无 
 **************************************************************************/
#if NRF_LOG_ENABLED
/**@brief Thread for handling the logger.
 *
 * @details This thread is responsible for processing log entries if logs are deferred.
 *          Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
void logger_task(void * arg)
{
    UNUSED_PARAMETER(arg);

    while (1)
    {
        NRF_LOG_FLUSH();

        vTaskSuspend(NULL); // Suspend myself
    }
}
#endif //NRF_LOG_ENABLED

