/*
 * Change Logs:
 * Date           Author       Notes
 * 2022-12-01     afun       the first version
 */
#ifndef APPLICATIONS_MYLED_H_
#define APPLICATIONS_MYLED_H_

#include <rtthread.h>
#include "rtdevice.h"
#include <board.h>
#include <drv_common.h>
#include "easyblink.h"

extern ebled_t g_test_led;
extern ebled_t g_led1;
extern ebled_t g_led2;
extern ebled_t g_led3;
extern ebled_t g_led4;
extern ebled_t g_led5;
extern ebled_t g_led6;
extern ebled_t g_led7;
extern ebled_t g_led8;
extern ebled_t g_led9;
extern ebled_t g_led10;
extern ebled_t g_led11;
extern ebled_t g_led12;
extern ebled_t g_led13;
extern ebled_t g_led14;
extern ebled_t g_led15;
extern ebled_t g_led16;
extern ebled_t g_led17;
extern ebled_t g_led18;


int led_init(void);
#endif /* APPLICATIONS_MYLED_H_ */
