/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-30     afun       the first version
 */
#ifndef APPLICATIONS_APP_M5311_H_
#define APPLICATIONS_APP_M5311_H_

#include <rtthread.h>
#include "rtdevice.h"
#include <board.h>
#include <drv_common.h>
#include "bsp_uart.h"





#define AT_SEND_BUF 512//AT指令最大长度
#define DOUHAO ","
#define DENNGHAO "="

struct M5311_Modle{
    char *compare_str_location;//被查询字符串首次出现的地址
    char *compare_str;//被查询字符串

    //AT+MQTTCFG
    char *mqtt_host;//服务器IP地址，不超过65字节
    char *mqtt_port;//端口号，[0:65535]，不超过5个字符
    char *mqtt_clientid;//客户端ID
    char *keepalive;//keepalive时间
    char *user;//用户名
    char *passwd;//密码
    char *clean;//是否删除session

    //AT+MQTTOPEN
    char *usrflag;//该标志位被设置表示一个客户端需使用用户名。
    char *pwdflag;//该标志位被设置表示一个客户端需使用密码。
    char *willflag;//被设置为 1 后，当服务器与客户端通信遇到 I/O 错误或客户端没有在一定时间计划内保持连接时，会发布一个message
    char *willretain;//当客户端意外的断开连接时，服务器是否应该保留被服务器发布的 Will message
    char *willqos;//指定要发布的消息使用 Qos 级别。
    char *willtopic;//Will message 的主题名称
    char *willmessage;//will 消息体

    //AT+MQTTSUB
    char *mqtt_sub_topic;//订阅的主题
    char *mqtt_sub_qos;//订阅的主题的消息等级，“0”，“1”，“2”
    char *mqtt_sub_code;//服务器反馈码,0/1/2：订阅成功 QoS0/1/2，80:失败

    //AT+MQTTPUB
    char *mqtt_pub_topic;//发布的主题，不超过60个字节
    char *mqtt_pub_qos;//发布的主题的消息等级，“0”，“1”，“2”
    char *mqtt_pub_retain;//服务器是否储存该消息，并向新订阅者发送最新订阅消息， 0-1。
    char *mqtt_pub_dup;//重发标志， 0-1
    char *mqtt_msg_len;//输入消息类型或长度，0：字符串格式输入，非0：16 进制字符串输入； 长度为输入的一半
    char *mqtt_pub_message;//消息内容

    //AT_GSN
    char *model_imei;

    //AT+CIMI
    char *sim_imsi;
};


int m5311_moudle_init(void);
rt_size_t send_at(char *ack, rt_uint32_t timeout, int num, ...);
#endif /* APPLICATIONS_APP_M5311_H_ */
