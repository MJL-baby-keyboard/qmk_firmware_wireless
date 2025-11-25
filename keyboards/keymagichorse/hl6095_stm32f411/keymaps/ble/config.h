/* Copyright 2024 keymagichorse
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



#ifndef SERIAL_NUMBER
#   define SERIAL_NUMBER "mjl_hl6095"
#endif

#ifdef BLUETOOTH_BHQ
#   define BHQ_IQR_PIN          A1             
#   define BHQ_INT_PIN          A0             

#   define BHQ_UART_DRIVER          SD2
#   define BHQ_UART_TX_PIN          A2
#   define BHQ_UART_TX_PAL_MODE     7
#   define BHQ_UART_RX_PIN          A3
#   define BHQ_UART_RX_PAL_MODE	    7

// STM32使用到的高速晶振引脚号，做低功耗需要用户配置，每款芯片有可能不一样的
#	define LPM_HSE_PIN_IN     H1  
#	define LPM_HSE_PIN_OUT    H0

#   define USB_POWER_SENSE_PIN  		A10		// USB插入检测引脚 
#	define USB_POWER_CONNECTED_LEVEL	1 		// USB插入后的电平

// 有线 转 无线 按键填入缓冲区 缓冲器的大小
#	define WIRELESS_REPORT_BUFFER_QUEUE_SIZE    64

#	define BATTERY_ADC_PIN              B1
#	define BATTERY_ADC_DRIVER           ADCD1


#	include "wireless_config.h"

#endif

