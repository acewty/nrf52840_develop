/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_lisdh12_drv.c
 *文件功能: 加速度计的驱动
 *作	    者：吴庭永
 *创建日期：2020-3-20 15:22:45
 *修改历史：Ver1.0，无
 *
 ************************************************************************/
#include <math.h>
#include "nrf_delay.h"
#include "qcit_lis2dh12_drv.h"
#include "qcit_twi_drv.h"


static float acceleration_mg[3] = {0};
double g_d64_g_acc = 0;


/**
 * @brief Function for initlise lis2dh12.
 */
void lis2dh12_initlise(void)
{
	twi_write_onebyte(LIS2DH12_SAD_WRITE,CTRL1, CTRL1_VALUE);
	twi_write_onebyte(LIS2DH12_SAD_WRITE,CTRL4, CTRL4_VALUE);
	nrf_delay_us(500);  //执行完这两个写指令之后，立刻执行读取指令的时候，会提示总线忙，测试此处延时必须大于200us
}

 
float lis2dh12_from_fs2_hr_to_mg(int16 i16_lsb)
{
	return ((float)i16_lsb / 16.0f) * 1.0f;
}

position_t read_acc_value(void)	//acc:加速度
{
	position_t position;
	uint8 u8_tmp_low;
	uint8 u8_tmp_high; 
	twi_read_onebyte(LIS2DH12_SAD_WRITE, LIS2DH12_SAD_READ, OUT_X_L, &u8_tmp_low);
	twi_read_onebyte(LIS2DH12_SAD_WRITE, LIS2DH12_SAD_READ, OUT_X_H, &u8_tmp_high);
	position.x_value = (uint16)(u8_tmp_low & 0xFF) | ((uint16)u8_tmp_high<<8); 

	twi_read_onebyte(LIS2DH12_SAD_WRITE, LIS2DH12_SAD_READ, OUT_Y_L, &u8_tmp_low);
	twi_read_onebyte(LIS2DH12_SAD_WRITE, LIS2DH12_SAD_READ, OUT_Y_H, &u8_tmp_high);
	position.y_value = (uint16)(u8_tmp_low & 0xFF) | ((uint16)u8_tmp_high<<8);
	
	twi_read_onebyte(LIS2DH12_SAD_WRITE, LIS2DH12_SAD_READ, OUT_Z_L, &u8_tmp_low);
	twi_read_onebyte(LIS2DH12_SAD_WRITE, LIS2DH12_SAD_READ, OUT_Z_H, &u8_tmp_high);
	position.z_value = (uint16)(u8_tmp_low & 0xFF) | ((uint16)u8_tmp_high<<8); 	

	acceleration_mg[0] = lis2dh12_from_fs2_hr_to_mg(position.x_value);//x
	acceleration_mg[1] = lis2dh12_from_fs2_hr_to_mg(position.y_value);//y
	acceleration_mg[2] = lis2dh12_from_fs2_hr_to_mg(position.z_value);//z
	
	g_d64_g_acc = (double)(sqrt(pow(acceleration_mg[0], 2) + pow(acceleration_mg[1], 2) + pow(acceleration_mg[2], 2)));
	
	return position;
}

