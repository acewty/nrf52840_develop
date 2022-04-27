/************************************************************************
 *
 *Copyright 2021 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_spi_drv.c
 *文件功能: spi操作相关接口
 *作     者: 李彦
 *编写日期: 2020年7月9日09:12:37
 *修改历史: 无
 *
 ************************************************************************/
#include <string.h>

#include "nrf_delay.h"
#include "nrf_drv_spi.h"
#include "nrf_gpio.h"

#include "qcit_gpio_drv.h"
#include "qcit_spi_drv.h"
#include "qcit_types.h"

#define SPI_INSTANCE            2                                           //SPI instance index./

static const nrf_drv_spi_t s_t_spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);    //SPI instance.
static volatile bool s_b_spi_xfer_done;                                     //SPI 触发中断事件的时候会被置位

/**************************************************************************
*函数名称：spi_event_handler
*函数功能：spi中断服务事件
*输入参数：p_event          p_context
*返 回 值：无
*作	   者：李彦
*创建日期：2020年10月29日14:34:07
*修改历史：Ver1.0，无
**************************************************************************/
void spi_event_handler(nrf_drv_spi_evt_t const *p_event, void *p_context)
{
	s_b_spi_xfer_done = true;
}

/**************************************************************************
*函数名称：spi_init
*函数功能：spi初始化
*输入参数：无
*返 回 值：无
*作	   者：李彦
*创建日期：2020年10月29日14:29:42
*修改历史：Ver1.0，无
*
*版	本：Ver1.1
*修改作者：臧强
*修改日期：2020年12月5日
*修改内容：(1)去掉片选初始化，修改模式配置
**************************************************************************/
void spi_init (void)
{
	nrf_drv_spi_config_t t_spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	
	//MISO和MOSI相对于DWM1000的定义
	//t_spi_config.irq_priority = 2;
	t_spi_config.miso_pin = SPI_MISO_PIN;
	t_spi_config.mosi_pin = SPI_MOSI_PIN; 
	t_spi_config.sck_pin  = SPI_SCK_PIN;
	t_spi_config.mode = NRF_DRV_SPI_MODE_0;
	t_spi_config.frequency = NRF_DRV_SPI_FREQ_8M;
	APP_ERROR_CHECK(nrf_drv_spi_init(&s_t_spi, &t_spi_config, spi_event_handler, NULL));

	//SPI 片选IO初始化
	nrf_gpio_cfg_output(SPI_SS_PIN);
	nrf_gpio_pin_set(SPI_SS_PIN);
}
/**************************************************************************
*函数名称：spi_uninit
*函数功能：取消spi初始化
*输入参数：无
*返 回 值：无
*作	   者：吴庭永
*创建日期：2021年3月18日15:15:13
*修改历史：Ver1.0，无
**************************************************************************/
void spi_uninit (void)
{
	nrf_drv_spi_uninit(&s_t_spi);
}

//uint32 g_u32_spi_cnt;
uint32 g_u32_ret_code;
/**************************************************************************
*函数名称：spi_transfer
*函数功能：spi启动 
*输入参数：*p_tx_buffer,tx_buffer_length,*p_rx_buffer,rx_buffer_length
*返 回 值：无
*作	   者：李彦
*创建日期：2020年10月29日14:51:31
*修改历史：Ver1.0，无
**************************************************************************/
void spi_transfer(uint8 const *p_tx_buffer,uint8 tx_buffer_length,uint8 *p_rx_buffer,uint8 rx_buffer_length)
{
	//uint16 u16_cnt = 0;
	s_b_spi_xfer_done = false;

	g_u32_ret_code = nrf_drv_spi_transfer(&s_t_spi, p_tx_buffer, tx_buffer_length, p_rx_buffer, rx_buffer_length);
	APP_ERROR_CHECK(g_u32_ret_code);

	while (!s_b_spi_xfer_done)
	{
#if 0
		u16_cnt++;
		//g_u32_spi_cnt = u16_cnt;
		if(u16_cnt > 600)
		{
			break;
		}
#endif
		__WFE();
	}
}

/**************************************************************************
 *函数名称：spi_cs_set_low
 *函数功能：spi片选置低
 *输入参数：无
 *返 回 值：无
 *作		者：李彦
 *创建日期：2020年11月13日14:50:48
 *修改历史：Ver1.0，无
 **************************************************************************/
void spi_cs_set_low(void)
{
	nrf_gpio_pin_clear(SPI_SS_PIN);
}

/**************************************************************************
 *函数名称：spi_cs_set_high
 *函数功能：spi片选置高
 *输入参数：无
 *返 回 值：无
 *作	   者：李彦
 *创建日期：2020年11月13日14:50:48
 *修改历史：Ver1.0，无
 **************************************************************************/
void spi_cs_set_high(void)
{
	nrf_gpio_pin_set(SPI_SS_PIN);
}

/**************************************************************************
 *函数名称：spi_set_rate_low
 *函数功能：spi设置为低速2M
 *输入参数：无
 *返 回 值：0
 *作	   者：李彦
 *创建日期：2020年10月30日16:44:21
 *修改历史：Ver1.0，无
 *
 *版    本：Ver1.1
 *修改作者：臧强
 *修改日期：2020年12月5日
 *修改内容：(1)去掉片选初始化，修改模式配置
 **************************************************************************/
void spi_set_rate_low (void)
{
	nrf_drv_spi_uninit(&s_t_spi);

	nrf_drv_spi_config_t t_spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	t_spi_config.miso_pin = SPI_MISO_PIN;
	t_spi_config.mosi_pin = SPI_MOSI_PIN;
	t_spi_config.sck_pin  = SPI_SCK_PIN;
	t_spi_config.mode	= NRF_DRV_SPI_MODE_0;
	t_spi_config.frequency  = NRF_DRV_SPI_FREQ_2M;
	APP_ERROR_CHECK(nrf_drv_spi_init(&s_t_spi, &t_spi_config, spi_event_handler, NULL));
	nrf_delay_us(100);
}

/**************************************************************************
 *函数名称：spi_set_rate_high
 *函数功能：spi设置为高速，8M
 *输入参数：无
 *返 回 值：0
 *作	   者：李彦
 *创建日期：2020年10月30日16:44:21
 *修改历史：Ver1.0，无
 *
 *版    本：Ver1.1
 *修改作者：臧强
 *修改日期：2020年12月5日
 *修改内容：(1)去掉片选初始化，修改模式配置
 **************************************************************************/
void spi_set_rate_high (void)
{
	nrf_drv_spi_uninit(&s_t_spi);

	nrf_drv_spi_config_t t_spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	t_spi_config.miso_pin = SPI_MISO_PIN;
	t_spi_config.mosi_pin = SPI_MOSI_PIN;
	t_spi_config.sck_pin  = SPI_SCK_PIN;
	t_spi_config.mode	= NRF_DRV_SPI_MODE_0;
	t_spi_config.frequency  = NRF_DRV_SPI_FREQ_8M;
	APP_ERROR_CHECK(nrf_drv_spi_init(&s_t_spi, &t_spi_config, spi_event_handler, NULL));
	nrf_delay_us(100);
}


