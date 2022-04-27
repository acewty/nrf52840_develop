/************************************************************************
 *
 *Copyright 2021 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_flash_drv.h
 *文件功能: flash存储驱动接口
 *作     者: 吴庭永
 *编写日期: 2021年4月8日20:33:42
 *修改历史: 无
 *
 ************************************************************************/

#ifndef QCIT_FLASH_DRV_H
#define QCIT_FLASH_DRV_H

#include "qcit_types.h"
//#include "nrf_fstorage.h"

#define UWB_DEVICE_ID_ADDR             0x4F000

void flash_read_data(uint32 u32_flash_addr, uint32 u32_data_len, uint8 *pu8_read_data);  //flash读数据

#endif

