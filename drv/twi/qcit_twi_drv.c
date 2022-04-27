/************************************************************************
 *
 *Copyright 2020 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_twi_drv.c
 *文件功能: twi函数接口
 *作	   者：吴庭永
 *创建日期：2020-3-23 11:21:48
 *修改历史：Ver1.0，无
 *
 ************************************************************************/

#include "qcit_gpio_drv.h"
#include "nrf_gpio.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

#include "qcit_twi_drv.h"
#include "qcit_lis2dh12_drv.h"


/* TWI instance ID. */
#define TWI_INSTANCE_ID     0

/* Indicates if operation on TWI has ended.*/
static volatile bool m_xfer_done = false;   // twi 操作结束标志

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

	
/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
	switch (p_event->type)
	{
		case NRF_DRV_TWI_EVT_DONE:
		{
            //if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
			{
			}
            m_xfer_done = true;
		}
		break;
		default:
		{
		}
		break;
    }
}

/**************************************************************************
 *函数名称：twi_init
 *函数功能：初始化twi
 *输入参数：无
 *返 回 值：无
 *作     者：吴庭永
 *创建日期：2021年2月3日10:35:58
 *修改历史：Ver1.0，无
 **************************************************************************/
void twi_init (void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
       .scl                = SCL_PIN,	
       .sda                = SDA_PIN,	//ARDUINO_SDA_PIN
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false  //清除总线上的数据进行初始化
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}



/**************************************************************************
 *函数名称：twi_write_onebyte
 *函数功能：往twi设备写一个字节
 *输入参数：twi_sad_write_addr,reg_addr,data
 *返 回 值：无
 *作     者：吴庭永
 *创建日期：2020-3-23 11:32:52
 *修改历史：Ver1.0，无
 **************************************************************************/
void twi_write_onebyte(uint32 twi_sad_write_addr,uint8 u8_reg_addr, uint8 data)
{
	ret_code_t u32_err_code;
	uint8 u8_reg[2] = {u8_reg_addr, data}; 

	m_xfer_done = false;
	u32_err_code = nrf_drv_twi_tx(&m_twi, twi_sad_write_addr, u8_reg, 2, true);
	APP_ERROR_CHECK(u32_err_code);
	
	while(m_xfer_done == false)
	{
	}
}

/**************************************************************************
 *函数名称：twi_read_onebyte
 *函数功能：从twi设备读一个字节
 *输入参数：twi_sad_write_addr, twi_sad_read_addr, reg_addr, data
 *返 回 值：无
 *作   者： 吴庭永
 *创建日期：2020-3-23 11:32:52
 *修改历史：Ver1.0，无
 **************************************************************************/

void twi_read_onebyte(uint32 twi_sad_write_addr, uint32 twi_sad_read_addr, uint8_t reg_addr, uint8_t *data)
{
	ret_code_t err_code;
	
	m_xfer_done = false;
	err_code = nrf_drv_twi_tx(&m_twi, twi_sad_write_addr, &reg_addr, 1, true);	//&reg_addr这个位置定义的是一个地址指针，所以参数一定要是指针，reg_addr加上&就变成了一个指针
	APP_ERROR_CHECK(err_code);
	while (m_xfer_done == false);
	
	m_xfer_done = false;
	err_code = nrf_drv_twi_rx(&m_twi, twi_sad_read_addr, data, 1);
	APP_ERROR_CHECK(err_code);
	while (m_xfer_done == false)
	{
	}
}