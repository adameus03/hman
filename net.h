#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include <winsock2.h>

SOCKET get_socket_as_server();

SOCKET get_socket_as_client(const char* ip_addr);


#endif // NET_H_INCLUDED
