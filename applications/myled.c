/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-15     user       the first version
 */


#include <myled.h>


ebled_t g_test_led = RT_NULL;

int led_init(void)
{
    g_test_led = easyblink_init_led(GET_PIN(C,13), PIN_LOW);
    return RT_EOK;
}
//INIT_APP_EXPORT(led_init);

