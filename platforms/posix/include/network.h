/* network.h - Define and implement the network endpoint. */

#ifndef __lf_network_h__
#define __lf_network_h__

/* POSIX network includes. */
#include <arpa/inet.h>
#include <netdb.h>

/* The default port over which FMR can be accessed. */
#define LF_UDP_PORT 3258

struct _lf_network_context {
    int fd;
    char host[64];
    struct sockaddr_in device;
};

int lf_network_read(struct _lf_device *device, void *dst, uint32_t length);
int lf_network_write(struct _lf_device *device, void *src, uint32_t length);
int lf_network_release(void *device);

struct _lf_device *lf_network_device_for_hostname(char *hostname);

#endif
