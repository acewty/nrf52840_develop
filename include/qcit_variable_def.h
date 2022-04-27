/************************************************************************
 *
 *Copyright 2018 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_variable_def.h
 *文件功能: 变量、中断编号定义
 *作    者: 臧强
 *编写日期: 2018年11月28日13:03:46
 *修改历史: 无
 *
 ************************************************************************/
#include "qcit_types.h"
#include "qcit_hardware_init.h"

#ifndef QCIT_VARIABLE_DEF_H
#define QCIT_VARIABLE_DEF_H

#define UWB_READ_RX_DATA_LEN            10  //UWB读取接收数据长度
#define UWB_TWR_FRAME_ST_LEN            8   //UWB TWR定位首次发送帧长度
#define UWB_TWR_FRAME_FN_LEN            55  //UWB TWR定位2次发送帧长度

//天线延迟参数，默认的64 MHz PRF
#define DW1000_TX_ANT_DLY               0       //其他-16300-zq
#define DW1000_RX_ANT_DLY               32899   //其他-16456-zq //尝试值36598;34598;32398

#define TDMA_UNIT                       30//40       //TWR一个定位周期40毫秒
#define TDMA_ID_CAPACITY                25       //1秒内一个解算组的容纳量，2秒则翻倍
#define TDMA_ID_CAPACITY_2_SEC          50       //2秒内一个解算组的容纳量

//TWR定位流程
typedef enum 
{
        TWR_PRC_NONE = 0,         //默认状态
        TWR_PRC_START,            //TWR起始状态
        TWR_PRC_FINAL             //TWR结束
}E_TWR_PROCESS_STAT;

//休眠类型
typedef enum
{
	TAG_IS_WORKING = 0,                    //标签处于正常工作状态
	TAG_LOST_UWB_COMMUNICATION_SLEEP,      //标签处于丢失基站通讯的休眠状态，此时通过蓝牙来唤醒uwb定位功能
	TAG_POWER_TOO_LOW_SLEEP,               //标签处于电量过低的休眠状态，此时
}E_TAG_SLEEP_TYPE;


//extern uint8 g_u8_tag_sn[UWB_TAG_ID_LEN];


#endif 




