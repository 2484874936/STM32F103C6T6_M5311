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

bits32_u g_rowled_data;
void set_led(void)
{
    if(g_rowled_data.bit.bit0) eb_led_on(g_led1);
    else                       eb_led_off(g_led1);

    if(g_rowled_data.bit.bit1) eb_led_on(g_led2);
    else                       eb_led_off(g_led2);

    if(g_rowled_data.bit.bit2) eb_led_on(g_led3);
    else                       eb_led_off(g_led3);

    if(g_rowled_data.bit.bit3) eb_led_on(g_led4);
    else                       eb_led_off(g_led4);

    if(g_rowled_data.bit.bit4) eb_led_on(g_led5);
    else                       eb_led_off(g_led5);

    if(g_rowled_data.bit.bit5) eb_led_on(g_led6);
    else                       eb_led_off(g_led6);

    if(g_rowled_data.bit.bit6) eb_led_on(g_led7);
    else                       eb_led_off(g_led7);

    if(g_rowled_data.bit.bit7) eb_led_on(g_led8);
    else                       eb_led_off(g_led8);

    if(g_rowled_data.bit.bit8) eb_led_on(g_led9);
    else                       eb_led_off(g_led9);

    if(g_rowled_data.bit.bit9) eb_led_on(g_led10);
    else                       eb_led_off(g_led10);

    if(g_rowled_data.bit.bit10) eb_led_on(g_led11);
    else                        eb_led_off(g_led11);

    if(g_rowled_data.bit.bit11) eb_led_on(g_led12);
    else                        eb_led_off(g_led12);

    if(g_rowled_data.bit.bit12) eb_led_on(g_led13);
    else                        eb_led_off(g_led13);

    if(g_rowled_data.bit.bit13) eb_led_on(g_led14);
    else                        eb_led_off(g_led14);

    if(g_rowled_data.bit.bit14) eb_led_on(g_led15);
    else                        eb_led_off(g_led15);

    if(g_rowled_data.bit.bit15) eb_led_on(g_led16);
    else                        eb_led_off(g_led16);

    if(g_rowled_data.bit.bit16)  eb_led_on(g_led17);
    else                        eb_led_off(g_led17);

    if(g_rowled_data.bit.bit17) eb_led_on(g_led18);
    else                        eb_led_off(g_led18);

}

