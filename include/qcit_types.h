
/************************************************************************
 *
 *Copyright 2019 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_types.h
 *文件功能: 类型重定义
 *作    者: 臧强
 *编写日期: 2018年8月3日10:44:41
 *修改历史: 无
 *
 ************************************************************************/


#ifndef QCIT_TYPE_DEF_H
#define QCIT_TYPE_DEF_H


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;


typedef signed char int8;
typedef signed short int16;
typedef signed long int32;


typedef signed long long int64;
typedef unsigned long long uint64;

//typedef signed int bool;

//布尔赋值
#define QCIT_FALSE         0
#define QCIT_TRUE          (!QCIT_FALSE)

#define QCIT_ENABLE        1
#define QCIT_DISABLE       0

//函数返回值
#define QCIT_FAIL         -1
#define QCIT_SUCCESS       0

#define QCIT_NULL ((void *)0UL)



#endif





