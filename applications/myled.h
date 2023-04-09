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



//extern ebled_t g_test_led;
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
  rt_uint32_t bits0 : 2;
  rt_uint32_t bits1 : 2;
  rt_uint32_t bits2 : 2;
  rt_uint32_t bits3 : 2;
  rt_uint32_t bits4 : 2;
  rt_uint32_t bits5 : 2;
  rt_uint32_t bits6 : 2;
  rt_uint32_t bits7 : 2;
  rt_uint32_t bits8 : 2;
  rt_uint32_t bits9 : 2;
  rt_uint32_t bits10 : 2;
  rt_uint32_t bits11 : 2;
  rt_uint32_t bits12 : 2;
  rt_uint32_t bits13 : 2;
  rt_uint32_t bits14 : 2;
  rt_uint32_t bits15 : 2;
} bits32_t;
typedef union
{

  bits32_t bits;
  rt_uint32_t word32;
}bits32_u;


extern bits32_u g_rowled_data1_16;
extern bits32_u g_rowled_data17_18;
void set_led(void);
int led_init(void);
#endif /* APPLICATIONS_MYLED_H_ */
