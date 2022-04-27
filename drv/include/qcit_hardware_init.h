/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_hardware_init.h
 *文件功能: 硬件初始化接口
 *作     者: 吴庭永
 *编写日期: 2020年6月19日14:48:39
 *修改历史: 无
 *
 ************************************************************************/
#ifndef QCIT_HARDWARE_INIT_H
#define QCIT_HARDWARE_INIT_H

#define UWB_TAG_ID_LEN 4



void hardware_init(void);     //硬件初始化
void log_init(void);
void idle_state_handle(void);
void sleep_process_lost_uwb_communication(void);
void sleep_process_power_too_low(void);


#endif

