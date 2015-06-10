/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
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
 */

/**
 * \file
 *      Erbium (Er) REST Engine example.
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"
#include "logger/logger.h"
#if PLATFORM_HAS_BUTTON
#include "dev/button-sensor.h"
#endif

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif
/*-----------------------------------------------------------------------------------------------*/
extern resource_t res_handshake, res_event_sen0, res_event_sen1, res_event_sen2,
		res_event_sen3, res_event_sen4, res_event_sen5, res_event_sen6,
		res_event_sen7, res_event_sen8;
/*-----------------------------------------------------------------------------------------------------*/

//PROCESS_NAME (udp_client_process);
PROCESS_NAME (sen0_process);
PROCESS_NAME (sen1_process);
PROCESS_NAME (sen2_process);
PROCESS_NAME (sen3_process);
PROCESS_NAME (sen4_process);
PROCESS_NAME (sen5_process);
PROCESS_NAME (sen6_process);
PROCESS_NAME (sen7_process);
PROCESS_NAME (sen8_process);
PROCESS(thinqbot_server, "Erbium Example Server");
//AUTOSTART_PROCESSES(&er_example_server);

PROCESS_THREAD(thinqbot_server, ev, data)
{
	PROCESS_BEGIN();

//	PRINTF("Starting udp_client_process\n");
//	process_start(&udp_client_process,NULL);
	LOG_INFO("Thinqbot MANIKANT");

	PROCESS_PAUSE();

	PRINTF("Starting Erbium Example Server\n");

#ifdef RF_CHANNEL
	PRINTF("RF channel: %u\n", RF_CHANNEL);
#endif
#ifdef IEEE802154_PANID
	PRINTF("PAN ID: 0x%04X\n", IEEE802154_PANID);
#endif

	PRINTF("uIP buffer: %u\n", UIP_BUFSIZE);
	PRINTF("LL header: %u\n", UIP_LLH_LEN);
	PRINTF("IP+UDP header: %u\n", UIP_IPUDPH_LEN);
	PRINTF("REST max chunk: %u\n", REST_MAX_CHUNK_SIZE);

	/* Initialize the REST engine. */
	rest_init_engine();
	process_start(&sen4_process,NULL);
	process_start(&sen5_process,NULL);
	process_start(&sen6_process,NULL);
	process_start(&sen7_process,NULL);
	rest_activate_resource(&res_handshake, "dev/handshake");
	rest_activate_resource(&res_event_sen4,"sen/4");
	rest_activate_resource(&res_event_sen5,"sen/5");
	rest_activate_resource(&res_event_sen6,"sen/6");
	rest_activate_resource(&res_event_sen7,"sen/7");
	while(1) {
		PROCESS_WAIT_EVENT();
#if PLATFORM_HAS_BUTTON
		if(ev == sensors_event && data == &button_sensor)
		PRINTF("*******BUTTON*******\n");
#endif /* PLATFORM_HAS_BUTTON */
	} /* while (1) */

	PROCESS_END();
}
