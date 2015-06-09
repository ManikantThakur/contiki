#include "contiki.h"
#include "net/ip/uip-debug.h"

#define COAPSERVER 1 

PROCESS_NAME(udp_client_process);
PROCESS(demo_newton_process, "Thinqbot Newton");
AUTOSTART_PROCESSES(&demo_newton_process);
#if COAPSERVER
PROCESS_NAME(thinqbot_server);
#endif
/*---------------------------------------------------------------------------*/
void
start_apps(void)
{
 #if COAPSERVER
  process_start(&thinqbot_server, NULL);
 #endif
}

PROCESS_THREAD(demo_newton_process, ev, data)
{
  PROCESS_BEGIN();
  PRINTF("Starting udp_client_process.");
  process_start(&udp_client_process,NULL);
  start_apps();
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
