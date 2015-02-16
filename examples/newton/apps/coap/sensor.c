#include <stdlib.h>
#include <string.h>
#include "stdio.h"
char *get_device_status()
     {
       return("{\"mote_id\":\"MMX21\",\"transducers\":[{\"transducer_id\":\"STM02\",\"hw_port\":4},{\"transducer_id\":\"SPR02\",\"hw_port\":5},{\"transducer_id\":\"SHU02\",\"hw_port\":6},{\"transducer_id\":\"SLU02\",\"hw_port\":7}] }");
     }

int get_motion()
     {
       int r;
       r = rand()%100 +1 ;
       if(r<50)
       return 0;
       else
       return 1;
     }
int get_contact()
     {
       int r;
       r = rand()%100 +1 ;
       if(r<50)
       return 0;
       else
       return 1;
     }
int get_glass_break()
     {
       int r;
       r = rand()%1000 +200 ;
       if(r<0)
         r = -(r);
       //printf("Random no %d\n",rand()%1000);
       return(r);
     }
int get_gas_leak()
     {
       int r;
       r = rand()%1000 +50 ;
        if(r<0)
         r = -(r);
       return(r);
     }
int get_temperature()
     {
       int r;
       r = rand()%40 +5 ;
        if(r<0)
         r = -(r);
       return(r);
     }
int get_pressure()
     {
       int r;
       r = rand()%1500 +200 ;
        if(r<0)
         r = -(r);
       return(r);
     }
int get_humidity()
     {
       int r;
       r = rand()%40 +10 ;
        if(r<0)
         r = -(r);
       return(r);
     }
int get_luminance()
     {
       int r;
       r = rand()%80 +10 ;
        if(r<0)
         r = -(r);
       return(r);
     }
char *get_current()
     {
       char buff[100];
       sprintf(buff,"100205");
       return(buff);
     }

