
/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_pwm_drv.h
 *文件功能: pwm驱动头文件
 *作    者: 吴庭永
 *编写日期: 2020-4-24 10:40:51
 *修改历史: Ver1.0，无
 *
 ************************************************************************/

#ifndef QCIT_NRF_PWM_DRV_H
#define QCIT_NRF_PWM_DRV_H

void pwm_init(void);
void pwm_uninit(void);	
void pwm_enable(void);
void pwm_disable(void);
void ble_breath_led(void);


#endif

