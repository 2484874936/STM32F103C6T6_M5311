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
static char IMEI[18] = {0};

void m5311_wakeup(void);
void m5311_pwron(void);
void m5311_pwroff(void);
void m5311_reset(void);
rt_size_t send_at(char *ack, rt_uint32_t timeout, int num, ...);

int m5311_moudle_init(void)
{
    rt_uint8_t cnt = 0;
    rt_pin_mode( m5311_wakeup_pin, PIN_MODE_OUTPUT);
    rt_pin_write(m5311_wakeup_pin, PIN_HIGH);
    rt_pin_mode( m5311_pwr_pin, PIN_MODE_OUTPUT);
    rt_pin_write(m5311_pwr_pin, PIN_HIGH);
    rt_pin_mode( um5311_reset_pin, PIN_MODE_OUTPUT);
    rt_pin_write(um5311_reset_pin, PIN_HIGH);
    m5311_pwron();
    m5311_reset();
    while(send_at("OK",500,1,"AT\r\n") != RT_EOK);//等待模组正常
    {
        cnt ++;
        if(cnt == 10)
        {
            return RT_ERROR;
        }
    }
    cnt = 0;
//    while(send_at("OK\r\n",100,1,"ATE1\r\n") != RT_EOK);
    while(send_at("OK\r\n",100,1,"AT+GSN\r\n") != RT_EOK);//获取IMEI
    while(send_at("OK\r\n",100,1,"AT+CIMI\r\n") != RT_EOK);//读SIM卡正常，获取IMSI
    while(send_at("OK\r\n",100,1,"ATE0\r\n") != RT_EOK);//ATE0:关闭回显，ATE1:打开回显
    while(send_at("OK\r\n",100,1,"AT+CMSYSCTRL=0,2\r\n") != RT_EOK);
    while(send_at("OK\r\n",100,1,"AT+MQTTPING=0\r\n") != RT_EOK);
    while(send_at("OK\r\n",500,1,"AT+CGPADDR=1\r\n") != RT_EOK)//获取网络IP
    {
        cnt ++;
        if(cnt ==10)
        {
            rt_kprintf("get ip timeout\r\n");
            return RT_ETIMEOUT;
        }
    }
    cnt = 0;

    m5311_modle.mqtt_host = "\"101.69.254.66\"";
    m5311_modle.mqtt_port = "1883";
    m5311_modle.mqtt_clientid = IMEI;
    m5311_modle.keepalive = "120";
    m5311_modle.user = "\"\"";
    m5311_modle.passwd = "\"\"";
    m5311_modle.clean= "1";
    while(send_at("OK\r\n", 100, 15, "AT+MQTTCFG=", m5311_modle.mqtt_host, DOUHAO, \
                  m5311_modle.mqtt_port, DOUHAO, m5311_modle.mqtt_clientid, DOUHAO, \
                  m5311_modle.keepalive, DOUHAO, m5311_modle.user, DOUHAO, \
                  m5311_modle.passwd, DOUHAO, m5311_modle.clean,"\r\n") != RT_EOK)//连接MQTT
    {
        cnt ++;
        if(cnt == 10)
        {
            rt_kprintf("connect mqtt error!\r\n");
            return RT_ERROR;
        }
    }
    cnt = 0;

   m5311_modle.usrflag = "0";
   m5311_modle.pwdflag = "0";
   m5311_modle.willflag = "0";
   m5311_modle.willretain = "0";
   m5311_modle.willqos = "0";
   m5311_modle.willtopic = "\"\"";
   m5311_modle.willmessage = "\"\"";
    while(send_at("+MQTTOPEN: OK\r\n", 100, 15, "AT+MQTTOPEN=", m5311_modle.usrflag, DOUHAO, \
                  m5311_modle.pwdflag, DOUHAO, m5311_modle.willflag, DOUHAO, \
                  m5311_modle.willretain, DOUHAO, m5311_modle.willqos, DOUHAO, \
                  m5311_modle.willtopic, DOUHAO, m5311_modle.willmessage,"\r\n") != RT_EOK)//连接MQTT
    {
        cnt ++;
        if(cnt == 10)
        {
            rt_kprintf("mqtt open error!\r\n");
            return RT_ERROR;
        }
    }
    cnt = 0;
}

int uart2_data_processing(char *buffer, rt_size_t index)
{
#if defined(BSP_UART2_RX_USING_DMA)
    if(index)
#else
    if(buffer[index-2] == '\r' && buffer[index-1] == '\n')//中断必须要限制才能输出字符串
#endif
    {
        char *temp;
        rt_kprintf("uart2 receive:%s",buffer);
        if(m5311_modle.compare_str != RT_NULL)
        {
           temp = strstr(buffer,m5311_modle.compare_str);
           if(temp)
           {
               if(strstr(buffer,"86"))
               {
                   for(rt_uint8_t i = 0; i < 15; i ++)
                   {
                       IMEI[0] = '\"';
                       IMEI[i+1] = buffer[i + 2];
                       IMEI[16] = IMEI[0];
                       IMEI[17] = '\0';
                   }
                   rt_kprintf("IMEI=%s\n",IMEI);
               }
               rt_mb_send(G_UART_2.out_mb,temp);
           }
        }
        rt_memset(buffer, 0, index);
        index = 0;
    }
    return index;
}


void m5311_wakeup(void)
{
    rt_pin_write(m5311_wakeup_pin, PIN_LOW);
    rt_thread_mdelay(55);
    rt_pin_write(m5311_wakeup_pin, PIN_HIGH);
    rt_thread_mdelay(10);
}


void m5311_pwron(void)
{
    rt_pin_write(m5311_pwr_pin, PIN_LOW);
    rt_thread_mdelay(2100);
    rt_pin_write(m5311_pwr_pin, PIN_HIGH);
    rt_thread_mdelay(10);
}

void m5311_pwroff(void)
{
    rt_pin_write(m5311_pwr_pin, PIN_LOW);
    rt_thread_mdelay(8100);
    rt_pin_write(m5311_pwr_pin, PIN_HIGH);
    rt_thread_mdelay(10);
}


void m5311_reset(void)
{
    rt_pin_write(um5311_reset_pin, PIN_LOW);
    rt_thread_mdelay(100);
    rt_pin_write(um5311_reset_pin, PIN_HIGH);
    rt_thread_mdelay(10);
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
    rt_memset(at_send_buf, 0, AT_SEND_BUF);
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

        if(rt_mb_recv(G_UART_2.out_mb, &m5311_modle.compare_str_location, timeout) == -RT_ETIMEOUT)
        {
            rt_kprintf("command [%s] ack timeout\r\n",at_send_buf);
            return -RT_ETIMEOUT;
        }
        m5311_modle.compare_str = RT_NULL;
        if(strstr(m5311_modle.compare_str_location,"ERR="))
        {
            rt_kprintf("command [%s] ack error\r\n",at_send_buf);
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
