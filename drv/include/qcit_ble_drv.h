/************************************************************************
 *
 *Copyright 2021 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_ble_drv.h
 *文件功能: BLE驱动头文件
 *作     者: 吴庭永
 *编写日期: 2021年3月20日16:31:46
 *修改历史: 无
 *
 ************************************************************************/

#ifndef QCIT_BLE_DRV_H
#define QCIT_BLE_DRV_H

#define APP_BLE_CONN_CFG_TAG            1       /**< A tag identifying the SoftDevice BLE configuration. */
#define ADV_TDMA_FRAME_VALID_DATA_LEN   16      //广播TDMA帧的有效数据（去掉帧头TDMA占16个字节）

void ble_stack_init(void);
void ble_scan_init_and_start(void);
void ble_scan_start(void);
void ble_scan_in_sleep_mode(void);
void ble_scan_in_uwb_refresh(void);


#endif 

