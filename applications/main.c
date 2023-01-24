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

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <myled.h>
#include "bsp_uart.h"

int main(void)
{

    uart_init();
    led_init();
    int count=0;
    easyblink(g_test_led, -1, 100, 500);
    for(;;)
    {
        count++;
        LOG_D("Hello RT-Thread for %d times!",count);
//        rt_kprintf("Hello RT-Thread for %d times!\n",count);
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}
