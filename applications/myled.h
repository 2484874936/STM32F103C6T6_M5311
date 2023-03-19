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


typedef struct
{
  uint32_t bit0 : 1;
  uint32_t bit1 : 1;
  uint32_t bit2 : 1;
  uint32_t bit3 : 1;
  uint32_t bit4 : 1;
  uint32_t bit5 : 1;
  uint32_t bit6 : 1;
  uint32_t bit7 : 1;
  uint32_t bit8 : 1;
  uint32_t bit9 : 1;
  uint32_t bit10 : 1;
  uint32_t bit11 : 1;
  uint32_t bit12 : 1;
  uint32_t bit13 : 1;
  uint32_t bit14 : 1;
  uint32_t bit15 : 1;
  uint32_t bit16 : 1;
  uint32_t bit17 : 1;
  uint32_t bit18 : 1;
  uint32_t bit19 : 1;
  uint32_t bit20 : 1;
  uint32_t bit21 : 1;
  uint32_t bit22 : 1;
  uint32_t bit23 : 1;
  uint32_t bit24 : 1;
  uint32_t bit25 : 1;
  uint32_t bit26 : 1;
  uint32_t bit27 : 1;
  uint32_t bit28 : 1;
  uint32_t bit29 : 1;
  uint32_t bit30 : 1;
  uint32_t bit31 : 1;
} bits32_t;
typedef union
{
  uint32_t word32;
  bits32_t bit;
}bits32_u;


extern bits32_u g_rowled_data;
void set_led(void);
int led_init(void);
#endif /* APPLICATIONS_MYLED_H_ */
