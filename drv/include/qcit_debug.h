/************************************************************************
 *
 *Copyright 2022 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_debug.h
 *文件功能: 调试使用函数头文件
 *作     者: 吴庭永
 *编写日期: 2022年5月9日10:51:24
 *修改历史: 无
 *
 ************************************************************************/

#ifndef QCIT_DEBUG_H
#define QCIT_DEBUG_H

#include "qcit_types.h"

//调试使用全局变量宏定义
#define DEBUG_VARIABLE                  1

extern uint32 g_u32_dw3000_all_interrupt_cnt;
extern uint32 g_u32_get_rtc1_cnt;
extern uint32 g_u32_get_rtc1_cnt_diff;
   
void logger_task(void * arg);

#endif 





