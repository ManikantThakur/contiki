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

//#define TQ_DEFINE_CLIENT() PROCESS_NAME(udp_client_process);
//#define TQ_START_CLIENT() process_start(&udp_client_process,NULL);
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])

static struct uip_udp_conn *server_conn;
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[UIP_LLH_LEN + UIP_IPH_LEN])

#define LOG_INFO(msg, ...)  \
		extern struct uip_udp_conn *client_conn; \
		printf("\nhello: ");\
		PRINT6ADDR(&client_conn->ripaddr);
//		sprintf (log_buf, "\t[%s %s][%s (%s) (%d)]\t:  " msg"\n", __DATE__, __TIME__, __FILE__, __func__ , __LINE__,##__VA_ARGS__); \
		uip_udp_packet_send(client_conn, log_buf, strlen(log_buf)); \
		printf("\nLSA : "); \
		PRINT6ADDR(&client_conn->ripaddr);
