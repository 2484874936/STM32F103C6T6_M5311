#include "bsp_uart.h"
#include <stdlib.h>
#include <string.h>

//#define RT_SERIAL_CONFIG_DEFAULT    \
//{                                   \
//    115200,    /* 115200 bits/s */  \
//    8,          /* 8 databits */    \
//    0,      /* 1 stopbit */         \
//    0,      /* No parity  */        \
//    0,    /* LSB first sent */      \
//    0,       /* Normal mode */      \
//    1024, /* Buffer size, "+1" is Prepared to "\0" */ \
//    0                               \
//}
//
//struct serial_configure
//{
//    rt_uint32_t baud_rate;//������
//    rt_uint32_t data_bits :4;//����λ
//    rt_uint32_t stop_bits :2;//ֹͣλ
//    rt_uint32_t parity :2;//У��λ
//    rt_uint32_t bit_order :1;
//    rt_uint32_t invert :1;
//    rt_uint32_t bufsz :16;
//    rt_uint32_t reserved :6;
//};

#if defined(USING_UART1)
void uart1_baud_rate_set(void);

static rt_size_t uart1_send(void *data, rt_size_t size);
static rt_size_t uart1_recv(char *buffer, rt_int32_t timeout);
static rt_err_t uart1_callback(rt_device_t dev, rt_size_t size);
__WEAK int uart1_data_processing(char *buffer, rt_size_t index);
static int uart1_init(void);

uart_t G_UART_1 = {
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        uart1_send,
#if defined(BSP_UART1_RX_USING_DMA)
        uart1_recv,
#else
        NULL,
#endif
        uart1_callback,
        uart1_init,
        uart1_data_processing};

static rt_size_t uart1_send(void *data, rt_size_t size)
{
    return rt_device_write(G_UART_1.serial, 0, data, size);
}


static rt_size_t uart1_recv(char *buffer, rt_int32_t timeout)
{
    rt_size_t len = 0;
#if defined(BSP_UART1_RX_USING_DMA)
    if (rt_mb_recv(G_UART_1.dma_mb, &len, timeout) != RT_EOK)
    {
        return 0;
    }
    len = rt_device_read(G_UART_1.serial, 0, buffer, len);
#endif
    return len;
}


/* �������ݻص����� */
static rt_err_t uart1_callback(rt_device_t dev, rt_size_t size)
{
#if defined(BSP_UART1_RX_USING_DMA)
    /* �����ʼ� */
    return rt_mb_send(G_UART_1.dma_mb, size);
#else
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    return rt_sem_release(G_UART_1.rx_sem);
#endif
}

__WEAK int uart1_data_processing(char *buffer, rt_size_t index)
{
#if defined(BSP_UART1_RX_USING_DMA)
    if(index)
#else
    if(buffer[index-2] == '\r' && buffer[index-1] == '\n')//�жϱ���Ҫ���Ʋ�������ַ���
#endif
    {
        rt_kprintf("usart1 ");
        G_UART_1.send(buffer,index);
//        rt_mb_send(G_UART_1.out_mb, (rt_uint32_t)buffer);
        rt_memset(buffer, 0, index);
        index = 0;
    }
    return index;
}

static void uart1_rev_thread(void *parameter)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    //    config.bufsz = 100;//���ý��ջ�������С,��������Ĭ�ϴ�С
    char uart1rev[config.bufsz];
    memset(uart1rev,0,config.bufsz);
    rt_size_t len1=0;
    char ch;

    for(;;)
    {
#if defined(BSP_UART1_RX_USING_DMA)
        //DMA�������ݴ�������
        len1 = G_UART_1.recv((char *)&uart1rev,RT_WAITING_FOREVER);
        if(len1)
        {
            len1 = G_UART_1.data_processing((char *)&uart1rev,len1);
        }
#else
        //�ж����ݴ�������
        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(G_UART_1.serial, -1, &ch, 1) != 1)
        {
            /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
            rt_sem_take(G_UART_1.rx_sem, RT_WAITING_FOREVER);
        }
        /* ��ȡ��������ͨ��������� */
//        G_UART_1.send(&ch,1);
        uart1rev[len1] = ch;
        len1 %= config.bufsz-1;
        len1 ++;
        len1 = G_UART_1.data_processing((char *)&uart1rev,len1);
#endif
    }
}


int uart1_init(void)
{
   /* ����ϵͳ�еĴ����豸 */
    G_UART_1.serial = rt_device_find(UART1_NAME);
    if (!G_UART_1.serial)
    {
        rt_kprintf("find %s failed!\n", UART1_NAME);
        return RT_ERROR;
    }
//    uart1_baud_rate_set();
    uart_baud_rate_set(G_UART_1,UART1_DEFAULT_BAUD_RATE);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(G_UART_1.serial, uart1_callback);
#if defined(BSP_UART1_RX_USING_DMA)
    /* ��DMA���ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(G_UART_1.serial, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_DMA_RX );
#else
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(G_UART_1.serial, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
#endif
    /* ��ʼ��������� */
    if (G_UART_1.out_mb == RT_NULL)
    {
        G_UART_1.out_mb = rt_mb_create("uart1_out_mb", 2, RT_IPC_FLAG_FIFO);
        if (G_UART_1.out_mb == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ��ʼ��DMA���� */
    if (G_UART_1.dma_mb == RT_NULL)
    {
        G_UART_1.dma_mb = rt_mb_create("uart1_dma_mb", 10, RT_IPC_FLAG_FIFO);
        if (G_UART_1.dma_mb == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ��ʼ���ź��� */
    if (G_UART_1.rx_sem == RT_NULL)
    {
        G_UART_1.rx_sem = rt_sem_create( "uart1_rx_sem", 0, RT_IPC_FLAG_FIFO);
        if (G_UART_1.rx_sem == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("serial1", uart1_rev_thread, RT_NULL, 2048, 21, 10);
    /* �����ɹ��������߳� */
    if (thread != RT_NULL)
    {
       rt_thread_startup(thread);
    }
    else
    {
       rt_kprintf("Create %s Entry failed!\n", UART1_NAME);
       return RT_ERROR;
    }
    return RT_EOK;
}
#endif



#if defined(USING_UART2)
static rt_size_t uart2_send(void *data, rt_size_t size);
static rt_size_t uart2_recv(char *buffer, rt_int32_t timeout);
static rt_err_t uart2_callback(rt_device_t dev, rt_size_t size);
__WEAK int uart2_data_processing(char *buffer, rt_size_t index);
static int uart2_init(void);

uart_t G_UART_2 = {
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        uart2_send,
#if defined(BSP_UART2_RX_USING_DMA)
        uart2_recv,
#else
        NULL,
#endif
        uart2_callback,
        uart2_init,
        uart2_data_processing };

static rt_size_t uart2_send(void *data, rt_size_t size)
{
    return rt_device_write(G_UART_2.serial, 0, data, size);
}


static rt_size_t uart2_recv(char *buffer, rt_int32_t timeout)
{
    rt_size_t len = 0;
#if defined(BSP_UART2_RX_USING_DMA)
    if (rt_mb_recv(G_UART_2.dma_mb, &len, timeout) != RT_EOK)
    {
        return 0;
    }
    len = rt_device_read(G_UART_2.serial, 0, buffer, len);
#endif
    return len;
}


/* �������ݻص����� */
static rt_err_t uart2_callback(rt_device_t dev, rt_size_t size)
{
#if defined(BSP_UART2_RX_USING_DMA)
    /* �����ʼ� */
    return rt_mb_send(G_UART_2.dma_mb, size);
#else
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    return rt_sem_release(G_UART_2.rx_sem);
#endif
}

__WEAK int uart2_data_processing(char *buffer, rt_size_t index)
{
#if defined(BSP_UART2_RX_USING_DMA)
    if(index)
#else
    if(buffer[index-2] == '\r' && buffer[index-1] == '\n')//�жϱ���Ҫ���Ʋ�������ַ���
#endif
    {
        rt_kprintf("%s",buffer);
//        rt_uprintf(G_UART_2, "usart2 ");
//        G_UART_2.send(buffer,index);
//        rt_mb_send(G_UART_2.out_mb, (rt_uint32_t)buffer);
        rt_memset(buffer, 0, index);
        index = 0;
    }
    return index;
}

static void uart2_rev_thread(void *parameter)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    //    config.bufsz = 100;//���ý��ջ�������С,��������Ĭ�ϴ�С
    char uart2rev[config.bufsz];
    memset(uart2rev,0,config.bufsz);
    rt_size_t len2=0;
    char ch;

    for(;;)
    {
#if defined(BSP_UART2_RX_USING_DMA)
        //DMA�������ݴ�������
        len2 = G_UART_2.recv((char *)&uart2rev,RT_WAITING_FOREVER);
        if(len2)
        {
            len2 = G_UART_2.data_processing((char *)&uart2rev,len2);
        }
#else
        //�ж����ݴ�������
        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(G_UART_2.serial, -1, &ch, 1) != 1)
        {
            /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
            rt_sem_take(G_UART_2.rx_sem, RT_WAITING_FOREVER);
        }
        /* ��ȡ��������ͨ��������� */
//        G_UART_2.send(&ch,1);
        uart2rev[len2] = ch;
        len2 %= config.bufsz-1;
        len2 ++;
        len2 = G_UART_2.data_processing((char *)&uart2rev,len2);
#endif
    }
}


int uart2_init(void)
{
    /* ����ϵͳ�еĴ����豸 */
    G_UART_2.serial = rt_device_find(UART2_NAME);
    if (!G_UART_2.serial)
    {
        rt_kprintf("find %s failed!\n", UART2_NAME);
        return RT_ERROR;
    }
    uart_baud_rate_set(G_UART_2,UART2_DEFAULT_BAUD_RATE);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(G_UART_2.serial, uart2_callback);
#if defined(BSP_UART2_RX_USING_DMA)
    /* ��DMA���ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(G_UART_2.serial, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_DMA_RX );
#else
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(G_UART_2.serial, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
#endif
    /* ��ʼ��������� */
    if (G_UART_2.out_mb == RT_NULL)
    {
        G_UART_2.out_mb = rt_mb_create("uart2_out_mb", 1, RT_IPC_FLAG_FIFO);
        if (G_UART_2.out_mb == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ��ʼ��DMA���� */
    if (G_UART_2.dma_mb == RT_NULL)
    {
        G_UART_2.dma_mb = rt_mb_create("uart2_dma_mb", 1, RT_IPC_FLAG_FIFO);
        if (G_UART_2.dma_mb == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ��ʼ���ź��� */
    if (G_UART_2.rx_sem == RT_NULL)
    {
        G_UART_2.rx_sem = rt_sem_create( "uart2_rx_sem", 0, RT_IPC_FLAG_FIFO);
        if (G_UART_2.rx_sem == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("serial2", uart2_rev_thread, RT_NULL, 1024, 22, 10);
    /* �����ɹ��������߳� */
    if (thread != RT_NULL)
    {
       rt_thread_startup(thread);
       return RT_EOK;
    }
    else
    {
       rt_kprintf("Create %s Entry failed!\n", UART2_NAME);
       return RT_ERROR;
    }
}
#endif



#if defined(USING_UART3)

static rt_size_t uart3_send(void *data, rt_size_t size);
static rt_size_t uart3_recv(char *buffer, rt_int32_t timeout);
static rt_err_t uart3_callback(rt_device_t dev, rt_size_t size);
__WEAK int uart3_data_processing(char *buffer, rt_size_t index);
static int uart3_init(void);

uart_t G_UART_3 = {
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        uart3_send,
#if defined(BSP_UART3_RX_USING_DMA)
        uart3_recv,
#else
        NULL,
#endif
        uart3_callback,
        uart3_init,
        uart3_data_processing};

static rt_size_t uart3_send(void *data, rt_size_t size)
{
    return rt_device_write(G_UART_3.serial, 0, data, size);
}


static rt_size_t uart3_recv(char *buffer, rt_int32_t timeout)
{
    rt_size_t len = 0;
#if defined(BSP_UART3_RX_USING_DMA)
    if (rt_mb_recv(G_UART_3.dma_mb, &len, timeout) != RT_EOK)
    {
        return 0;
    }
    len = rt_device_read(G_UART_3.serial, 0, buffer, len);
#endif
    return len;
}


/* �������ݻص����� */
static rt_err_t uart3_callback(rt_device_t dev, rt_size_t size)
{
#if defined(BSP_UART3_RX_USING_DMA)
    /* �����ʼ� */
    return rt_mb_send(G_UART_3.dma_mb, size);
#else
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    return rt_sem_release(G_UART_3.rx_sem);
#endif
}


__WEAK int uart3_data_processing(char *buffer, rt_size_t index)
{
#if defined(BSP_UART3_RX_USING_DMA)
    if(index)
#else
    if(buffer[index-2] == '\r' && buffer[index-1] == '\n')//�жϱ���Ҫ���Ʋ�������ַ���
#endif
    {
        rt_uprintf(G_UART_3, "usart3 ");
        G_UART_3.send(buffer,index);
//        rt_mb_send(G_UART_3.out_mb, (rt_uint32_t)buffer);
        rt_memset(buffer, 0, index);
        index = 0;
    }
    return index;
}

static void uart3_rev_thread(void *parameter)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    //    config.bufsz = 100;//���ý��ջ�������С,��������Ĭ�ϴ�С
    char uart3rev[config.bufsz];
    memset(uart3rev,0,config.bufsz);
    rt_size_t len3=0;
    char ch;

    for(;;)
    {
#if defined(BSP_UART3_RX_USING_DMA)
        //DMA�������ݴ�������
        len3 = G_UART_3.recv((char *)&uart3rev,RT_WAITING_FOREVER);
        if(len3)
        {
            len3 = G_UART_3.data_processing((char *)&uart3rev,len3);
        }
#else
        //�ж����ݴ�������
        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(G_UART_3.serial, -1, &ch, 1) != 1)
        {
            /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
            rt_sem_take(G_UART_3.rx_sem, RT_WAITING_FOREVER);
        }
        /* ��ȡ��������ͨ��������� */
//        G_UART_3.send(&ch,1);
        uart3rev[len3] = ch;
        len3 %= config.bufsz-1;
        len3 ++;
        len3 = G_UART_3.data_processing((char *)&uart3rev,len3);
#endif
    }
}


int uart3_init(void)
{
    /* ����ϵͳ�еĴ����豸 */
    G_UART_3.serial = rt_device_find(UART3_NAME);
    if (!G_UART_3.serial)
    {
        rt_kprintf("find %s failed!\n", UART3_NAME);
        return RT_ERROR;
    }
    uart_baud_rate_set(G_UART_3,UART3_DEFAULT_BAUD_RATE);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(G_UART_3.serial, uart3_callback);
#if defined(BSP_UART3_RX_USING_DMA)
    /* ��DMA���ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(G_UART_3.serial, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_DMA_RX );
#else
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(G_UART_3.serial, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
#endif
    /* ��ʼ��������� */
    if (G_UART_3.out_mb == RT_NULL)
    {
        G_UART_3.out_mb = rt_mb_create("uart3_out_mb", 1, RT_IPC_FLAG_FIFO);
        if (G_UART_3.out_mb == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ��ʼ��DMA���� */
    if (G_UART_3.dma_mb == RT_NULL)
    {
        G_UART_3.dma_mb = rt_mb_create("uart3__dma_mb", 1, RT_IPC_FLAG_FIFO);
        if (G_UART_3.dma_mb == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ��ʼ���ź��� */
    if (G_UART_3.rx_sem == RT_NULL)
    {
        G_UART_3.rx_sem = rt_sem_create( "uart3_rx_sem", 0, RT_IPC_FLAG_FIFO);
        if (G_UART_3.rx_sem == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("serial3", uart3_rev_thread, RT_NULL, 2048, 23, 10);
    /* �����ɹ��������߳� */
    if (thread != RT_NULL)
    {
       rt_thread_startup(thread);
       return RT_EOK;
    }
    else
    {
       rt_kprintf("Create %s Entry failed!\n", UART3_NAME);
       return RT_ERROR;
    }
}
#endif


#if defined(USING_UART4)
static rt_size_t uart4_send(void *data, rt_size_t size);
static rt_size_t uart4_recv(char *buffer, rt_int32_t timeout);
static rt_err_t uart4_callback(rt_device_t dev, rt_size_t size);
__WEAK int uart4_data_processing(char *buffer, rt_size_t index);
static int uart4_init(void);

uart_t G_UART_4 = {
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        uart4_send,
#if defined(BSP_UART4_RX_USING_DMA)
        uart4_recv,
#else
        NULL,
#endif
        uart4_callback,
        uart4_init,
        uart4_data_processing};

static rt_size_t uart4_send(void *data, rt_size_t size)
{
    return rt_device_write(G_UART_4.serial, 0, data, size);
}


static rt_size_t uart4_recv(char *buffer, rt_int32_t timeout)
{
    rt_size_t len = 0;
#if defined(BSP_UART4_RX_USING_DMA)
    if (rt_mb_recv(G_UART_4.dma_mb, &len, timeout) != RT_EOK)
    {
        return 0;
    }
    len = rt_device_read(G_UART_4.serial, 0, buffer, len);
#endif
    return len;
}


/* �������ݻص����� */
static rt_err_t uart4_callback(rt_device_t dev, rt_size_t size)
{
#if defined(BSP_UART4_RX_USING_DMA)
    /* �����ʼ� */
    return rt_mb_send(G_UART_4.dma_mb, size);
#else
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    return rt_sem_release(G_UART_4.rx_sem);
#endif
}


__WEAK int uart4_data_processing(char *buffer, rt_size_t index)
{
#if defined(BSP_UART4_RX_USING_DMA)
    if(index)
#else
    if(buffer[index-2] == '\r' && buffer[index-1] == '\n')//�жϱ���Ҫ���Ʋ�������ַ���
#endif
    {
        rt_uprintf(G_UART_4, "uart4 ");
        G_UART_4.send(buffer,index);
//        rt_mb_send(G_UART_4.out_mb, (rt_uint32_t)buffer);
        rt_memset(buffer, 0, index);
        index = 0;
    }
    return index;
}

static void uart4_rev_thread(void *parameter)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    //    config.bufsz = 100;//���ý��ջ�������С,��������Ĭ�ϴ�С
    char uart4rev[config.bufsz];
    memset(uart4rev,0,config.bufsz);
    rt_size_t len4=0;
    char ch;

    for(;;)
    {
#if defined(BSP_UART4_RX_USING_DMA)
        //DMA�������ݴ�������
        len4 = G_UART_4.recv((char *)&uart4rev,RT_WAITING_FOREVER);
        if(len4)
        {
            len4 = G_UART_4.data_processing((char *)&uart4rev,len4);
        }
#else
        //�ж����ݴ�������
        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(G_UART_4.serial, -1, &ch, 1) != 1)
        {
            /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
            rt_sem_take(G_UART_4.rx_sem, RT_WAITING_FOREVER);
        }
        /* ��ȡ��������ͨ��������� */
//        G_UART_4.send(&ch,1);
        uart4rev[len4] = ch;
        len4 %= config.bufsz-1;
        len4 ++;
        len4 = G_UART_4.data_processing((char *)&uart4rev,len4);
#endif
    }
}


int uart4_init(void)
{
    /* ����ϵͳ�еĴ����豸 */
    G_UART_4.serial = rt_device_find(UART4_NAME);
    if (!G_UART_4.serial)
    {
        rt_kprintf("find %s failed!\n", UART4_NAME);
        return RT_ERROR;
    }
    uart_baud_rate_set(G_UART_4,UART4_DEFAULT_BAUD_RATE);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(G_UART_4.serial, uart4_callback);
#if defined(BSP_UART4_RX_USING_DMA)
    /* ��DMA���ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(G_UART_4.serial, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_DMA_RX );
#else
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(G_UART_4.serial, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
#endif
    /* ��ʼ��������� */
    if (G_UART_4.out_mb == RT_NULL)
    {
        G_UART_4.out_mb = rt_mb_create("uart4_out_mb", 1, RT_IPC_FLAG_FIFO);
        if (G_UART_4.out_mb == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ��ʼ��DMA���� */
    if (G_UART_4.dma_mb == RT_NULL)
    {
        G_UART_4.dma_mb = rt_mb_create("uart4_dma_mb", 1, RT_IPC_FLAG_FIFO);
        if (G_UART_4.dma_mb == RT_NULL)
        {
            return RT_ERROR;
        }
    }

    /* ��ʼ���ź��� */
    if (G_UART_4.rx_sem == RT_NULL)
    {
        G_UART_4.rx_sem = rt_sem_create( "uart4_rx_sem", 0, RT_IPC_FLAG_FIFO);
        if (G_UART_4.rx_sem == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("serial4", uart4_rev_thread, RT_NULL, 2048, 24, 10);
    /* �����ɹ��������߳� */
    if (thread != RT_NULL)
    {
       rt_thread_startup(thread);
       return RT_EOK;
    }
    else
    {
       rt_kprintf("Create %s Entry failed!\n", UART4_NAME);
       return RT_ERROR;
    }
}
#endif



#if defined(BSP_USING_UART5)
static rt_size_t uart5_send(void *data, rt_size_t size);
static rt_err_t uart5_callback(rt_device_t dev, rt_size_t size);
__WEAK int uart5_data_processing(char *buffer, rt_size_t index);
static int uart5_init(void);

uart_t G_UART_5 = {
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        uart5_send,
        NULL,
        uart5_callback,
        uart5_init,
        uart5_data_processing};

static rt_size_t uart5_send(void *data, rt_size_t size)
{
    return rt_device_write(G_UART_5.serial, 0, data, size);
}

/* �������ݻص����� */
static rt_err_t uart5_callback(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(G_UART_5.rx_sem);
    return RT_EOK;
}


__WEAK int uart5_data_processing(char *buffer, rt_size_t index)
{
    if(buffer[index-2] == '\r' && buffer[index-1] == '\n')//�жϱ���Ҫ���Ʋ�������ַ���
    {
        rt_uprintf(G_UART_5, "uart5 ");
        G_UART_5.send(buffer,index);
//        rt_mb_send(G_UART_5.out_mb, (rt_uint32_t)buffer);
        rt_memset(buffer, 0, index);
        index = 0;
    }
    return index;
}

static void uart5_rev_thread(void *parameter)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    //    config.bufsz = 100;//���ý��ջ�������С,��������Ĭ�ϴ�С
    char uart5rev[config.bufsz];
    memset(uart5rev,0,config.bufsz);
    rt_size_t len5=0;
    char ch;

    for(;;)
    {
        //�ж����ݴ�������
        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(G_UART_5.serial, -1, &ch, 1) != 1)
        {

            /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
            rt_sem_take(G_UART_5.rx_sem, RT_WAITING_FOREVER);
        }
        /* ��ȡ��������ͨ��������� */
//        G_UART_5.send(&ch,1);
        uart5rev[len5] = ch;
        len5 %= config.bufsz-1;
        len5 ++;
        len5 = G_UART_5.data_processing((char *)&uart5rev,len5);

    }
}

int uart5_init(void)
{
   /* ����ϵͳ�еĴ����豸 */
    G_UART_5.serial = rt_device_find(UART5_NAME);
    if (!G_UART_5.serial)
    {
       rt_kprintf("find %s failed!\n", UART5_NAME);
    }
    /* ��ʼ��������� */
    if (G_UART_5.out_mb == RT_NULL)
    {
        G_UART_5.out_mb = rt_mb_create("uart5_out_mb", 1, RT_IPC_FLAG_FIFO);
        if (G_UART_5.out_mb == RT_NULL)
        {
            return RT_ERROR;
        }
    }
    /* ��ʼ���ź��� */
    G_UART_5.rx_sem = rt_sem_create( "uart5_rx_sem", 0, RT_IPC_FLAG_FIFO);

    uart_baud_rate_set(G_UART_5,UART5_DEFAULT_BAUD_RATE);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(G_UART_5.serial, uart5_callback);
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
        rt_device_open(G_UART_5.serial, RT_DEVICE_FLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("serial5", uart5_rev_thread, RT_NULL, 2048, 25, 10);
    /* �����ɹ��������߳� */
    if (thread != RT_NULL)
    {
       rt_thread_startup(thread);
       return RT_EOK;
    }
    else
    {
       rt_kprintf("Create %s Entry failed!\n", UART5_NAME);
       return RT_ERROR;
    }
}
#endif




int uart_init(void)
{
#if defined(USING_UART1)
    if (G_UART_1.init() == RT_EOK)
    {
#if defined(BSP_UART1_RX_USING_DMA)
        rt_kprintf("uart1 interrupt init successful!\r\n");
        rt_uprintf(G_UART_1,"uart1 dma init successful!\r\n");
#else
        rt_kprintf("uart1 interrupt init successful!\r\n");
        rt_uprintf(G_UART_1,"uart1 interrupt init successful!\r\n");
#endif
    }
    else
    {
        rt_kprintf("uart1  init errorR!\r\n");
        return RT_ERROR;
    }
#endif

#if defined(USING_UART2)
    if (G_UART_2.init() == RT_EOK)
    {
#if defined(BSP_UART2_RX_USING_DMA)
        rt_kprintf("uart2 dma init successful!\r\n");
//        rt_uprintf(G_UART_2,"uart2 dma init successful!\r\n");
#else
        rt_kprintf("uart2 interrupt init successful!\r\n");
        rt_uprintf(G_UART_2,"uart2 interrupt init successful!\r\n");
#endif
    }
    else
    {
        rt_kprintf("uart2  init error!\r\n");
        return RT_ERROR;
    }
#endif

#if defined(USING_UART3)
    if (G_UART_3.init() == RT_EOK)
    {
#if defined(BSP_UART3_RX_USING_DMA)
        rt_kprintf("uart3 dma init successful!\r\n");
        rt_uprintf(G_UART_3,"uart3 dma init successful!\r\n");
#else
        rt_kprintf("uart3 interrupt init successful!\r\n");
        rt_uprintf(G_UART_3,"uart3 interrupt init successful!\r\n");
#endif
    }
    else
    {
        rt_kprintf("uart3  init errorR!\r\n");
        return RT_ERROR;
    }
#endif

#if defined(USING_UART4)
    if (G_UART_4.init() == RT_EOK)
    {
#if defined(BSP_UART4_RX_USING_DMA)
        rt_kprintf("uart4 dma init successful!\r\n");
        rt_uprintf(G_UART_4,"uart4 dma init successful!\r\n");
#else
        rt_kprintf("uart4 interrupt init successful!\r\n");
        rt_uprintf(G_UART_4,"uart4 interrupt init successful!\r\n");
#endif
    }
    else
    {
        rt_kprintf("uart4  init errorR!\r\n");
        return RT_ERROR;
    }
#endif

#if defined(BSP_USING_UART5)
    if (G_UART_5.init() == RT_EOK)
    {
        rt_kprintf("uart5 interrupt init successful!\r\n");
        rt_uprintf(G_UART_5,"uart5 interrupt init successful!\r\n");
    }
    else
    {
        rt_kprintf("uart5  init errorR!\r\n");
        return RT_ERROR;
    }
#endif
    return RT_EOK;
}
//INIT_APP_EXPORT(uart_init); /* ʹ������Զ���ʼ������ */



int uart_baud_rate_set(uart_t uart, rt_uint32_t baud_rate)
{
    RT_ASSERT(uart.serial != RT_NULL);
    RT_ASSERT(baud_rate != RT_NULL);
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    config.baud_rate = baud_rate;        //�޸Ĳ�����
    rt_device_control(uart.serial, RT_DEVICE_CTRL_CONFIG, &config);
}


/**
 * This function will print a formatted string on system console.
 *
 * @param uart is the device of serial.
 * @param fmt is the format parameters.
 *
 * @return The number of characters actually written to buffer.
 */
int rt_uprintf(uart_t uart,const char *fmt, ...)
{
    RT_ASSERT(uart.serial != RT_NULL);
    va_list args;
    rt_size_t length = 0;
    static char rt_log_buf[UART_TX_SIZE];

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
    length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > UART_TX_SIZE - 1)
        length = UART_TX_SIZE - 1;
    rt_device_write(uart.serial, 0, rt_log_buf, length);
    va_end(args);
    return length;
}
RTM_EXPORT(rt_uprintf);

