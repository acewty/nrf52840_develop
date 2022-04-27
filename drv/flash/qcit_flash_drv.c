
/************************************************************************
 *
 *Copyright 2018 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_flash_drv.c
 *文件功能: MCU片上flash操作
 *作    者: 臧强
 *编写日期: 2018年10月23日09:39:25
 *修改历史: 无
 *
 ************************************************************************/
#include "app_error.h"
#include "nrf52840.h"
#include "nrf_log.h"
#include "nrf_soc.h"

#include "qcit_flash_drv.h"
#include "qcit_types.h"

//打开flash读保护
//const uint32_t UICR_APPROTECT __attribute__((at(0x10001208))) __attribute__((used)) = 0xFFFFFF00;


/**********************************************************************************************
 *函数名称：flash_read_data
 *函数功能：读flash数据
 *输入参数：u32_flash_addr,读数据的flash地址;u8_len,读取数据的长度,pu8_read_data,读取的数据
 *返 回 值：无
 *作	    者：臧强
 *创建日期：2018年10月23日09:51:28
 *修改历史：Ver1.0，无
 **********************************************************************************************/
void flash_read_data(uint32 u32_flash_addr, uint32 u32_data_len, uint8 *pu8_read_data)
{
	uint32 u32_cnt = 0;

	for(u32_cnt = 0; u32_cnt < u32_data_len; u32_cnt++)
	{
		pu8_read_data[u32_cnt] = *(uint8 *)(UWB_DEVICE_ID_ADDR + u32_cnt);
	}
}


