#ifndef CONFIG_H
#define CONFIG_H

#define ACTUATORS 0
#define SENSORS 1

static struct uip_udp_conn *client_conn = NULL;

#define LOG_INFO(msg, ...)  \
        sprintf (log_buf, "\t[%s %s][%s (%s) (%d)]\t:  " msg"\n", __DATE__, __TIME__, __FILE__, __func__ , __LINE__, ##__VA_ARGS__); \
        uip_udp_packet_send(client_conn, log_buf, strlen(log_buf));

#endif
