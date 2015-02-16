#include "contiki.h"

#define COAPSERVER 1 

PROCESS(demo_newton_process, "6LBR Demo");
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
      
           start_apps();

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
