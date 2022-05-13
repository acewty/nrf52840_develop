#include <stdint.h>

#include "FReeRTOS.h"
#include "timers.h"
#include "projdefs.h"

#include "app_timer.h"
#include "nrf.h"
#include "nrf_ble_scan.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_timer.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_timer.h"

#include "qcit_ble_drv.h"
#include "qcit_timer_drv.h"
#include "qcit_gpio_drv.h"
#include "qcit_saadc_drv.h" 
#include "qcit_led_drv.h"


#define TIMER_SYS_TICK_OVERFLOW_THRESHOLD    (unsigned long)0xF0FFFFFF
#define LED_BLINK_INTERVAL    1000 //pdMS_TO_TICKS(1000)

bool g_b_app_timer_senconds_cnt_is_running = QCIT_FALSE;       //app_timer_senconds_cnt运行标志
uint8 g_u8_dw1000_timer_timeout_seconds = 1;                   //dw1000的定时超时秒数。默认1s，其他有2s、3s、5s、10s
volatile uint32 g_u32_ms_cnt= 0;                               //硬件定时器实现的ms计数
volatile uint32 g_u32_seconds_cnt= 0;                          //软件定时器实现的秒计数
volatile unsigned long g_u32_sys_tick_cnt = 0;     
volatile unsigned long g_u32_not_received_uwb_tick_cnt = 0;    //记录没接收到uwb的超时
const nrf_drv_timer_t TIMER_DW1000 = NRF_DRV_TIMER_INSTANCE(2);
const nrf_drv_timer_t TIMER_MS = NRF_DRV_TIMER_INSTANCE(3);

extern float g_f_voltage_val;
extern uint8 g_u8_anchor_id[4];

static TimerHandle_t os_app_led_timer;    //led使用定时器    

APP_TIMER_DEF(app_timer_senconds_cnt);            //用于秒计数的软件定时器
//APP_TIMER_DEF(app_timer_ms_cnt);                  //用于毫秒计数的软件定时器


/**************************************************************************
 *函数名称：app_timer_seconds_cnt_timeout_handler
 *函数功能：软件定时器实现秒计时的中断处理函数，每秒计数加1
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年7月29日23:16:43
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
static void app_timer_seconds_cnt_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
	g_u32_seconds_cnt++;
	if(g_u32_seconds_cnt > TIMER_SYS_TICK_OVERFLOW_THRESHOLD)
	{
		g_u32_seconds_cnt = 0;
	}
}

/**************************************************************************
 *函数名称：os_app_led_timer_timeout_handler
 *函数功能：led使用定时器处理函数，周期 LED_BLINK_INTERVAL
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2022年5月13日16:06:50
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
static void os_app_led_timer_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
	nrf_gpio_pin_toggle(LED_NRF52840_BRACELET);
}


/**************************************************************************
 *函数名称：app_timer_seconds_cnt_start
 *函数功能：启动为秒计时创建的软件定时器
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年8月11日22:57:49
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
void app_timer_seconds_cnt_start(void)
{
	ret_code_t err_code;
	if(!g_b_app_timer_senconds_cnt_is_running)
	{
		err_code = app_timer_start(app_timer_senconds_cnt, APP_TIMER_TICKS(1000), NULL);
		APP_ERROR_CHECK(err_code);
		g_b_app_timer_senconds_cnt_is_running = QCIT_TRUE;	
	}
}

/**************************************************************************
 *函数名称：app_timer_seconds_cnt_stop
 *函数功能：停止为秒计时创建的软件定时器
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年8月11日22:58:19
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
void app_timer_seconds_cnt_stop(void)
{
	ret_code_t err_code;
	if(g_b_app_timer_senconds_cnt_is_running)
	{
		err_code = app_timer_stop(app_timer_senconds_cnt);
		APP_ERROR_CHECK(err_code);
		g_b_app_timer_senconds_cnt_is_running = QCIT_FALSE;	
	}
}


/**************************************************************************
 *函数名称：app_timers_init
 *函数功能：初始化时钟
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年7月13日11:43:24
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
void app_timers_init(void)
{
	//初始化软件定时器模块
	uint32 u32_err_code;
    u32_err_code = app_timer_init();
    APP_ERROR_CHECK(u32_err_code);

	u32_err_code = app_timer_create(&app_timer_senconds_cnt, APP_TIMER_MODE_REPEATED, app_timer_seconds_cnt_timeout_handler);
	APP_ERROR_CHECK(u32_err_code);
	
}

/**************************************************************************
 *函数名称：clock_source_xtal
 *函数功能：选择外部晶振作为时钟源
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年7月13日11:10:53
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
void clock_source_xtal(void)
{	
	uint32 u32_err_code;
	//检查clock是否已经初始化
	if(!nrf_drv_clock_init_check())
	{
		u32_err_code = nrf_drv_clock_init();
	}
	APP_ERROR_CHECK(u32_err_code);
	nrf_drv_clock_hfclk_request(NULL);
	//等待时钟源切换为外部32m晶振完成
	while(CLOCK_HFCLKSTAT_SRC_Xtal != nrf_clock_hf_src_get())
	{
	}
}

/**************************************************************************
 *函数名称：clock_init
 *函数功能：初始化时钟模块
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2022年5月13日10:57:09
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
void clock_init(void)
{	
	uint32 u32_err_code;
	u32_err_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(u32_err_code);
}

/**************************************************************************
 *函数名称：os_timers_init
 *函数功能：创建程序使用的相应系统软件定时器
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2022年5月13日10:57:09
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
void os_timers_init(void)
{
    // Initialize timer module.
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    // 创建定时器，优先级由 app_timer_init()中的相应参数定义
    os_app_led_timer = xTimerCreate("LED",
                                   LED_BLINK_INTERVAL,
                                   pdTRUE,
                                   NULL,
                                   os_app_led_timer_timeout_handler);

    /* Error checking */
    if (NULL == os_app_led_timer)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/**************************************************************************
 *函数名称：os_app_led_timer_start
 *函数功能：启动led系统软件定时器
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2022年5月13日17:44:24
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
void os_app_led_timer_start(void)
{
    if (pdPASS != xTimerStart(os_app_led_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}


/**************************************************************************
*函数名称：timer_dw1000_event_handler
*函数功能：dw1000定时器中断事件处理函数
*输入参数：无
*返 回 值：无
*作     者：吴庭永
*创建日期：2021年7月9日11:32:52
*修改历史：Ver1.0，无
**************************************************************************/
static void timer_dw1000_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
		{	
		}
		break;
		
        default:
		{
			//Do nothing. 
		}
		break;   
    }
}

/**************************************************************************
*函数名称：timer_ms_event_handler
*函数功能：ms定时器中断事件处理函数
*输入参数：无
*返 回 值：无
*作     者：吴庭永
*创建日期：2021年7月9日11:32:52
*修改历史：Ver1.0，无
**************************************************************************/
static void timer_ms_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
		{
			g_u32_ms_cnt++;
			if(g_u32_ms_cnt > TIMER_SYS_TICK_OVERFLOW_THRESHOLD)
			{
				g_u32_ms_cnt = 0;
			}	
		}	
		break;

        default:
        {
        	//Do nothing.
        }
		break;
    }
}

/**************************************************************************
*函数名称：timer_dw1000_init
*函数功能：dw1000定位使用硬件定时器初始化
*输入参数：u16_cycle: 定位周期，单位ms
*返 回 值：无
*作     者：吴庭永
*创建日期：2021年7月9日11:09:53
*修改历史：Ver1.0，无
**************************************************************************/
void timer_dw1000_init(uint16 u16_cycle)
{
	uint32 u32_timer_ticks;
	uint32 u32_err_code = NRFX_SUCCESS;
	nrf_drv_timer_uninit(&TIMER_DW1000);	//确保定时器未初始化
	
	nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
	u32_err_code = nrf_drv_timer_init(&TIMER_DW1000, &timer_cfg, timer_dw1000_event_handler);
	APP_ERROR_CHECK(u32_err_code);
	//将毫秒数转换为ticks
	u32_timer_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_DW1000, u16_cycle);

	nrf_drv_timer_extended_compare(
	     &TIMER_DW1000, NRF_TIMER_CC_CHANNEL0, u32_timer_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
	
}


/**************************************************************************
*函数名称：timer_ms_init
*函数功能：ms使用硬件定时器初始化
*输入参数：无
*返 回 值：无
*作     者：吴庭永
*创建日期：2021年7月12日12:11:48
*修改历史：Ver1.0，无
**************************************************************************/
void timer_ms_init(void)
{
	uint32 u32_timer_ticks;
	uint32 u32_err_code = NRFX_SUCCESS;
	nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
	//timer_cfg.interrupt_priority = 3;
	u32_err_code = nrf_drv_timer_init(&TIMER_MS, &timer_cfg, timer_ms_event_handler);
	APP_ERROR_CHECK(u32_err_code);
	//将毫秒数转换为ticks
	u32_timer_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_MS, 1);

	nrf_drv_timer_extended_compare(
	     &TIMER_MS, NRF_TIMER_CC_CHANNEL0, u32_timer_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
}

/**************************************************************************
*函数名称：timer_dw1000_start
*函数功能：dw1000定位使用硬件定时器开始
*输入参数：无
*返 回 值：无
*作     者：吴庭永
*创建日期：2021年7月9日11:09:53
*修改历史：Ver1.0，无
**************************************************************************/
void timer_dw1000_start(void)
{
	nrf_drv_timer_enable(&TIMER_DW1000);
}

/**************************************************************************
*函数名称：timer_ms_start
*函数功能：ms计数硬件定时器打开
*输入参数：无
*返 回 值：无
*作     者：吴庭永
*创建日期：2021年7月12日12:30:44
*修改历史：Ver1.0，无
**************************************************************************/
void timer_ms_start(void)
{
	nrf_drv_timer_enable(&TIMER_MS);
}

/**************************************************************************
*函数名称：timer_ms_stop
*函数功能：ms计数硬件定时器关闭
*输入参数：无
*返 回 值：无
*作     者：吴庭永
*创建日期：2021年7月29日13:36:39
*修改历史：Ver1.0，无
**************************************************************************/
void timer_ms_stop(void)
{
	nrf_drv_timer_disable(&TIMER_MS);
}

/**************************************************************************
*函数名称：timer_dw1000_stop
*函数功能：dw1000定位使用硬件定时器停止
*输入参数：无
*返 回 值：无
*作     者：吴庭永
*创建日期：2021年7月12日10:35:48
*修改历史：Ver1.0，无
**************************************************************************/
void  timer_dw1000_stop(void)
{
	nrf_drv_timer_disable(&TIMER_DW1000);
}

/**************************************************************************
 *函数名称：get_rtc1_counter
 *函数功能：获取rtc1计数值
 *输入参数：无
 *返 回 值：rtc1的counter寄存器的值
 *作	    者：吴庭永
 *创建日期：2021年4月7日14:04:073
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
uint32 get_rtc1_counter(void)
{
    return NRF_RTC1->COUNTER;
}

/**************************************************************************
 *函数名称：timer_ms_delay
 *函数功能：实现ms延时，使用timer3实现了ms中断
 *输入参数：u32_ms_delay：延时ms数量
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年7月14日15:00:13
 *修改历史：Ver1.0，无
 *
 **************************************************************************/ 
void timer_ms_delay(uint32 u32_ms_delay)
{
	uint32 g_u32_ms_cnt_current = g_u32_ms_cnt;
#if 0
	//如果延时参数时间超过30s，则认为参数出错
	if(u32_ms_delay > 30000)
	{
		return QCIT_FAIL;
	}
#endif
	while(g_u32_ms_cnt - g_u32_ms_cnt_current < u32_ms_delay)
	{
	}
}

