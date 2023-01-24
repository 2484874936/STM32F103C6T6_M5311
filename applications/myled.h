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

int led_init(void);
#endif /* APPLICATIONS_MYLED_H_ */
