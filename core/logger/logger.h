#include "net/ip/uip.h"
#include "net/ip/uipopt.h"
#include "net/ip/uip-debug.h"
#include "test.h"

#ifndef LOGGING_PORT
#define LOGGING_PORT 3000
#endif

#ifndef UDP_PERIOD
#define UDP_PERIOD 1
#endif

#define SEND_INTERVAL    (UDP_PERIOD * CLOCK_SECOND)
#define MAX_PAYLOAD_LEN 2048

char log_buf[MAX_PAYLOAD_LEN];

#define TQ_DEFINE_CLIENT() PROCESS_NAME(udp_client_process);
#define TQ_START_CLIENT() process_start(&udp_client_process,NULL);

//extern uip_udp_conn *client_conn;
static struct uip_udp_conn *client_conn;
static struct uip_udp_conn *server_conn;
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[UIP_LLH_LEN + UIP_IPH_LEN])

#define LOG_INFO(msg, ...)  \
        sprintf (log_buf, "\t[%s %s][%s (%s) (%d)]\t:  " msg"\n", __DATE__, __TIME__, __FILE__, __func__ , __LINE__, ##__VA_ARGS__); \
        printf("\nLogger.h: %s %s \n",client_conn, log_buf);

//uip_udp_packet_send(get_client_conn(), log_buf, strlen(log_buf));
