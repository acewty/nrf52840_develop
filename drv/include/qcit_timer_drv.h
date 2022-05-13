/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_timer_drv.h
 *文件功能: 定时器驱动头文件
 *作     者: 吴庭永
 *编写日期: 2020年7月11日14:49:27
 *修改历史: 无
 *
 ************************************************************************/
#include "qcit_types.h"

#ifndef QCIT_TIMER_DRV_H
#define	QCIT_TIMER_DRV_H

void app_timers_init(void);
void app_timer_seconds_cnt_start(void);
void app_timer_seconds_cnt_stop(void);
void clock_init(void);
void clock_source_xtal(void);
uint32 get_rtc1_counter(void);
void os_timers_init(void);
void os_app_led_timer_start(void);
void timer_ms_init(void);
void timer_dw1000_start(void);
void timer_ms_start(void);
void timer_ms_stop(void);
void timer_dw1000_stop(void);
void timer_ms_delay(uint32 u32_ms_delay);
void timer_dw1000_init(uint16 u16_cycle);



#endif
