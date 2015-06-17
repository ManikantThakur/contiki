#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/rpl/rpl.h"
#include "net/ip/uip.h"

#include "test.h"
#if UDP_CLIENT_STORE_RADIO_INFO
#include "dev/radio-sensor.h"
#endif

#include <string.h>

#define DEBUG 1
#include "logger/logger.h"

//uip_ip6addr_t server_ip = {(char *uint8_t),}
//#define SERVER_IP "aaaa::200:0:0:1"

extern uip_ds6_prefix_t uip_ds6_prefix_list[];
uip_ip6addr_t dest_addr;
uint8_t use_user_dest_addr = 0;
uip_ip6addr_t user_dest_addr;
uint16_t user_dest_port = LOGGING_PORT;
uint8_t udp_client_run = 0;
clock_time_t udp_interval = UDP_PERIOD * CLOCK_SECOND;
//struct uip_udp_conn *client_conn;

#if PLATFORM_HAS_RADIO && UDP_CLIENT_STORE_RADIO_INFO
int udp_client_lqi = 0;
int udp_client_rssi = 0;
#endif
static struct uip_udp_conn *client_conn;
//struct uip_udp_conn *get_client_conn(void) {
//	LOG_INFO("\nIP : %s :\n", &UIP_IP_BUF->srcipaddr);
//	return &UIP_IP_BUF->srcipaddr;
//return client_conn;
//}

/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client process");
//PROCESS_NAME (udp_client_process);
//process_start(&udp_client_process,NULL);
//AUTOSTART_PROCESSES (&udp_client_process);
/*---------------------------------------------------------------------------*/
void send_message(char *input) {
	LOG_INFO("%s", input);
}
static void tcpip_handler(void) {
	LOG_INFO("In TCP handler\n");
	char *str;

	if (uip_newdata()) {
		str = uip_appdata;
		str[uip_datalen()] = '\0';

#if PLATFORM_HAS_RADIO && UDP_CLIENT_STORE_RADIO_INFO
		udp_client_lqi = radio_sensor.value(RADIO_SENSOR_LAST_PACKET);
		udp_client_rssi = radio_sensor.value(RADIO_SENSOR_LAST_VALUE);
#endif
	}
}
/*---------------------------------------------------------------------------*/
char *
add_ipaddr(char * buf, const uip_ipaddr_t *addr) {
	uint16_t a;
	unsigned int i;
	int f;
	char *p = buf;

	for (i = 0, f = 0; i < sizeof(uip_ipaddr_t); i += 2) {
		a = (addr->u8[i] << 8) + addr->u8[i + 1];
		if (a == 0 && f >= 0) {
			if (f++ == 0) {
				p += sprintf(p, "::");
			}
		} else {
			if (f > 0) {
				f = -1;
			} else if (i > 0) {
				p += sprintf(p, ":");
			}
			p += sprintf(p, "%04x", a);
		}
	}
	return p;
}

void timeout_handler(char *log_msg) {
	static int seq_id;
	char buf[MAX_PAYLOAD_LEN];
	int i;
	uip_ip6addr_t *globaladdr = NULL;
	uint16_t dest_port = use_user_dest_addr ? user_dest_port : LOGGING_PORT;
	int has_dest = 0;
	uip_ipaddr_t * defrt;

	if (use_user_dest_addr) {
		uip_ipaddr_copy(&dest_addr, &user_dest_addr);
		has_dest = 1;
	} else {
		uip_ds6_addr_t * addr_desc = uip_ds6_get_global(ADDR_PREFERRED);
		if (addr_desc != NULL) {
			globaladdr = &addr_desc->ipaddr;
#if UIP_CONF_IPV6_RPL
			rpl_dag_t *dag = rpl_get_any_dag();
			if(dag) {
				uip_ipaddr_copy(&dest_addr, globaladdr);
				memcpy(&dest_addr.u8[8], &dag->dag_id.u8[8], sizeof(uip_ipaddr_t) / 2);
				has_dest = 1;
			}
#else
			defrt = uip_ds6_defrt_choose();
			if (defrt != NULL) {
				uip_ipaddr_copy(&dest_addr, globaladdr);
				memcpy(&dest_addr.u8[8], &defrt->u8[8],
						sizeof(uip_ipaddr_t) / 2);
				has_dest = 1;
			}
#endif
		}
	}

	if (has_dest) {
		if (client_conn == NULL) {
			PRINTF("UDP-CLIENT: address destination: ");
			PRINT6ADDR(&dest_addr);
			PRINTF("\n");
			client_conn = udp_new(&dest_addr, UIP_HTONS(dest_port), NULL);

			if (client_conn != NULL) {
				LOG_INFO("Created a connection with the Log server ");
				PRINT6ADDR(&client_conn->ripaddr);
				PRINTF(" local/remote port %u/%u\n",
						UIP_HTONS(client_conn->lport),
						UIP_HTONS(client_conn->rport));
			} else {
				PRINTF("Could not open connection\n");
			}
		} else {
			if (memcmp(&client_conn->ripaddr, &dest_addr, sizeof(uip_ipaddr_t))
					!= 0) {
				PRINTF("UDP-CLIENT: new address destination: ");
				PRINT6ADDR(&dest_addr);
				PRINTF("\n");
				uip_udp_remove(client_conn);
				client_conn = udp_new(&dest_addr, UIP_HTONS(dest_port), NULL);

				if (client_conn != NULL) {
					LOG_INFO("Created a connection with the Log server ");
					PRINT6ADDR(&client_conn->ripaddr);
					PRINTF(" local/remote port %u/%u\n",
							UIP_HTONS(client_conn->lport),
							UIP_HTONS(client_conn->rport));
				} else {
					PRINTF("Could not open connection\n");
				}
			}
		}
		PRINT6ADDR(&client_conn->ripaddr);
		if (client_conn != NULL) {
			PRINT6ADDR(&client_conn->ripaddr);

			if (udp_client_run) {

				sprintf(buf, log_msg);
#if SEND_TOO_LARGE_PACKET_TO_TEST_FRAGMENTATION
				uip_udp_packet_send(client_conn, buf, UIP_APPDATA_SIZE);
#else /* SEND_TOO_LARGE_PACKET_TO_TEST_FRAGMENTATION */
				uip_udp_packet_send(client_conn, buf, strlen(buf));
#endif /* SEND_TOO_LARGE_PACKET_TO_TEST_FRAGMENTATION */

			}
		} else {
			PRINTF("No connection created\n");
		}
	} else {
		PRINTF("No address configured\n");
	}
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
	static struct etimer et;

	PROCESS_BEGIN();
	LOG_INFO("UDP client process started\n");
	memset(&dest_addr, 0, sizeof(uip_ipaddr_t));

	udp_client_run = 1;

	etimer_set(&et, udp_interval);
	while (1) {
		PROCESS_YIELD();
		if (etimer_expired(&et)) {
			timeout_handler("");
			etimer_set(&et, udp_interval);
		} else if (ev == tcpip_event) {
			tcpip_handler();
		}
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
