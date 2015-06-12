#include "contiki.h"
#include "net/ip/uip-debug.h"
#include "logger/logger.h"
#include "logger/test.h"

#define COAPSERVER 1

#if COAPSERVER
PROCESS_NAME (thinqbot_server);
#endif

void start_apps(void) {
	#if COAPSERVER
		process_start(&thinqbot_server, NULL);
	#endif
}
PROCESS_NAME(udp_client_process);

PROCESS(demo_newton_process, "Thinqbot Newton");
AUTOSTART_PROCESSES (&demo_newton_process);
PROCESS_THREAD(demo_newton_process, ev, data)
{
	PROCESS_BEGIN();
	process_start(&udp_client_process,NULL);
	LOG_INFO("Starting newton_process\n");
	PRINT6ADDR(&client_conn->ripaddr);
	start_apps();
	PROCESS_END();
}
