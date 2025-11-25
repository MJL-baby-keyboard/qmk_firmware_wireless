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

// Very few codes are borrowed from https://www.keychron.com
#include "quantum.h"
#include "wireless.h"
#include "usb_main.h"
#include "transport.h"
#include "outputselect.h"
static kb_transport_t transport = KB_TRANSPORT_USB; // 默认上电就是usb

void transport_set(kb_transport_t new_transport)
{
    transport = new_transport;
    if (IS_WIRELESS_TRANSPORT(transport)) 
    {
        set_output(OUTPUT_BLUETOOTH);
    }
    else if(transport == KB_TRANSPORT_USB)
    {
        set_output(OUTPUT_USB);
    }
    else
    {
        set_output(OUTPUT_NONE);
    }
}

kb_transport_t transport_get(void)
{
    return transport;
}


void usb_remote_wakeup(void) {
    usb_event_queue_task();

    if (USB_DRIVER.state == USB_SUSPENDED) {
#if defined(WB32F3G71xx)
        // wait_ms(300);
        // if (!usb_power_connected()) return;
#endif
        while (USB_DRIVER.state == USB_SUSPENDED) {
            if (transport_get() != KB_TRANSPORT_USB) {
                suspend_wakeup_init_quantum();
                return;
            }
            if (transport_get() == KB_TRANSPORT_USB && !usb_power_connected()) return;
            /* Do this in the suspended state */
            suspend_power_down(); // on AVR this deep sleeps for 15ms
            /* Remote wakeup */
            if (suspend_wakeup_condition()) {
                usbWakeupHost(&USB_DRIVER);
                wait_ms(300);
#ifdef MOUSEKEY_ENABLE
                // Wiggle to wakeup
                mousekey_on(MS_LEFT);
                mousekey_send();
                wait_ms(10);
                mousekey_on(MS_RGHT);
                mousekey_send();
                wait_ms(10);
                mousekey_off((MS_RGHT));
                mousekey_send();
#else
                set_mods(0x02);
                send_keyboard_report();
                wait_ms(10);
                del_mods(0x02);
                send_keyboard_report();
#endif
            }
        }
        /* Woken up */
        // variables has been already cleared by the wakeup hook
        send_keyboard_report();
#ifdef MOUSEKEY_ENABLE
        mousekey_send();
#endif /* MOUSEKEY_ENABLE */
    }
}
