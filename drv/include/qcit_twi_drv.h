
/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_twi_drv.h
 *文件功能: twi驱动头文件
 *作    者:   吴庭永
 *编写日期: 2020-3-23 10:36:20
 *修改历史: Ver1.0，无
 *
 ************************************************************************/

#ifndef QCIT_TWI_DRV_H
#define QCIT_TWI_DRV_H
 
#include <stdint.h>
#include "qcit_types.h"

void twi_init(void);	//TWI初始化配置，SCL：P0.27；SDA：P0.26
void twi_read_onebyte(uint32 twi_sad_write_addr, uint32 twi_sad_read_addr, uint8_t reg_addr, uint8_t * data);
void twi_write_onebyte(uint32 twi_sad_write_addr, uint8_t reg_addr, uint8_t data);

#endif

