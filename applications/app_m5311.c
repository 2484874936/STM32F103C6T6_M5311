 /*
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-30     afun       the first version
 */
#include "app_m5311.h"
#include <myled.h>
#include <string.h>
#define LOG_TAG "M5311"
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"

rt_base_t m5311_wakeup_pin = GET_PIN(A,4);
rt_base_t m5311_pwr_pin    = GET_PIN(A,5);
rt_base_t um5311_reset_pin = GET_PIN(A,6);
static struct M5311_Modle m5311_modle;
static char IMEI[18] = {0};
static char urctopiccompare[19] = {0};
static _Bool get_IMEI_flag;



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

    while(send_at("OK\r\n",100,1,"ATE0\r\n") != RT_EOK);//ATE0:关闭回显， ATE1:打开回显
    while(send_at("OK\r\n",100,1,"AT+SM=LOCK_FOREVER\r\n") != RT_EOK);//关闭模组休眠，掉电保持
//    while(send_at("OK\r\n",100,1,"AT+CEDRXS=0,5\r\n") != RT_EOK);//关闭eDXR模式
//    while(send_at("OK\r\n",100,1,"AT+CPSMS=1,,,\"00100010\",\"00101111\"\r\n") != RT_EOK);//使能PSM模式

    get_IMEI_flag = 1;
    while(send_at("86",100,1,"AT+GSN\r\n") != RT_EOK);//获取IMEI
    get_IMEI_flag = 0;
    while(send_at("OK\r\n",1000,1,"AT+CIMI\r\n") != RT_EOK);//读SIM卡正常，获取IMSI
    while(send_at(",1\r\n",1000,1,"AT+CEREG?\r\n") != RT_EOK)//确认基站注册状态，1-代表本地已注册上， 5-代表漫游已注册上
    {
        while(send_at("OK\r\n",1000,1,"AT+CEREG=1\r\n") != RT_EOK);
    }
    while(send_at("+CGATT: 1\r\n",1000,1,"AT+CGATT?\r\n") != RT_EOK);//确认 PDP 激活状态，1-代表已激活 0-代表未激活

    while(send_at("OK\r\n",100,1,"AT+CMSYSCTRL=0,2\r\n") != RT_EOK);

    while(send_at("OK\r\n",100,1,"AT+MQTTPING=1\r\n") != RT_EOK);
//    send_at("OK\r\n",100,1,"AT+MQTTPING?\r\n");

    MQTT_connect();

}

int MQTT_connect(void)
{
    rt_uint8_t cnt = 0;
    while(send_at("+CGPADDR: 1",2000,1,"AT+CGPADDR=1\r\n") != RT_EOK)//获取网络IP
    {
       cnt ++;
       if(cnt ==10)
       {
           rt_kprintf("Can't get IP, M5311 havn't connect net\r\n");
           return RT_ETIMEOUT;
       }
    }
    cnt = 0;
    send_at("OK\r\n",100,1,"AT+MQTTDISC\r\n");//断开MQTT连接
    send_at("OK\r\n",100,1,"AT+MQTTDEL\r\n");//删除MQTT client配置
    //    m5311_modle.mqtt_host = "\"101.69.254.66\"";
    //    m5311_modle.mqtt_port = "1883";
    //    m5311_modle.mqtt_clientid = IMEI;
    //    m5311_modle.keepalive = "120";
    //    m5311_modle.user = "\"\"";
    //    m5311_modle.passwd = "\"\"";
    //    m5311_modle.clean= "1";
    m5311_modle.mqtt_host = "\"zz.zcczcc.com\"";
    m5311_modle.mqtt_port = "9004";
    m5311_modle.mqtt_clientid = IMEI;
    m5311_modle.keepalive = "120";
    m5311_modle.user = "\"\"";
    m5311_modle.passwd = "\"\"";
    m5311_modle.clean= "1";
    do{
       send_at("OK\r\n", 1000, 15, "AT+MQTTCFG=", m5311_modle.mqtt_host, DOUHAO, \
                         m5311_modle.mqtt_port, DOUHAO, m5311_modle.mqtt_clientid, DOUHAO, \
                         m5311_modle.keepalive, DOUHAO, m5311_modle.user, DOUHAO, \
                         m5311_modle.passwd, DOUHAO, m5311_modle.clean,"\r\n");//进行MQTT client配置
       //手册测试条例
    //        send_at("OK", 1000, 1,"AT+MQTTCFG=\"183.230.40.39\",6002,\"4069959\",60,\"75829\",\"IIOu0oFUg1guk20ornTK1uzAcnM=\",1\r\n");//连接MQTT
       cnt ++;
       if(cnt == 10)
       {
           rt_kprintf("connect mqtt error!\r\n");
           return RT_ERROR;
       }
    } while(send_at("+MQTTSTAT: 1\r\n",100,1,"AT+MQTTSTAT?\r\n") != RT_EOK);//若client参数未初始化
    cnt = 0;


    m5311_modle.usrflag = "0";
    m5311_modle.pwdflag = "0";
    m5311_modle.willflag = "0";
    m5311_modle.willretain = "0";
    m5311_modle.willqos = "0";
    m5311_modle.willtopic = "\"\"";
    m5311_modle.willmessage = "\"\"";

    do{
    send_at("+MQTTOPEN: OK\r\n", 1000, 15, "AT+MQTTOPEN=", m5311_modle.usrflag, DOUHAO, \
                 m5311_modle.pwdflag, DOUHAO, m5311_modle.willflag, DOUHAO, \
                 m5311_modle.willretain, DOUHAO, m5311_modle.willqos, DOUHAO, \
                 m5311_modle.willtopic, DOUHAO, m5311_modle.willmessage,"\r\n");//连接MQTT
       cnt ++;
       if(cnt == 10)
       {
           LOG_E("mqtt open error!\r\n");
           return RT_ERROR;
       }
    } while(send_at("+MQTTSTAT: 5\r\n",100,1,"AT+MQTTSTAT?\r\n") != RT_EOK);//若MQTT服务器未连接
    cnt = 0;
    send_at("OK\r\n",100,3,"AT+MQTTSUB=",IMEI,",1\r\n");
    return RT_EOK;
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
        LOG_D("uart2 receive:%s",buffer);

        if(m5311_modle.compare_str != RT_NULL)
        {
           temp = strstr(buffer,m5311_modle.compare_str);
           if(temp)
           {
               if(index>15 && get_IMEI_flag)
               {
                   IMEI[0] = '\"';
                   IMEI[16] = IMEI[0];
                   IMEI[17] = '\0';


                   for(rt_uint8_t i = 0; i < 15; i ++)
                   {
                       IMEI[i+1] = buffer[i + 2];

                   }
                   strcpy(urctopiccompare,IMEI);
                   urctopiccompare[0] = ',';
                   urctopiccompare[16] = urctopiccompare[0];
                   urctopiccompare[17] = '6';
                   urctopiccompare[18] = '\0';
                   LOG_I("IMEI=%s\n",IMEI);
                   LOG_I("urctopiccompare=%s\n",urctopiccompare);
               }
               rt_mb_send(G_UART_2.out_mb,temp);
           }
        }

        if(strstr(buffer,"+MQTTPUBLISH:"))
        {
            uint8_t Calibration=0;
            uint16_t location=0;
           rt_kprintf("+MQTTPUBLISH: location = %d\n",location-(uint16_t)buffer);
            location = strstr(buffer,urctopiccompare) - (uint16_t)buffer;
            if(location>0)
            {
                rt_kprintf("%s location = %d\n",urctopiccompare,location);
                for(uint8_t i = location+19; i < location+19+5; i ++)
                {
                    rt_kprintf("buffer[%d] = 0x%02x\n",i,buffer[i]);
                    Calibration ^= buffer[i];
                }
                rt_kprintf("buffer[%d] = 0x%02x\n",location+19+5,buffer[location+19+5]);
                if(Calibration == buffer[location+19+5])
                {
                    g_rowled_data.word32 = buffer[location+19+2] | buffer[location+19+3] | buffer[location+19+4];
                    rt_kprintf("Calibration success,g_rowled_data=0x%06X\n",g_rowled_data.word32);
                }
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
    LOG_D("AT_command:%s",at_send_buf);
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
        else
            return RT_EOK;
//        rt_kprintf("A=%s\r\n",m5311_modle.compare_str_location);
    }
    else
    {
        rt_thread_mdelay(timeout);
        return RT_EOK;
    }
}
