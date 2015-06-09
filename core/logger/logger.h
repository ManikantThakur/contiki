#include "net/ip/uip-debug.h"
#include "net/ip/resolv.h"

#define LOGGER_SERVER_PORT 5500
#define LOGGER_CLIENT_PORT 5501

#define SEND_INTERVAL		1 * CLOCK_SECOND
#define MAX_PAYLOAD_LEN		256

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])

#define LOG_INFO(msg, ...)  \
	do { \
            sprintf (buf, "\n[%s %s][%s (%s) (%d)]\t: [INFO]  "msg"  ", __DATE__, __TIME__, __FILE__, __func__ , __LINE__, ##__VA_ARGS__); \
        	uip_udp_packet_send(client_conn, buf, UIP_APPDATA_SIZE); \
	} while(0)

static void tcpip_handler(void);
static void timeout_handler(void);
static void set_global_address(void);
static char buf[MAX_PAYLOAD_LEN];

struct uip_udp_conn *client_conn;
static struct uip_udp_conn *server_conn;
