/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-30     afun       the first version
 */
#include "app_m5311.h"
#include <string.h>


rt_base_t m5311_wakeup_pin = GET_PIN(A,4);
rt_base_t m5311_pwr_pin    = GET_PIN(A,5);
rt_base_t um5311_reset_pin = GET_PIN(A,6);
static struct M5311_Modle m5311_modle;

void m5311_wakeup(void)
{
    rt_pin_write(m5311_wakeup_pin, PIN_LOW);
    rt_thread_mdelay(55);
    rt_pin_write(m5311_wakeup_pin, PIN_HIGH);
}


void m5311_pwron(void)
{
    rt_pin_write(m5311_pwr_pin, PIN_LOW);
    rt_thread_mdelay(2100);
    rt_pin_write(m5311_pwr_pin, PIN_HIGH);
}

void m5311_pwroff(void)
{
    rt_pin_write(m5311_pwr_pin, PIN_LOW);
    rt_thread_mdelay(8100);
    rt_pin_write(m5311_pwr_pin, PIN_HIGH);
}


void m5311_reset(void)
{
    rt_pin_write(um5311_reset_pin, PIN_LOW);
    rt_thread_mdelay(100);
    rt_pin_write(um5311_reset_pin, PIN_HIGH);
}

int m5311_moudle_init(void)
{
    rt_pin_mode( m5311_wakeup_pin, PIN_MODE_OUTPUT);
    rt_pin_write(m5311_wakeup_pin, PIN_HIGH);
    rt_pin_mode( m5311_pwr_pin, PIN_MODE_OUTPUT);
    rt_pin_write(m5311_pwr_pin, PIN_HIGH);
    rt_pin_mode( um5311_reset_pin, PIN_MODE_OUTPUT);
    rt_pin_write(um5311_reset_pin, PIN_HIGH);
    m5311_pwron();
}


int uart2_data_processing(char *buffer, rt_size_t index)
{
#if defined(BSP_UART2_RX_USING_DMA)
    if(index)
#else
    if(buffer[index-2] == '\r' && buffer[index-1] == '\n')//中断必须要限制才能输出字符串
#endif
    {
        rt_kprintf("%s",buffer);
//        rt_mb_send(G_UART_2.out_mb, (rt_uint32_t)buffer);
        rt_memset(buffer, 0, index);
        index = 0;
    }
    return index;
}

//ack:需要对比的字符串
//timewait:等待AT回复的时间
//num:需要拼接的字符串个数
//...:n个需要被拼接的字符串
rt_size_t send_at(char *ack, rt_uint32_t timeout, int num, ...)
{
    RT_ASSERT(num);
    va_list args;

    rt_size_t length;
    static char at_send_buf[AT_SEND_BUF];

    va_start(args,num);
    for(rt_uint8_t i = 0; i < num; i++)
    {
        char* str = va_arg(args,char*);
        length = rt_sprintf(at_send_buf, "%s%s", at_send_buf,str);
        if (length > AT_SEND_BUF - 1)
        {
            rt_kprintf("too long AT command\r\n");
            rt_memset(at_send_buf, 0, AT_SEND_BUF);
            return RT_ERROR;
        }
    }
    va_end(args);
    rt_kprintf("AT_command:%s",at_send_buf);
    G_UART_2.send(at_send_buf, length);
    rt_memset(at_send_buf, 0, length);

    if(strstr(ack,DENNGHAO))
    {
        m5311_modle.compare_str_location = at_send_buf;
    }
    else
    {
        m5311_modle.compare_str = ack;
    }

    if(m5311_modle.compare_str != RT_NULL)
    {

        if(rt_mb_recv(G_UART_2.out_mb, &m5311_modle.compare_str_location, timeout) != RT_EOK)
        {
            rt_kprintf("AT ack timeout\r\n");
            return RT_ERROR;
        }
        m5311_modle.compare_str = RT_NULL;
        if(strstr(m5311_modle.compare_str_location,"ERR="))
        {
            rt_kprintf("AT command error\r\n");
            return RT_ERROR;
        }
        else return RT_EOK;
//        rt_kprintf("A=%s\r\n",m5311_modle.compare_str_location);
     }
    else
    {
        rt_thread_mdelay(timeout);
        return RT_EOK;
    }
}
