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
ebled_t g_led1 = RT_NULL;
ebled_t g_led2 = RT_NULL;
ebled_t g_led3 = RT_NULL;
ebled_t g_led4 = RT_NULL;
ebled_t g_led5 = RT_NULL;
ebled_t g_led6 = RT_NULL;
ebled_t g_led7 = RT_NULL;
ebled_t g_led8 = RT_NULL;
ebled_t g_led9 = RT_NULL;
ebled_t g_led10 = RT_NULL;
ebled_t g_led11 = RT_NULL;
ebled_t g_led12 = RT_NULL;
ebled_t g_led13 = RT_NULL;
ebled_t g_led14 = RT_NULL;
ebled_t g_led15 = RT_NULL;
ebled_t g_led16 = RT_NULL;
ebled_t g_led17 = RT_NULL;
ebled_t g_led18 = RT_NULL;

int led_init(void)
{
    g_test_led = easyblink_init_led(GET_PIN(C,13), PIN_LOW);
    g_led1 = easyblink_init_led(GET_PIN(B,0), PIN_LOW);
    g_led2 = easyblink_init_led(GET_PIN(B,1), PIN_LOW);
    g_led3 = easyblink_init_led(GET_PIN(B,2), PIN_LOW);
    g_led4 = easyblink_init_led(GET_PIN(B,3), PIN_LOW);
    g_led5 = easyblink_init_led(GET_PIN(B,4), PIN_LOW);
    g_led6 = easyblink_init_led(GET_PIN(B,5), PIN_LOW);
    g_led7 = easyblink_init_led(GET_PIN(B,6), PIN_LOW);
    g_led8 = easyblink_init_led(GET_PIN(B,7), PIN_LOW);
    g_led9 = easyblink_init_led(GET_PIN(B,8), PIN_LOW);
    g_led10 = easyblink_init_led(GET_PIN(B,9), PIN_LOW);
    g_led11 = easyblink_init_led(GET_PIN(B,10), PIN_LOW);
    g_led12 = easyblink_init_led(GET_PIN(B,11), PIN_LOW);
    g_led13 = easyblink_init_led(GET_PIN(B,12), PIN_LOW);
    g_led14 = easyblink_init_led(GET_PIN(B,13), PIN_LOW);
    g_led15 = easyblink_init_led(GET_PIN(B,14), PIN_LOW);
    g_led16 = easyblink_init_led(GET_PIN(B,15), PIN_LOW);
    g_led17 = easyblink_init_led(GET_PIN(A,11), PIN_LOW);
    g_led18 = easyblink_init_led(GET_PIN(A,12), PIN_LOW);
    return RT_EOK;
}
//INIT_APP_EXPORT(led_init);

