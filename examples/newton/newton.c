#include "contiki.h"
#include "net/ip/uip-debug.h"
#include "logger/logger.h"
#include "logger/test.h"

#define COAPSERVER 1
//PROCESS_NAME (udp_client_process);
TQ_DEFINE_CLIENT();
PROCESS(demo_newton_process, "Thinqbot Newton");
AUTOSTART_PROCESSES (&demo_newton_process);
#if COAPSERVER
PROCESS_NAME (thinqbot_server);
#endif
/*---------------------------------------------------------------------------*/
void start_apps(void) {
#if COAPSERVER
	process_start(&thinqbot_server, NULL);
#endif
}

PROCESS_THREAD(demo_newton_process, ev, data)
{
	PROCESS_BEGIN();
	TQ_START_CLIENT();
	LOG_INFO("Starting newton_process.");
	start_apps();
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
