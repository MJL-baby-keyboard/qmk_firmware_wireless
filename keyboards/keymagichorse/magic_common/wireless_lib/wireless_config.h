/* Copyright 2025 keymagichorse
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include "config.h"
// 无线相关的配置宏定义



// ****************************************** keymap.h and wireless_config.h *****************
// 切换输出通道（蓝牙 / 2.4G / USB）
// 切换到蓝牙输出，并开启广播（非配对类型） 0x7786
#ifndef BLE_TOG 
#   define BLE_TOG OU_BT       
#endif
// 切换到 2.4GHz 输出 0x7785 
#ifndef RF_TOG
#   define RF_TOG OU_2P4G     
#endif
// 切换到 USB 输出 0x7784 
#ifndef USB_TOG
#   define USB_TOG OU_USB     
#endif

// 蓝牙通道控制（短按开启广播，长按进入配对广播）
// 蓝牙通道 0 0x7793
#ifndef BL_SW0
#   define BL_SW0 BT_PRF1  
#endif
// 蓝牙通道 1 0x7794
#ifndef BL_SW1
#   define BL_SW1 BT_PRF2      
#endif
// 蓝牙通道 2 0x7795
#ifndef BL_SW2
#   define BL_SW2 BT_PRF3    
#endif

// 蓝牙控制功能
// 蓝牙复位
#ifndef BLE_RESET
#   define BLE_RESET    QK_USER_28      
#endif
 // 关闭蓝牙连接
#ifndef BLE_OFF
#   define BLE_OFF      QK_USER_29       
#endif
// ****************************************** keymap.h and wireless_config.h *****************


// ****************************************** lpm.h ******************************************
// 低功耗相关
#ifndef LPM_QMK_SLEEP_S
#    define LPM_QMK_SLEEP_S 	10
#    define LPM_QMK_SLEEP_MS 	LPM_QMK_SLEEP_S * 1000
#endif

// STM32使用到的高速晶振引脚号，做低功耗需要用户配置，每款芯片有可能不一样的
#if STM32_HSE_ENABLED
// F4 F1都是H1H0
#	ifndef LPM_HSE_PIN_IN
#    error "LPM_HSE_PIN_IN is not defined optional[H1]"
#	endif

#	ifndef LPM_HSE_PIN_OUT
#    error "LPM_HSE_PIN_OUT is not defined optional[H0]"
#	endif
#endif
// ****************************************** lpm.h ******************************************

// ****************************************** wireless.h *************************************
// USB电源检测引脚
#ifndef USB_POWER_SENSE_PIN
#    define USB_POWER_SENSE_PIN         A10
#endif
// USB 插入后的 电平
#ifndef USB_POWER_CONNECTED_LEVEL
#    define USB_POWER_CONNECTED_LEVEL         1
#endif 
// ****************************************** wireless.h *************************************

// ****************************************** battery.h **************************************
// 电池电压最高最低 mv
#ifndef BATTERY_MAX_MV                       
#    define BATTERY_MAX_MV     4150
#endif
#ifndef BATTERY_MIN_MV                      
#    define BATTERY_MIN_MV     3300
#endif

//  电池分压电阻的配置 
/* Battery voltage resistive voltage divider setting of MCU */
#ifndef BAT_R_UPPER                        
// Upper side resitor value (uint: KΩ)
#   define BAT_R_UPPER 100  
#endif
#ifndef BAT_R_LOWER    
 // Lower side resitor value (uint: KΩ)                   
#   define BAT_R_LOWER 100         
#endif

//  电池电压读取的引脚 
#ifndef BATTERY_ADC_PIN                       
#    error "BATTERY NEED BATTERY_ADC_PIN is not defined"
#endif
// https://docs.qmk.fm/drivers/adc#stm32
#ifndef BATTERY_ADC_DRIVER                      
#    error "BATTERY NEED BATTERY_ADC_DRIVER is not defined"
#endif
// ****************************************** battery.h **************************************

// ****************************************** report_buffer.c ********************************

// 这个宏定义是用于填充到无线缓冲区的，无线模式下按下一个按键就会将一个数据包填充到 report_buffer.c
/* The report buffer is mainly used to fix key press lost issue of macro
 * when bluetooth module fifo isn't large enough. The maximun macro
 * string length is determined by this queue size, and should be
 * WIRELESS_REPORT_BUFFER_QUEUE_SIZE devided by 2 since each character is implemented
 * by sending a key pressing then a key releasing report.
 * Please note that it cosume sizeof(report_buffer_t)  * WIRELESS_REPORT_BUFFER_QUEUE_SIZE
 * bytes RAM, with default setting, used RAM size is
 *        sizeof(report_buffer_t) * 256 = 34* 256  =  8704 bytes
 */
// Very few codes are borrowed from https://www.keychron.com
// 启用 蓝牙BHQ 时 需要 配置 缓冲区大小
#if defined(BLUETOOTH_BHQ)
#	ifndef WIRELESS_REPORT_BUFFER_QUEUE_SIZE
#	    define WIRELESS_REPORT_BUFFER_QUEUE_SIZE 64
#	endif

#	if !((WIRELESS_REPORT_BUFFER_QUEUE_SIZE == 8)   || \
      (WIRELESS_REPORT_BUFFER_QUEUE_SIZE == 16)  || \
      (WIRELESS_REPORT_BUFFER_QUEUE_SIZE == 32)  || \
      (WIRELESS_REPORT_BUFFER_QUEUE_SIZE == 64)  || \
      (WIRELESS_REPORT_BUFFER_QUEUE_SIZE == 128) || \
      (WIRELESS_REPORT_BUFFER_QUEUE_SIZE == 256) || \
      (WIRELESS_REPORT_BUFFER_QUEUE_SIZE == 512))
#		error "WIRELESS_REPORT_BUFFER_QUEUE_SIZE optional[8, 16, 32, 64, 128, 256, 512]"
#	endif

#endif
// ****************************************** report_buffer.c ********************************

// ****************************************** bhq.h and uart.h *******************************
// BHQ 中断引脚配置
#ifndef BHQ_IQR_PIN
#    error "BHQ WIRELESS NEED BHQ_IQR_PIN is not defined"
#endif
#ifndef BHQ_INT_PIN
#    error "BHQ WIRELESS NEED BHQ_INT_PIN is not defined"
#endif

// 这个不能改
#define BHQ_RUN_OR_INT_LEVEL       1                


// UART驱动配置
#ifndef BHQ_UART_DRIVER
#    error "BHQ WIRELESS NEED UART_DRIVER is not defined optional[SD1 SD2]"
#else
#    define UART_DRIVER BHQ_UART_DRIVER
#endif

// UART TX引脚配置
#ifndef BHQ_UART_TX_PIN
#    error "BHQ_UART_TX_PIN is not defined"
#else
#    define UART_TX_PIN BHQ_UART_TX_PIN
#endif

#ifndef BHQ_UART_TX_PAL_MODE
#    error "BHQ_UART_TX_PAL_MODE is not defined"
#else
#    define UART_TX_PAL_MODE BHQ_UART_TX_PAL_MODE
#endif

// UART RX引脚配置
#ifndef BHQ_UART_RX_PIN
#    error "BHQ_UART_RX_PIN is not defined"
#else
#    define UART_RX_PIN BHQ_UART_RX_PIN
#endif

#ifndef BHQ_UART_RX_PAL_MODE
#    error "BHQ_UART_RX_PAL_MODE is not defined"
#else
#    define UART_RX_PAL_MODE BHQ_UART_RX_PAL_MODE
#endif
// ****************************************** bhq.h and uart.h *******************************