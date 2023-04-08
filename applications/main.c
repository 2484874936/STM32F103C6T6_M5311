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
    m5311_moudle_init();
    int count=0;
    easyblink(g_test_led, -1, 100, 500);
    for(;;)
    {
        count++;
//        LOG_D("Hello RT-Thread for %d times!",count);
//        rt_kprintf("Hello RT-Thread for %d times!\n",count);
//        send_at("OK\r\n",100,1,"AT+MQTTPUB=\"row_led_data\",1,0,0,0,\"HELLOWORLD\"\r\n");
//        rt_uprintf(G_UART_2,"AT\r\n");
        if(send_at("+MQTTSTAT: 5\r\n",100,1,"AT+MQTTSTAT?\r\n") != RT_EOK)
        {
            rt_kprintf("MQTT RECONNECTING...\n");
            if(m5311_moudle_init() != RT_EOK)
            {
                rt_kprintf("MQTT RECONNECT ERROR\n");
            }
            else
            {
                rt_kprintf("MQTT RECONNECT SUCCESS\n");
            }
        }
//        g_rowled_data1_16.bits.bits0 = 2;
//        g_rowled_data1_16.bits.bits1 = 2;
//        g_rowled_data1_16.bits.bits2 = 2;
//        g_rowled_data1_16.bits.bits3 = 2;
//        g_rowled_data1_16.bits.bits4 = 2;
//        g_rowled_data1_16.bits.bits5 = 2;
//        g_rowled_data1_16.bits.bits6 = 2;
//        g_rowled_data1_16.bits.bits7 = 2;
//        g_rowled_data1_16.bits.bits8 = 2;
//        g_rowled_data1_16.bits.bits9 = 2;
//        g_rowled_data1_16.bits.bits10 = 2;
//        g_rowled_data1_16.bits.bits11 = 2;
//        g_rowled_data1_16.bits.bits12 = 2;
//        g_rowled_data1_16.bits.bits13 = 2;
//        g_rowled_data1_16.bits.bits14 = 2;
//        g_rowled_data1_16.bits.bits15 = 2;
//        g_rowled_data17_18.bits.bits0 = 2;
//        g_rowled_data17_18.bits.bits1 = 2;
//        rt_kprintf("g_rowled_data1_16=0x%08X\g_rowled_data17_18=0x%08X\n",g_rowled_data1_16.word32,g_rowled_data17_18.word32);
        set_led();
        rt_thread_mdelay(5000);
    }
    return RT_EOK;
}
