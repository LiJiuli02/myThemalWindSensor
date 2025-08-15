#include "tcp_client.h"

/* 需要自己设置远程IP地址 */
#define IP_ADDR   "192.168.31.31"
// #define IP_ADDR   "192.168.163.44"

#define LWIP_DEMO_RX_BUFSIZE         100                        /* 最大接收数据长度 */
#define LWIP_DEMO_PORT               8080                       /* 连接的本地端口号 */

/* 数据发送标志位 */
uint8_t g_lwip_send_flag;
int g_sock = -1;
int g_lwip_connect_state = 0;


/**
 * @brief       lwip_demo实验入口，传入任意字符串进行发送
 * @param       void
 * @retval      无
 */
void mYtcp_init(void)
{
    struct sockaddr_in atk_client_addr;
    err_t err;
    
sock_start:
    g_lwip_connect_state = 0;
    atk_client_addr.sin_family = AF_INET;                   /* 表示IPv4网络协议 */
    atk_client_addr.sin_port = htons(LWIP_DEMO_PORT);       /* 端口号 */
    atk_client_addr.sin_addr.s_addr = inet_addr(IP_ADDR);   /* 远程IP地址 */
    g_sock = socket(AF_INET, SOCK_STREAM, 0);               /* 可靠数据流交付服务既是TCP协议 */
    memset(&(atk_client_addr.sin_zero), 0, sizeof(atk_client_addr.sin_zero));
    
    /* 连接远程IP地址 */
    err = connect(g_sock, (struct sockaddr *)&atk_client_addr, sizeof(struct sockaddr));

    if (err == -1)
    {
        g_sock = -1;
        closesocket(g_sock);
        vTaskDelay(10);
        goto sock_start;
    }

    g_lwip_connect_state = 1;
        
}