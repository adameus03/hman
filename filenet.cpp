#include "net.h"
#include <winsock2.h>
#include <cstring>
#include <fstream>

#include <iostream> //verbose
#include "tests.h" //verbose

/*
    BLK = 1024B of data
    SIZE = 8B of file size
    -----------------------
    TX  8B      SIZE
    TX  1024B   BLK
    TX  1024B   BLK
    TX  1024B   BLK
    ...
    TX  1024B   BLK
*/

typedef unsigned char uchar;
typedef unsigned long long ull;

const size_t BLK_SIZE = 1024;
//const uchar ACK = 0xa;

/**
    @brief
        Transmit a file over TCP/IP
    @param source_path
        Path of the file to be sent
    @param local_ip_addr
        Local socket IPv4 address
    @param local_port
        Local socket port
*/
void transmit_file(const char* source_path, const char* local_ip_addr, const int& local_port){ // hman -t source_path
    std::ifstream f_in(source_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    size_t sbuff_size = f_in_pbuf->pubseekoff(0, f_in.end, f_in.in);
    f_in_pbuf->pubseekpos(0, f_in.in);
    uchar* sbuff = new uchar[sbuff_size+(BLK_SIZE-sbuff_size%BLK_SIZE)];
    f_in_pbuf->sgetn((char*)sbuff, sbuff_size);
    f_in.close();

    SOCKET s = get_socket_as_server(local_ip_addr, local_port);
    std::cout << "sbuff_size: " << sbuff_size << std::endl;
    uchar* sz_blk = new uchar[8];

        *sz_blk     = (uchar)(sbuff_size >> 0x38);
        *(sz_blk+1) = (uchar)(sbuff_size >> 0x30) & 0xff;
        *(sz_blk+2) = (uchar)(sbuff_size >> 0x28) & 0xff;
        *(sz_blk+3) = (uchar)(sbuff_size >> 0x20) & 0xff;
        *(sz_blk+4) = (uchar)(sbuff_size >> 0x18) & 0xff;
        *(sz_blk+5) = (uchar)(sbuff_size >> 0x10) & 0xff;
        *(sz_blk+6) = (uchar)(sbuff_size >>  0x8) & 0xff;
        *(sz_blk+7) = (uchar)sbuff_size & 0xff;

    printbp(sz_blk, 64);
    memcpy(sz_blk, &sbuff_size, 8);
    send(s, (const char*)sz_blk, 8, 0);
    delete[] sz_blk;

    uchar c;
    for(ull u=0x0; u<sbuff_size; u+=BLK_SIZE){
        std::cout << "Sending block" << std::endl;
        printbp(sbuff+u, BLK_SIZE);
        send(s, (const char*)(sbuff+u), BLK_SIZE, 0);
    }
    delete[] sbuff;
    WSACleanup();
}

/**
    @brief
        Receive a file over TCP/IP
    @param dest_path
        Destination path for the received file
    @param remote_ip_addr
        Remote socket IPv4 address
    @param remote_port
        Remote socket port
*/
void receive_file(const char* dest_path, const char* remote_ip_addr, const int& remote_port){ //hman -r dest_path ip_addr
    SOCKET s = get_socket_as_client(remote_ip_addr, remote_port);
    std::cout << "Socket_as client OK" << std::endl;
    uchar* sz_blk = new uchar[8];
    recv(s, (char*)sz_blk, 8, 0);

    printbp(sz_blk, 64);

    size_t dbuff_size = (ull)*(sz_blk+7) | ((ull)*(sz_blk+6)<<0x8) | ((ull)*(sz_blk+5)<<0x10) | ((ull)*(sz_blk+4)<<0x18) | ((ull)*(sz_blk+3)<<0x20) | ((ull)*(sz_blk+2)<<0x28) | ((ull)*(sz_blk+1)<<0x30) | ((ull)*sz_blk<<0x38);
    memcpy(&dbuff_size, sz_blk, 8);
    delete[] sz_blk;

    std::cout << "dbuff_size: " << dbuff_size << std::endl;
    uchar* dbuff = new uchar[dbuff_size+BLK_SIZE-(dbuff_size%BLK_SIZE)];

    for(ull u=0x0; u<dbuff_size; u+=BLK_SIZE){
        std::cout << "Receiving block..." << std::endl;
        recv(s, (char*)(dbuff+u), BLK_SIZE, 0);;
    }
    std::cout << "All blocks received" << std::endl;
    std::ofstream f_out(dest_path, std::ofstream::trunc|std::ofstream::binary);
    std::filebuf* f_out_pbuf = f_out.rdbuf();
    f_out_pbuf->sputn((char*)dbuff, dbuff_size);
    f_out.close();
    WSACleanup();
}
