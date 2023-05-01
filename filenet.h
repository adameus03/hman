#ifndef FILENET_H_INCLUDED
#define FILENET_H_INCLUDED

#include "net.h"
#include <winsock2.h>
#include <cstring>
#include <fstream>

#include <iostream> //debug

void transmit_file(const char* source_path, const char* local_ip_addr, const int& local_port);

void receive_file(const char* dest_path, const char* remote_ip_addr, const int& remote_port);


#endif // FILENET_H_INCLUDED
