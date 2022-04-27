/************************************************************************
 *
 *Copyright 2021 (c) 青岛联合创智科技有限公司
 *           All rights reserved.
 *
 *文件名称: qcit_ble_drv.c
 *文件功能: BLE驱动
 *作     者: 吴庭永
 *编写日期: 2021年3月20日16:57:05
 *修改历史: 无
 *
 ************************************************************************/
#include <sdk_errors.h>
#include "app_error.h"
#include "app_timer.h"
#include "nrf_ble_scan.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"

#include "qcit_ble_drv.h"
#include "qcit_gpio_drv.h"
#include "qcit_timer_drv.h"
#include "qcit_types.h"

#define APP_BLE_OBSERVER_PRIO           3        /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define SCAN_INTERVAL                   1600     //以0.625ms为单位，最小为4(2.5ms)，此处设置为1000ms 
#define SCAN_INTERVAL_SLEEP             4800     //休眠扫描间隔，此处设置为3000ms 
#define SCAN_WINDOW                     1600     //以0.625ms为单位，最小为4(2.5ms)，此处设置为1000ms
#define SCAN_DURATION                   1000     //以10ms为单位，最小为10ms，此处设置为10s
#define ADV_TDMA_FRAME_HEADER_0         7        //广播报告包里的TDMA帧头0
#define ADV_TDMA_FRAME_HEADER_1         8        //广播报告包里的TDMA帧头1
#define ADV_TDMA_FRAME_HEADER_2         9        //广播报告包里的TDMA帧头2
#define ADV_TDMA_FRAME_HEADER_3         10       //广播报告包里的TDMA帧头3
#define ADV_TDMA_FRAME_VALID_DATA_LEN   16       //广播TDMA帧的有效数据（去掉帧头TDMA占16个字节）
#define BLE_INTERVAL_NUM                5        //扫描的蓝牙广播包间隔的数量

int8 g_i8_ble_rssi = 0;                          //扫描到的蓝牙功率值       
uint8 g_u8_ble_scanned_times = 0;                //扫描到的蓝牙次数
//uint16 g_u16_ble_interval[BLE_INTERVAL_NUM] = 0; //扫描的蓝牙广播包间隔
static uint8_t m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_MIN];    //用于存储协议栈的广播报告的缓冲区

//指向存储协议栈的广播报告的缓冲区
static ble_data_t m_scan_buffer =
{
    m_scan_buffer_data,
    BLE_GAP_SCAN_BUFFER_MIN
};

bool g_b_scanned_tdma = QCIT_FALSE;                            //扫描到tdma广播标志
uint8 adv_tdma_data[ADV_TDMA_FRAME_VALID_DATA_LEN] ={0};       //广播获取的tdma数据
#if 1
uint32 g_u32_tick_new = 0;
uint32 g_u32_tick_old = 0;
uint32 g_u32_time_ble_first = 0;        //扫描到第一个广播包的时间
uint32 g_u32_time_ble_interval = 0;     //第一次扫描到所需的最后一个广播包计算与第一包的间隔

#endif

extern volatile unsigned long g_u32_sys_tick_cnt;
extern volatile uint32 g_u32_ms_cnt;


NRF_BLE_SCAN_DEF(m_scan);                                       				/**< Scanning module instance. */

/**************************************************************************
*函数名称：ble_stack_init
*函数功能：初始化蓝牙堆栈
*输入参数：无
*返 回 值：无
*作	   者：吴庭永
*创建日期：2021年3月20日16:50:08
*修改历史：Ver1.0，无
**************************************************************************/
void ble_stack_init(void)
{
    uint32 u32_err_code = 0;
	uint32_t u32_ram_start = 0;
	
    u32_err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(u32_err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    
    u32_err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &u32_ram_start);
    APP_ERROR_CHECK(u32_err_code);

    // Enable BLE stack.
    u32_err_code = nrf_sdh_ble_enable(&u32_ram_start);
    APP_ERROR_CHECK(u32_err_code);
}

/**************************************************************************
 *函数名称：scan_evt_handler
 *函数功能：蓝牙扫描事件的处理函数
 *输入参数：p_scan_evt，扫描事件
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年3月20日14:11:45
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
static void scan_evt_handler(scan_evt_t const * p_scan_evt)
{
    uint32 u32_err_code = 0;

    switch(p_scan_evt->scan_evt_id)
    {
        case NRF_BLE_SCAN_EVT_CONNECTING_ERROR:
            u32_err_code = p_scan_evt->params.connecting_err.err_code;
            APP_ERROR_CHECK(u32_err_code);
            break;
        default:
          break;
    }
}

/**************************************************************************
 *函数名称：ble_scan_init_and_start
 *函数功能：蓝牙扫描初始化并开始运行，与从休眠唤醒情况一致
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年3月19日20:05:40
 *修改历史：Ver1.0，无
 *          Ver1.1，吴庭永，2021年10月9日15:01:22
 *                （1）修改函数名称
 *
 **************************************************************************/
void ble_scan_init_and_start(void)
{
    uint32          u32_err_code = 0;
    nrf_ble_scan_init_t t_init_scan;
#if 1
	static ble_gap_scan_params_t const scan_params = 
	{
		.active = 1,
		.interval = SCAN_INTERVAL_SLEEP,
		.window = SCAN_WINDOW,
		.timeout = 0,
		.scan_phys = BLE_GAP_PHY_CODED,
		.filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
		.extended = 1
	};

#endif
    memset(&t_init_scan, 0, sizeof(t_init_scan));
	t_init_scan.p_scan_param = &scan_params;
	
    u32_err_code =  nrf_ble_scan_init(&m_scan, &t_init_scan, scan_evt_handler);
    APP_ERROR_CHECK(u32_err_code);

	u32_err_code =  nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(u32_err_code);

}

/**************************************************************************
 *函数名称：ble_scan_in_sleep_mode
 *函数功能：休眠后配置新的蓝牙扫描参数并开始扫描
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年7月29日20:55:00
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
void ble_scan_in_sleep_mode(void)
{
    uint32          u32_err_code = 0;
    nrf_ble_scan_init_t t_init_scan;
	static ble_gap_scan_params_t const scan_params = 
	{
		.active = 1,
		.interval = SCAN_INTERVAL_SLEEP,
		.window = SCAN_WINDOW,
		.timeout = 0,
		.scan_phys = BLE_GAP_PHY_CODED,
		.filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
		.extended = 1
	};
    memset(&t_init_scan, 0, sizeof(t_init_scan));
	t_init_scan.p_scan_param = &scan_params;

	nrf_ble_scan_stop();
	
    u32_err_code =  nrf_ble_scan_init(&m_scan, &t_init_scan, scan_evt_handler);
    APP_ERROR_CHECK(u32_err_code);
	
	u32_err_code = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(u32_err_code);

}

/**************************************************************************
 *函数名称：ble_scan_in_uwb_refresh
 *函数功能：uwb发送校正过程的蓝牙扫描
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年7月29日20:55:00
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
void ble_scan_in_uwb_refresh(void)
{
    uint32          u32_err_code = 0;
    nrf_ble_scan_init_t t_init_scan;
	static ble_gap_scan_params_t const scan_params = 
	{
		.active = 1,
		.interval = SCAN_INTERVAL ,
		.window = SCAN_WINDOW,
		.timeout = SCAN_DURATION,
		.scan_phys = BLE_GAP_PHY_CODED,
		.filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
		.extended = 1
	};
    memset(&t_init_scan, 0, sizeof(t_init_scan));
	t_init_scan.p_scan_param = &scan_params;

	nrf_ble_scan_stop();
	
    u32_err_code =  nrf_ble_scan_init(&m_scan, &t_init_scan, scan_evt_handler);
    APP_ERROR_CHECK(u32_err_code);
	
	u32_err_code = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(u32_err_code);

}


/**************************************************************************
 *函数名称：ble_scan_start
 *函数功能：蓝牙扫描开始
 *输入参数：无
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年3月20日14:08:44
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
void ble_scan_start(void)
{
    uint32 u32_err_code;

    u32_err_code = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(u32_err_code);
	
	//u32_err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_SCAN_INIT, BLE_CONN_HANDLE_INVALID, 8);
	//APP_ERROR_CHECK(u32_err_code);
	
}


/**************************************************************************
 *函数名称：ble_evt_handler
 *函数功能：蓝牙事件的处理函数
 *输入参数：p_ble_evt，蓝牙事件；p_context，没有使用
 *返 回 值：无
 *作	    者：吴庭永
 *创建日期：2021年3月20日16:34:45
 *修改历史：Ver1.0，无
 *
 **************************************************************************/
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
	ble_gap_evt_adv_report_t const * p_adv_report = &p_ble_evt->evt.gap_evt.params.adv_report;
	switch (p_ble_evt->header.evt_id)
	{
		 case BLE_GAP_EVT_ADV_REPORT:
			/**/if(('T' == (char)p_adv_report->data.p_data[ADV_TDMA_FRAME_HEADER_0]) &&
				('D' == (char)p_adv_report->data.p_data[ADV_TDMA_FRAME_HEADER_1]) &&
				('M' == (char)p_adv_report->data.p_data[ADV_TDMA_FRAME_HEADER_2]) && 
				('A' == (char)p_adv_report->data.p_data[ADV_TDMA_FRAME_HEADER_3]))
			//if(1)
			{
				g_i8_ble_rssi = p_adv_report->rssi;
				//NRF_LOG_INFO("RSSI: %d",p_adv_report->rssi);
				//NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len);
#if 0
				g_u32_tick_old = g_u32_tick_new;
				g_u32_tick_new = g_u32_ms_cnt;
				NRF_LOG_INFO("%d",g_u32_tick_new - g_u32_tick_old);
				//NRF_LOG_FLUSH();
#endif			
			}
			break;
		default:
			// No implementation needed.
			break;
	}
}

// Register a handler for BLE events.
NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);



