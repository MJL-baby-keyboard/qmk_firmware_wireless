/* Copyright 2022~2024 @ lokher (https://www.keychron.com)
 * Adapted from  keymagichorse
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
// 这里处理正常休眠 低电量休眠
#include "quantum.h"
#include "usb_util.h"
#include <usb_main.h>
#include "debounce.h"

#include "matrix_sleep.h"
#include "lpm.h"
#include "battery.h"
#include "bluetooth.h"
#include "wireless.h"
#include "wireless_config.h"
#include "report_buffer.h"
#include "bhq.h"

static uint32_t     lpm_timer_buffer = 0;
static bool         lpm_time_up               = false;
uint8_t lpm_battery_status = 0; // 0:正常, 1:10%低电量, 2:5%严重低电量


__attribute__((weak)) void lpm_device_power_open(void){}
__attribute__((weak)) void lpm_device_power_close(void){}


__attribute__((weak)) void lpm_set_unused_pins_to_input_analog(void){}

__attribute__((weak)) void lpm_mcu_wakeup_prepare(void){}
__attribute__((weak)) void lpm_mcu_enter_stop_mode(void){}



// low power mode
void lpm_init(void)
{
    lpm_battery_status = 0;
}

void lpm_timer_reset(void) {
    lpm_time_up      = false;
    lpm_timer_buffer = 0;
    lpm_battery_status = 0;
}

// 低电量回调函数
__attribute__((weak)) void lpm_low_battery_handle_kb(void){}
// 低电量检测任务
void lpm_low_battery_check_task(void)
{
    
    if (usb_power_connected()) {
        if (lpm_battery_status != 0) {
            lpm_battery_status = 0;
            bluetooth_enabled();  // USB供电时确保蓝牙开启
        }
        return;
    }

    uint8_t battery_percent = battery_percent_get();
    
    if (battery_percent <= 5) {
        if (lpm_battery_status != 2) {
            lpm_battery_status = 2;
            lpm_low_battery_handle_kb(); // 5%严重低电量
            bluetooth_disabled();         // 严重低电量时禁用蓝牙
        }
    } 
    else if (battery_percent <= 10) {
        if (lpm_battery_status != 1) {
            lpm_battery_status = 1;
            lpm_low_battery_handle_kb(); // 10%低电量
        }
    } 
    else {
        if (lpm_battery_status != 0) {
            lpm_battery_status = 0; // 电量恢复正常
            bluetooth_enabled(); // 确保蓝牙开启
        }
    }
}


void lpm_sleep_config(void)
{
// 配置矩阵io口  
    lpm_set_unused_pins_to_input_analog();    // 设置没有使用的引脚为模拟输入
    matrix_sleepConfig();
// usb 插入检测中断
    gpio_set_pin_input(USB_POWER_SENSE_PIN);
    palEnableLineEvent(USB_POWER_SENSE_PIN, PAL_EVENT_MODE_RISING_EDGE);
    /* Usb unit is actived and running, stop and disconnect first */
    
// 配置BHQ
    gpio_set_pin_input_low(BHQ_IQR_PIN);
    palEnableLineEvent(BHQ_IQR_PIN, PAL_EVENT_MODE_RISING_EDGE);
    gpio_write_pin_low(BHQ_INT_PIN);
    sdStop(&UART_DRIVER);
    palSetLineMode(UART_TX_PIN, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(UART_RX_PIN, PAL_MODE_INPUT_ANALOG);
    bhq_Disable();
// 禁用usb
    usbStop(&USBD1);
    usbDisconnectBus(&USBD1);

    lpm_device_power_close();    // 外围设备 电源 关闭

    lpm_mcu_enter_stop_mode();  // 进入mcu停止模式
}
void lpm_wakeup_config(void)
{
    lpm_mcu_wakeup_prepare();   // mcu唤醒后

    if (usb_power_connected()) {
        usb_event_queue_init();
        init_usb_driver(&USBD1);
    }
 
    /* Call debounce_free() to avoiding memory leak of debounce_counters as debounce_init()
    invoked in matrix_init() alloc new memory to debounce_counters */
    debounce_free();
    matrix_init();

    lpm_timer_reset();
    report_buffer_init();
    bhq_init();     // uart_init

    clear_keyboard();
    layer_clear();

    lpm_device_power_open();    // 外围设备 电源 打开
  
    gpio_write_pin_high(BHQ_INT_PIN);

}

void lpm_task(void)
{
    if (usb_power_connected() && USBD1.state == USB_STOP) {
        usb_event_queue_init();
        init_usb_driver(&USBD1);
    }


    if(report_buffer_is_empty() == false)
    {
        lpm_time_up = false;
        lpm_timer_buffer = 0;
        return;
    }
    lpm_low_battery_check_task();

    if(lpm_time_up == false && lpm_timer_buffer == 0)
    {
        lpm_time_up = true;
        lpm_timer_buffer = sync_timer_read32();
    }

    if (lpm_time_up == true && sync_timer_elapsed32(lpm_timer_buffer) > LPM_QMK_SLEEP_MS) {
        lpm_time_up = false;
        lpm_timer_buffer = 0;
        lpm_sleep_config();
        lpm_wakeup_config();
    }
}