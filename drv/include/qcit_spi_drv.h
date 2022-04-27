/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_spi_drv.h
 *文件功能: I2C操作相关接口
 *作    者: 李彦
 *编写日期: 2020年7月9日09:12:37
 *修改历史: 无
 *
 ************************************************************************/

#ifndef QCIT_SPI_DRV_H
#define QCIT_SPI_DRV_H

#include "qcit_types.h"

void spi_set_rate_low (void);
void spi_set_rate_high (void);
void spi_init (void);
void spi_transfer(uint8 const *p_tx_buffer,uint8  tx_buffer_length,uint8 *p_rx_buffer,uint8 rx_buffer_length);
void spi_cs_set_low(void);
void spi_cs_set_high(void);


#endif

