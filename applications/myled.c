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

bits32_u g_rowled_data1_16;
bits32_u g_rowled_data17_18;
int led_init(void)
{
    g_test_led = easyblink_init_led(GET_PIN(C,13), PIN_HIGH);
    g_led1 = easyblink_init_led(GET_PIN(B,0), PIN_HIGH);
    g_led2 = easyblink_init_led(GET_PIN(B,1), PIN_HIGH);
    g_led3 = easyblink_init_led(GET_PIN(B,2), PIN_HIGH);
    g_led4 = easyblink_init_led(GET_PIN(B,3), PIN_HIGH);
    g_led5 = easyblink_init_led(GET_PIN(B,4), PIN_HIGH);
    g_led6 = easyblink_init_led(GET_PIN(B,5), PIN_HIGH);
    g_led7 = easyblink_init_led(GET_PIN(B,6), PIN_HIGH);
    g_led8 = easyblink_init_led(GET_PIN(B,7), PIN_HIGH);
    g_led9 = easyblink_init_led(GET_PIN(B,8), PIN_HIGH);
    g_led10 = easyblink_init_led(GET_PIN(B,9), PIN_HIGH);
    g_led11 = easyblink_init_led(GET_PIN(B,10), PIN_HIGH);
    g_led12 = easyblink_init_led(GET_PIN(B,11), PIN_HIGH);
    g_led13 = easyblink_init_led(GET_PIN(B,12), PIN_HIGH);
    g_led14 = easyblink_init_led(GET_PIN(B,13), PIN_HIGH);
    g_led15 = easyblink_init_led(GET_PIN(B,14), PIN_HIGH);
    g_led16 = easyblink_init_led(GET_PIN(B,15), PIN_HIGH);
    g_led17 = easyblink_init_led(GET_PIN(A,11), PIN_HIGH);
    g_led18 = easyblink_init_led(GET_PIN(A,12), PIN_HIGH);
//    g_rowled_data1_16.word32 = 0xffffffff;
//    g_rowled_data17_18.word32 = 0x0000000f;
//    set_led();
    return RT_EOK;
}
//INIT_APP_EXPORT(led_init);

void set_led(void)
{

    switch(g_rowled_data1_16.bits.bits0)
    {
        case 0:easyblink_stop(g_led1); eb_led_off(g_led1);            break;
        case 1:easyblink_stop(g_led1); easyblink(g_led1,-1,1000,2000);break;
        case 2:easyblink_stop(g_led1); easyblink(g_led1,-1,200,400);  break;
        case 3:easyblink_stop(g_led1); eb_led_on(g_led1);             break;
    }
    switch(g_rowled_data1_16.bits.bits1)
   {
       case 0:easyblink_stop(g_led2); eb_led_off(g_led2);            break;
       case 1:easyblink_stop(g_led2); easyblink(g_led2,-1,1000,2000);break;
       case 2:easyblink_stop(g_led2); easyblink(g_led2,-1,200,400);  break;
       case 3:easyblink_stop(g_led2); eb_led_on(g_led2);             break;
   }
    switch(g_rowled_data1_16.bits.bits2)
    {
        case 0:easyblink_stop(g_led3); eb_led_off(g_led3);            break;
        case 1:easyblink_stop(g_led3); easyblink(g_led3,-1,1000,2000);break;
        case 2:easyblink_stop(g_led3); easyblink(g_led3,-1,200,400);  break;
        case 3:easyblink_stop(g_led3); eb_led_on(g_led3);             break;
    }
    switch(g_rowled_data1_16.bits.bits3)
   {
       case 0:easyblink_stop(g_led4); eb_led_off(g_led4);            break;
       case 1:easyblink_stop(g_led4); easyblink(g_led4,-1,1000,2000);break;
       case 2:easyblink_stop(g_led4); easyblink(g_led4,-1,200,400);  break;
       case 3:easyblink_stop(g_led4); eb_led_on(g_led4);             break;
   }
    switch(g_rowled_data1_16.bits.bits4)
    {
        case 0:easyblink_stop(g_led5); eb_led_off(g_led5);            break;
        case 1:easyblink_stop(g_led5); easyblink(g_led5,-1,1000,2000);break;
        case 2:easyblink_stop(g_led5); easyblink(g_led5,-1,200,400);  break;
        case 3:easyblink_stop(g_led5); eb_led_on(g_led5);             break;
    }
    switch(g_rowled_data1_16.bits.bits5)
   {
       case 0:easyblink_stop(g_led6); eb_led_off(g_led6);            break;
       case 1:easyblink_stop(g_led6); easyblink(g_led6,-1,1000,2000);break;
       case 2:easyblink_stop(g_led6); easyblink(g_led6,-1,200,400);  break;
       case 3:easyblink_stop(g_led6); eb_led_on(g_led6);             break;
   }
    switch(g_rowled_data1_16.bits.bits6)
    {
        case 0:easyblink_stop(g_led7); eb_led_off(g_led7);            break;
        case 1:easyblink_stop(g_led7); easyblink(g_led7,-1,1000,2000);break;
        case 2:easyblink_stop(g_led7); easyblink(g_led7,-1,200,400);  break;
        case 3:easyblink_stop(g_led7); eb_led_on(g_led7);             break;
    }
    switch(g_rowled_data1_16.bits.bits7)
   {
       case 0:easyblink_stop(g_led8); eb_led_off(g_led8);            break;
       case 1:easyblink_stop(g_led8); easyblink(g_led8,-1,1000,2000);break;
       case 2:easyblink_stop(g_led8); easyblink(g_led8,-1,200,400);  break;
       case 3:easyblink_stop(g_led8); eb_led_on(g_led8);             break;
   }
    switch(g_rowled_data1_16.bits.bits8)
    {
        case 0:easyblink_stop(g_led9); eb_led_off(g_led9);            break;
        case 1:easyblink_stop(g_led9); easyblink(g_led9,-1,1000,2000);break;
        case 2:easyblink_stop(g_led9); easyblink(g_led9,-1,200,400);  break;
        case 3:easyblink_stop(g_led9); eb_led_on(g_led9);             break;
    }
    switch(g_rowled_data1_16.bits.bits9)
   {
       case 0:easyblink_stop(g_led10); eb_led_off(g_led10);            break;
       case 1:easyblink_stop(g_led10); easyblink(g_led10,-1,1000,2000);break;
       case 2:easyblink_stop(g_led10); easyblink(g_led10,-1,200,400);  break;
       case 3:easyblink_stop(g_led10); eb_led_on(g_led10);             break;
   }
    switch(g_rowled_data1_16.bits.bits10)
    {
        case 0:easyblink_stop(g_led11); eb_led_off(g_led11);            break;
        case 1:easyblink_stop(g_led11); easyblink(g_led11,-1,1000,2000);break;
        case 2:easyblink_stop(g_led11); easyblink(g_led11,-1,200,400);  break;
        case 3:easyblink_stop(g_led11); eb_led_on(g_led11);             break;
    }
    switch(g_rowled_data1_16.bits.bits11)
   {
       case 0:easyblink_stop(g_led12); eb_led_off(g_led12);            break;
       case 1:easyblink_stop(g_led12); easyblink(g_led12,-1,1000,2000);break;
       case 2:easyblink_stop(g_led12); easyblink(g_led12,-1,200,400);  break;
       case 3:easyblink_stop(g_led12); eb_led_on(g_led12);             break;
   }
    switch(g_rowled_data1_16.bits.bits12)
    {
        case 0:easyblink_stop(g_led13); eb_led_off(g_led13);            break;
        case 1:easyblink_stop(g_led13); easyblink(g_led13,-1,1000,2000);break;
        case 2:easyblink_stop(g_led13); easyblink(g_led13,-1,200,400);  break;
        case 3:easyblink_stop(g_led13); eb_led_on(g_led13);             break;
    }
    switch(g_rowled_data1_16.bits.bits13)
   {
       case 0:easyblink_stop(g_led14); eb_led_off(g_led14);            break;
       case 1:easyblink_stop(g_led14); easyblink(g_led14,-1,1000,2000);break;
       case 2:easyblink_stop(g_led14); easyblink(g_led14,-1,200,400);  break;
       case 3:easyblink_stop(g_led14); eb_led_on(g_led14);             break;
   }
    switch(g_rowled_data1_16.bits.bits14)
    {
        case 0:easyblink_stop(g_led15); eb_led_off(g_led15);            break;
        case 1:easyblink_stop(g_led15); easyblink(g_led15,-1,1000,2000);break;
        case 2:easyblink_stop(g_led15); easyblink(g_led15,-1,200,400);  break;
        case 3:easyblink_stop(g_led15); eb_led_on(g_led15);             break;
    }
    switch(g_rowled_data1_16.bits.bits15)
   {
       case 0:easyblink_stop(g_led16); eb_led_off(g_led16);            break;
       case 1:easyblink_stop(g_led16); easyblink(g_led16,-1,1000,2000);break;
       case 2:easyblink_stop(g_led16); easyblink(g_led16,-1,200,400);  break;
       case 3:easyblink_stop(g_led16); eb_led_on(g_led16);             break;
   }
    switch(g_rowled_data17_18.bits.bits0)
   {
       case 0:easyblink_stop(g_led17); eb_led_off(g_led17);            break;
       case 1:easyblink_stop(g_led17); easyblink(g_led17,-1,1000,2000);break;
       case 2:easyblink_stop(g_led17); easyblink(g_led17,-1,200,400);  break;
       case 3:easyblink_stop(g_led17); eb_led_on(g_led17);             break;
   }
    switch(g_rowled_data17_18.bits.bits1)
    {
        case 0:easyblink_stop(g_led18); eb_led_off(g_led18);            break;
        case 1:easyblink_stop(g_led18); easyblink(g_led18,-1,1000,2000);break;
        case 2:easyblink_stop(g_led18); easyblink(g_led18,-1,200,400);  break;
        case 3:easyblink_stop(g_led18); eb_led_on(g_led18);             break;
    }

}

