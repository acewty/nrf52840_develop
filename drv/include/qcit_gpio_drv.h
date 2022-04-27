/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_gpio_drv.h
 *文件功能: 串口驱动头文件
 *作    者: 吴庭永
 *编写日期: 2020-3-20 16:46:51
 *修改历史: Ver1.0，无
 *
 ************************************************************************/
#ifndef QCIT_GPIO_DRV_H
#define	QCIT_GPIO_DRV_H

#include "qcit_types.h"

//uwb标签Ver1.3的RGB引脚R和G交换
#define LED_R_G_CHANGE        0

#if LED_R_G_CHANGE
	#define LED_R          9
	#define LED_G          10
	
#else
	#define LED_R          10
	#define LED_G          9
#endif

#define LED_B                   NRF_GPIO_PIN_MAP(1,6)
#define LED_NRF52840_BRACELET   NRF_GPIO_PIN_MAP(1,9)

#define CHARGER_DETECT_PIN   4                  //充电器检测引脚
#define CHARGE_STATUS        25                 //bq25060充电状态指示引脚
#define LED_CHARGE_STATUS    LED_G              //绿灯指示充电状态
#define LED_BATTERY          LED_R              //红灯指示电池电压低

#define LED3030_CE     NRF_GPIO_PIN_MAP(0,7)	//蓝牙手环大功率led使能控制引脚



#define EBYTE_LED1     NRF_GPIO_PIN_MAP(0,17)
#define EBYTE_LED2     NRF_GPIO_PIN_MAP(0,18)
#define EBYTE_KEY1     NRF_GPIO_PIN_MAP(0,14)
#define EBYTE_KEY2     NRF_GPIO_PIN_MAP(0,13)

#define QCIT_RXD       NRF_GPIO_PIN_MAP(0,8)    //在库文件uart.h和uarte.h里面有关于变量txd和rxd的定义，会报错，故此处设为以QCIT为前缀
#define QCIT_TXD       NRF_GPIO_PIN_MAP(0,6)
//#define QCIT_RXD       NRF_GPIO_PIN_MAP(1,10)  
//#define QCIT_TXD       NRF_GPIO_PIN_MAP(1,11)

#define I2C1_SDA       NRF_GPIO_PIN_MAP(0,13)
#define I2C1_SCL       NRF_GPIO_PIN_MAP(1,9)
#define I2C2_SDA       NRF_GPIO_PIN_MAP(0,26)
#define I2C2_SCL       NRF_GPIO_PIN_MAP(0,4)
//#define INT_WAKE       NRF_GPIO_PIN_MAP(0,15)
#define QSPI_CSN       NRF_GPIO_PIN_MAP(0,17)
#define RST            NRF_GPIO_PIN_MAP(0,18)
#define QSPI_DIO1      NRF_GPIO_PIN_MAP(0,20)
#define QSPI_DIO2      NRF_GPIO_PIN_MAP(0,22)
#define QSPI_DIO3      NRF_GPIO_PIN_MAP(0,24)
#define QSPI_SCK       NRF_GPIO_PIN_MAP(1,0)
#define QSPI_DIO0      NRF_GPIO_PIN_MAP(1,2)

//uwb蓝牙测试板的加速度计引脚
#define SCL_PIN		   NRF_GPIO_PIN_MAP(0,13)	    // SCL signal pin(P1.13)
#define SDA_PIN 	   NRF_GPIO_PIN_MAP(1,9) 	    // SDA signal pin(P1.15)
#define INT_WAKE	   NRF_GPIO_PIN_MAP(0,15) 	    

//DWM1000 spi使用引脚
#define SPI_MISO_PIN       NRF_GPIO_PIN_MAP(0,22)
#define SPI_MOSI_PIN       NRF_GPIO_PIN_MAP(0,24)
#define SPI_SCK_PIN        NRF_GPIO_PIN_MAP(0,20)
#define SPI_SS_PIN         NRF_GPIO_PIN_MAP(0,17)
#define DW1000_WAKEUP      NRF_GPIO_PIN_MAP(0,12)
#define DW1000_IRQ         NRF_GPIO_PIN_MAP(1,4)
#define QCIT_DW1000_RST    NRF_GPIO_PIN_MAP(1,2)
#define EXTON              NRF_GPIO_PIN_MAP(1,0)

//DWM1000 spi使用引脚
#define W5500_INT          NRF_GPIO_PIN_MAP(0,19)
#define W5500_RESET        NRF_GPIO_PIN_MAP(1,5)
#define W5500_LINKLED      NRF_GPIO_PIN_MAP(0,25)
#define SPI2_MISO_PIN      NRF_GPIO_PIN_MAP(0,31)
#define SPI2_MOSI_PIN      NRF_GPIO_PIN_MAP(1,9)
#define SPI2_SCK_PIN       NRF_GPIO_PIN_MAP(0,15)
#define SPI2_NSS_PIN       NRF_GPIO_PIN_MAP(0,13)



//uwb蓝牙测试板的按键引脚
#define KEY 26
#define DC_2V8_EN 18    //使用默认RST引脚做GPIO使用

//5V升压模块使能引脚
#define DC_5V_EN       NRF_GPIO_PIN_MAP(1,5)

//基站板的5V升压模块使能引脚
//#define DC_5V_EN       2

#endif


