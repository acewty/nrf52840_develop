/************************************************************************
 *
 *Copyright 2018 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_lis2dh12_drv.h
 *文件功能: lis2dh12驱动头文件
 *作     者: 李彦
 *编写日期: 2020-3-20 16:46:51
 *修改历史: Ver1.0，无
 *			Ver2.0，增加函数定义
 *
 ************************************************************************/

#ifndef QCIT_LIS2DH12_DRV_H
#define QCIT_LIS2DH12_DRV_H

#include <stdint.h>
#include "qcit_types.h"


/* REGISTER MAPING*/
#define OUT_T_L (0x0D)
#define OUT_T_H (0x0E)
#define WHO_AM_I (0x0F)
#define CTRL1 (0x20)
#define CTRL2 (0x21)
#define CTRL3 (0x22)
#define CTRL4 (0x23)
#define CTRL5_INT1_PAD_CTRL (24h)
#define CTRL6_INT2_PAD_CTRL (25h)
#define OUT_T (0x26)
#define STATUS (0x27)
#define OUT_X_L (0x28)
#define OUT_X_H (0x29)
#define OUT_Y_L (0x2A)
#define OUT_Y_H (0x2B)
#define OUT_Z_L (0x2C)
#define OUT_Z_H (0x2D)
#define FIFO_CTRL (0x2E)
#define FIFO_SAMPLES (0x2F)
#define TAP_THS_X (0x30)
#define TAP_THS_Y (0x31)
#define TAP_THS_Z (0x32)
#define INT_DUR (0x33)
#define WAKE_UP_THS (0x34)
#define WAKE_UP_DUR (0x35)
#define FREE_FALL (0x36)
#define STATUS_DUP (0x37)
#define WAKE_UP_SRC (0x38)
#define TAP_SRC (0x39)
#define SIXD_SRC (0x3A)
#define ALL_INT_SRC (0x3B)
#define X_OFS_USR (0x3C)
#define Y_OFS_USR (0x3D)
#define Z_OFS_USR (0x3E)
#define CTRL7 (0x3F)

#define CTRL1_VALUE (0x2F)
#define CTRL2_VALUE (0x84)
#define CTRL4_VALUE (0x80)
#define CTRL6_VALUE (0x04)
#define LIS2DH12_SAD_WRITE (0x32U >> 1)	//模块SD0连接3v3，基地址为0011001，末尾1为read，0为write;而
#define LIS2DH12_SAD_READ (0x33U >> 1)

 
typedef struct 
{
	int16_t x_value;
	int16_t y_value;
	int16_t z_value;
} position_t;

void lis2dh12_initlise(void);
float lis2dh12_from_fs2_hr_to_mg(int16 i16_lsb);
position_t read_acc_value(void);

#endif

