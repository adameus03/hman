#ifndef FILENET_H_INCLUDED
#define FILENET_H_INCLUDED

#include "net.h"
#include <winsock2.h>
#include <cstring>
#include <fstream>

#include <iostream> //debug

void transmit_file(const char* source_path);

void receive_file(const char* dest_path);


#endif // FILENET_H_INCLUDED
