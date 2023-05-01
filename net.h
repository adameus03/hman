#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include <winsock2.h>

SOCKET get_socket_as_server(const char* local_ip_addr, const int& local_port);

SOCKET get_socket_as_client(const char* remote_ip_addr, const int& remote_port);


#endif // NET_H_INCLUDED
