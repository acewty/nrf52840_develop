/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_exit_drv.h
 *作    者: 吴庭永
 *编写日期: 2020-4-2 17:17:14
 *修改历史: ver1.0，无
 *
 ************************************************************************/
 
#include "nrf_drv_gpiote.h"
#include "qcit_types.h"

#ifndef QCIT_EXIT_DRV_H
#define	QCIT_EXIT_DRV_H

void exit_charger_detect_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void exit_charger_detect_pin_init(void);
void charge_process(void);


#endif
