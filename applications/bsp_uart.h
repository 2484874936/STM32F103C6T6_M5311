#ifndef __BSP_UART_H
#define __BSP_UART_H

#include <rtthread.h>
#include <board.h>
#include <drv_common.h>

#include "rtdevice.h"
typedef struct Uart
{
    rt_device_t serial;//设备名
    rt_mailbox_t out_mb;//外部邮箱
    rt_mailbox_t dma_mb;//DMA接收邮箱
    rt_sem_t rx_sem;//中断接收信号量
    rt_mutex_t tx_mutex;//发送互斥量
    rt_size_t (*send)(void *, rt_size_t);//发送函数
    rt_size_t (*recv)(char *, rt_int32_t);//DMA接收函数
    rt_err_t (*input)(rt_device_t, rt_size_t);//接收回调函数
    int (*init)(void);//初始化函数
    int (*data_processing)(char *,rt_size_t);//数据处理函数
    struct rt_ringbuffer *ringbuffer;
    rt_int16_t  ringbuffer_size;
} uart_t;

#define UART_TX_SIZE 256

//#define USING_UART1
#define USING_UART2
//#define USING_UART3
//#define USING_UART4
//#define USING_UART5

#if defined(USING_UART1)
#define UART1_NAME       "uart1"      /* 串口设备名称 */
#define UART1_DEFAULT_BAUD_RATE   115200
//#define RS485_UART1
extern uart_t G_UART_1;
#endif

#if defined(USING_UART2)
#define UART2_NAME       "uart2"       /* 串口设备名称 */
#define UART2_DEFAULT_BAUD_RATE   115200
//#define RS485_UART2
extern uart_t G_UART_2;
#endif

#if defined(USING_UART3)
#define UART3_NAME       "uart3"      /* 串口设备名称 */
#define UART3_DEFAULT_BAUD_RATE   115200
//#define RS485_UART3
extern uart_t G_UART_3;
#endif

#if defined(USING_UART4)
#define UART4_NAME       "uart4"      /* 串口设备名称 */
#define UART4_DEFAULT_BAUD_RATE   57600
#define RS485_UART4
extern uart_t G_UART_4;
#endif

#if defined(USING_UART5)
#define UART5_NAME       "uart5"      /* 串口设备名称 */
#define UART5_DEFAULT_BAUD_RATE   115200
#define RS485_UART5
extern uart_t G_UART_5;
#endif

int rt_uprintf(uart_t uart,const char *fmt, ...);
int uart_init(void);
int uart_baud_rate_set(uart_t uart, rt_uint32_t baud_rate);
int find_pos(char *a,char *b);
void print_hex(const rt_uint8_t *buf, int len);

#endif //__BSP_UART_H
