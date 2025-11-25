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
#include "wireless_config.h"
#include "report_buffer.h"
#include "bluetooth.h"
#include "bhq.h"

/* report_interval value should be less than bluetooth connection interval because
 * it takes some time for communicating between mcu and bluetooth module. Carefully
 * set this value to feed the bt module so that we don't lost the key report nor lost
 * the anchor point of bluetooth interval. The bluetooth connection interval varies
 * if BLE is used, invoke report_buffer_set_inverval() to update the value
 */
uint8_t report_interval = DEFAULT_REPORT_INVERVAL_MS;

static uint32_t report_timer_buffer = 0;
uint32_t        retry_time_buffer   = 0;
report_buffer_t report_buffer_queue[WIRELESS_REPORT_BUFFER_QUEUE_SIZE];
uint16_t         report_buffer_queue_head;
uint16_t         report_buffer_queue_tail;
report_buffer_t kb_rpt;
uint8_t         retry = 0;

void report_buffer_init(void) {
    // Initialise the report queue
    memset(&report_buffer_queue, 0, sizeof(report_buffer_queue));
    report_buffer_queue_head    = 0;
    report_buffer_queue_tail    = 0;
    report_timer_buffer         = timer_read32();
    retry                       = 0;
    retry_time_buffer           = 0;
}

bool report_buffer_enqueue(report_buffer_t *report) {
    uint16_t next = (report_buffer_queue_head + 1) % WIRELESS_REPORT_BUFFER_QUEUE_SIZE;
    if (next == report_buffer_queue_tail) {
        return false;
    }

    report_buffer_queue[report_buffer_queue_head] = *report;
    report_buffer_queue_head                      = next;
    return true;
}

inline bool report_buffer_dequeue(report_buffer_t *report) {
    if (report_buffer_queue_head == report_buffer_queue_tail) {
        return false;
    }

    *report                  = report_buffer_queue[report_buffer_queue_tail];
    report_buffer_queue_tail = (report_buffer_queue_tail + 1) % WIRELESS_REPORT_BUFFER_QUEUE_SIZE;
    return true;
}
report_buffer_t* report_buffer_peek_last(void) {
    if (report_buffer_queue_head == report_buffer_queue_tail) {
        return NULL; 
    }

    uint16_t last_index = (report_buffer_queue_head + WIRELESS_REPORT_BUFFER_QUEUE_SIZE - 1) % WIRELESS_REPORT_BUFFER_QUEUE_SIZE;
    return &report_buffer_queue[last_index];
}

bool report_buffer_is_empty() {
    return report_buffer_queue_head == report_buffer_queue_tail;
}

void report_buffer_update_timer(void) {
    report_timer_buffer = timer_read32();
}

bool report_buffer_next_inverval(void) {
    return timer_elapsed32(report_timer_buffer) > report_interval;
}

void report_buffer_set_inverval(uint8_t interval) {
    report_interval = interval;
}


uint8_t report_buffer_get_retry(void) {
    return retry;
}

void report_buffer_set_retry(uint8_t times) {
    retry = times;
}
void report_buffer_clear(void) {
    report_buffer_queue_head = 0;
    report_buffer_queue_tail = 0;
    retry                    = 0;
    retry_time_buffer        = 0;
    memset(&kb_rpt, 0, sizeof(kb_rpt));
}

void report_buffer_task(void) {
    if ((!report_buffer_is_empty() || retry) && report_buffer_next_inverval()) 
    {
        bool pending_data = false;
        if (!retry) {
            if (report_buffer_dequeue(&kb_rpt) && kb_rpt.type != REPORT_TYPE_NONE) {
                if (timer_read32() > 2) {
                    pending_data      = true;
                    retry             = RETPORT_RETRY_COUNT;
                    retry_time_buffer = timer_read32();
                    // bhq_printf("1 retry:%d\n",retry);
                }
            }
        } else {
            if (timer_elapsed32(retry_time_buffer) > 15) {  // retry interval
                pending_data = true;
                --retry;
                retry_time_buffer = timer_read32();
                // bhq_printf("2 retry:%d\n",retry);
            }
        }
       if (pending_data) {
#if defined(NKRO_ENABLE)
            // nkro 
            if(kb_rpt.type == REPORT_TYPE_NKRO)
            {
                bhq_send_nkro(kb_rpt.report_data);
            }
#endif
            if(kb_rpt.type == REPORT_TYPE_KB)
            {
                bhq_send_keyboard(kb_rpt.report_data);
            }
            if(kb_rpt.type == REPORT_TYPE_CONSUMER)
            {
                bhq_send_consumer(kb_rpt.consumer);
            }
            if(kb_rpt.type == REPORT_TYPE_SYSTEM)
            {
                bhq_send_system(kb_rpt.consumer);
            }
            if(kb_rpt.type == REPORT_TYPE_HID_RAW)
            {
                // bhq_printf("report hid raw mcu send length:%d\r\n",kb_rpt.length);

                bhq_send_hid_raw(kb_rpt.report_data, kb_rpt.length);
            }
            report_buffer_update_timer();
        }
    }
}
