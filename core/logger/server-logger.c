/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include <string.h>

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])

#define MAX_PAYLOAD_LEN 256

#define LOGGER_SERVER_PORT 5500
#define LOGGER_CLIENT_PORT 5501

static struct uip_udp_conn *server_conn;
//
//PROCESS(udp_server_process, "UDP server process");
//AUTOSTART_PROCESSES(&resolv_process,&udp_server_process);

static void tcpip_handler(void) {
	static int seq_id;
	char buf[MAX_PAYLOAD_LEN];

	if (uip_newdata()) {
		((char *) uip_appdata)[uip_datalen()] = 0;

		PRINTF("%s", (char *) uip_appdata);

		//PRINTF("\nServer received: '%s' from ", (char *)uip_appdata);
		//PRINT6ADDR(&UIP_IP_BUF->srcipaddr);
		//PRINTF("\n");
		uip_ipaddr_copy(&server_conn->ripaddr, &UIP_IP_BUF->srcipaddr);

		/* Restore server connection to allow data from any node */
		memset(&server_conn->ripaddr, 0, sizeof(server_conn->ripaddr));
	}
}

PROCESS_THREAD(udp_server_process, ev, data)
{
#if UIP_CONF_ROUTER
	uip_ipaddr_t ipaddr;
#endif /* UIP_CONF_ROUTER */

	PROCESS_BEGIN();
	PRINTF("UDP server started\n");

#if RESOLV_CONF_SUPPORTS_MDNS
	resolv_set_hostname("contiki-logging-server");
#endif

	server_conn = udp_new(NULL, UIP_HTONS(LOGGER_CLIENT_PORT), NULL);
	udp_bind(server_conn, UIP_HTONS(LOGGER_SERVER_PORT));

	while(1) {
		PROCESS_YIELD();
		if(ev == tcpip_event)
		tcpip_handler();
	}

	PROCESS_END();
}
