/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-16     RT-Thread    first version
 */

#include <rtthread.h>

//#define DBG_TAG "main"
//#define DBG_LVL DBG_LOG
//#include <rtdbg.h>
#include <myled.h>
#include "bsp_uart.h"
#include "app_m5311.h"

int main(void)
{

    uart_init();
    led_init();
    m5311_moudle_init(INIT_BLINK_LED);
    int count=0;
//    easyblink(g_test_led, -1, 100, 500);
    for(;;)
    {

        if(send_at("STAT: 5\r\n",1000,1,"AT+MQTTSTAT?\r\n") != RT_EOK)
        {
            count++;
            if(count == 3)
            {
                count = 0;
            rt_kprintf("MQTT RECONNECTING...\n");
            if(m5311_moudle_init(REINIT_BLINK_LED) != RT_EOK)
            {
                rt_kprintf("MQTT RECONNECT ERROR\n");
            }
            else
            {
                rt_kprintf("MQTT RECONNECT SUCCESS\n");
            }
        }
        }
        else
        {
            count = 0;
        }
        set_led();
        rt_thread_mdelay(900);
    }
    return RT_EOK;
}
