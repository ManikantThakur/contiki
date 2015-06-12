#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include <string.h>

#define DEBUG 1
#include "logger/logger.h"

PROCESS(udp_server_process, "UDP server process");

static void tcpip_handler(void) {
	static int seq_id;
	char buf[MAX_PAYLOAD_LEN];

	if (uip_newdata()) {
		((char *) uip_appdata)[uip_datalen()] = 0;

		/**
		 * PRINT the Mote Address of Sender and the INCOMING Message.
		 * Can also be saved in CFS. To Be Implemented.
		 */

		PRINT6ADDR(&UIP_IP_BUF->srcipaddr);
		PRINTF(" : %s ", (char *) uip_appdata);

		/**
		 * END
		 */

		uip_ipaddr_copy(&server_conn->ripaddr, &UIP_IP_BUF->srcipaddr);
		server_conn->rport = UIP_UDP_BUF->srcport;

		uip_udp_packet_send(server_conn, buf, strlen(buf));

		/* Restore server connection to allow data from any node */
		memset(&server_conn->ripaddr, 0, sizeof(server_conn->ripaddr));
		server_conn->rport = 0;
	}
}

PROCESS_THREAD(udp_server_process, ev, data)
{
	PROCESS_BEGIN();
	LOG_INFO("Starting UDP Server Process\n");

	server_conn = udp_new(NULL, 0, NULL);
	udp_bind(server_conn, UIP_HTONS(LOGGING_PORT));

	while(1) {
		PROCESS_YIELD();
		if(ev == tcpip_event) {
			tcpip_handler();
		}
	}
	PROCESS_END();
}

