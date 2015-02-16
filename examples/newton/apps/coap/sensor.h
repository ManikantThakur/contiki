/* This file is the part of Newton's software stack,
   Copyright : Thinqbot Technologies Pvt. Ltd.
*/

#ifndef SENSOR_H
#define SENSOR_H
/*--------------------------------------------------------------------------------------------------*/
char *get_device_status();
int   get_motion();
int   get_contact();
int   get_glass_break();
int   get_gas_leak();
int   get_temperature();
int   get_pressure();
int   get_humidity();
int   get_luminance();
char *get_current();
/*---------------------------------------------------------------------------------------------------*/
#endif /* SENSOR_H */
