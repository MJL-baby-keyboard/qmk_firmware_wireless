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
#include QMK_KEYBOARD_H
#include "config.h"
#include "ws2812.h"

#include "wireless.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_all(
    QK_GESC, KC_1,    KC_2,     KC_3,     KC_4,    KC_5,    KC_6,    KC_7,    KC_8,      KC_9,     KC_0,     KC_MINS,  KC_EQL,  KC_BSLS, KC_BSPC,
    KC_TAB,  KC_Q,    KC_W,     KC_E,     KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,      KC_O,     KC_P,     KC_LBRC,  KC_RBRC, KC_BSLS,
    KC_CAPS, KC_A,    KC_S,     KC_D,     KC_F,    KC_G,    KC_H,    KC_J,    KC_K,      KC_L,     KC_SCLN,  KC_QUOT,  KC_BSLS, KC_ENT,
    KC_LSFT, KC_Z,    KC_X,     KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM,   KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,   KC_DEL,
    KC_LCTL, KC_LGUI, KC_LALT,  KC_SPC,   KC_SPC,  KC_SPC,                    KC_SPC,    KC_RALT,  MO(1),    KC_LEFT,  KC_DOWN, KC_RIGHT),
  [1] = LAYOUT_all(
    KC_GRV , KC_F1,   KC_F2,   KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,    KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, KC_DEL,
    NK_TOGG, BL_SW0,  BL_SW1,  BL_SW2,   RF_TOG,  USB_TOG, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, GU_TOGG, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
  [2] = LAYOUT_all(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
  [3] = LAYOUT_all(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return process_record_wireless(keycode, record);
}

__attribute__((weak)) bool via_command_kb(uint8_t *data, uint8_t length) {
    return via_command_wireless(data, length);
}

//  每个通道的颜色 以及大写按键的颜色
const rgblight_segment_t PROGMEM bt_conn1[]     = RGBLIGHT_LAYER_SEGMENTS( {0, 1, HSV_BLUE} );      // 通道1：蓝色
const rgblight_segment_t PROGMEM bt_conn2[]     = RGBLIGHT_LAYER_SEGMENTS( {0, 1, HSV_TURQUOISE} ); // 通道2：蓝绿色（青绿）
const rgblight_segment_t PROGMEM bt_conn3[]     = RGBLIGHT_LAYER_SEGMENTS( {0, 1, HSV_ORANGE} );    // 通道3：橙色
const rgblight_segment_t PROGMEM rf24g_conn[]   = RGBLIGHT_LAYER_SEGMENTS( {0, 1, HSV_ORANGE} );    // 2.4g连接:橙色
const rgblight_segment_t PROGMEM caps_lock_[]   = RGBLIGHT_LAYER_SEGMENTS( {0, 1, HSV_PURPLE} );    // 大小写：紫色
const rgblight_segment_t PROGMEM bat_low_led[]  = RGBLIGHT_LAYER_SEGMENTS( {0, 1, HSV_RED} );      // 低电量：红色

const rgblight_segment_t* const PROGMEM _rgb_layers[] = RGBLIGHT_LAYERS_LIST( 
    bt_conn1, 
    bt_conn2, 
    bt_conn3, 
    rf24g_conn, 
    caps_lock_, 
    bat_low_led
);

// 取消所有闪烁
void rgb_adv_unblink_all_layer(void) {
    for (uint8_t i = 0; i < 5; i++) {
        rgblight_unblink_layer(i);
    }
}

// rgb电源开关 能复用，但暂时没想好放哪里
void rgb_set_power(uint8_t on)
{
    if (on)
    {
        writePin(WS2812_POWER_PIN, WS2812_POWER_ON_LEVEL);
    } 
    else 
    {
        writePin(WS2812_POWER_PIN, !WS2812_POWER_ON_LEVEL);
    }
}

// 这个是 大小写 数字锁回调
bool led_update_user(led_t led_state) {
    // 如果当前是USB连接，或者是蓝牙/2.4G连接且已配对连接状态
    if( (transport_get() > KB_TRANSPORT_USB && wireless_get() == WT_STATE_CONNECTED) || ( usb_power_connected() == true && transport_get() == KB_TRANSPORT_USB))
    {
        rgb_adv_unblink_all_layer();
        rgblight_set_layer_state(4, led_state.caps_lock);
    }
    return true;
}

// 蓝牙回调事件 连接状态回调
void wireless_ble_hanlde_kb(uint8_t host_index,uint8_t advertSta,uint8_t connectSta,uint8_t pairingSta)
{
    // rgblight_disable() 是不让rgb灯效出来，因为只有一颗灯，没必要触发灯效 
    rgblight_disable();
    rgb_adv_unblink_all_layer();

    // 蓝牙没有连接 && 蓝牙广播开启  && 蓝牙配对模式
    if(connectSta != 1 && advertSta == 1 && pairingSta == 1)
    {
        // 这里第一个参数使用host_index正好对应_rgb_layers的索引
        rgblight_blink_layer_repeat(host_index , 500, 50);
    }
    // 蓝牙没有连接 && 蓝牙广播开启  && 蓝牙非配对模式
    else if(connectSta != 1 && advertSta == 1 && pairingSta == 0)
    {
        rgblight_blink_layer_repeat(host_index , 2000, 50);
    }
    // 蓝牙已连接
    if(connectSta == 1)
    {
        rgblight_blink_layer_repeat(host_index , 200, 2);
    }

}

// 2.4g接收器 连接状态回调
void wireless_rf24g_hanlde_kb(uint8_t connectSta,uint8_t pairingSta)
{
    rgblight_disable();
    rgb_adv_unblink_all_layer();

    if(connectSta == 1)
    {
        rgblight_blink_layer_repeat(3 , 200, 2);
    }
}

void lpm_low_battery_handle_kb(void)
{
    rgblight_disable();
    // 这里的第一个参数5 ，代表 _rgb_layers 数组里面的 bat_low_led
    rgb_adv_unblink_all_layer();
    rgblight_blink_layer_repeat(5, 500, 255);    // 255次闪，相当于就让他一闪一闪的不停
}

// After initializing the peripheral
void keyboard_post_init_kb(void)
{
    ws2812_init();
    rgb_set_power(1);

    rgblight_disable();
    rgb_adv_unblink_all_layer();
}

void lpm_device_power_open(void)
{
    rgb_set_power(1);
}

void lpm_device_power_close(void)
{
    rgb_set_power(0);
}



// 将未使用的引脚设置为输入模拟 
// PS：在6095中，如果不加以下代码休眠时是102ua。如果加了就是30ua~32ua浮动
void lpm_set_unused_pins_to_input_analog(void)
{
    // 禁用调试功能以降低功耗
    DBGMCU->CR &= ~DBGMCU_CR_DBG_SLEEP;   // 禁用在Sleep模式下的调试
    DBGMCU->CR &= ~DBGMCU_CR_DBG_STOP;    // 禁用在Stop模式下的调试
    DBGMCU->CR &= ~DBGMCU_CR_DBG_STANDBY; // 禁用在Standby模式下的调试
    // 在系统初始化代码中禁用SWD接口
    palSetLineMode(A13, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(A14, PAL_MODE_INPUT_ANALOG);

    palSetLineMode(A0, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A1, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A2, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A3, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A4, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A5, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A6, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A7, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A8, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A9, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A10, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A11, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A13, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A14, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(A15, PAL_MODE_INPUT_ANALOG); 

    palSetLineMode(B0, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B1, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B2, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B3, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B4, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B5, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B6, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B7, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B8, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B9, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B10, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B11, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B13, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B14, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(B15, PAL_MODE_INPUT_ANALOG); 
}