#include <cstdio>
#include <cstdlib>
#include <winsock2.h>

/**
    @brief
        Get a TCP/IP listening socket
    @param local_ip_addr
        Local socket IPv4 address
    @param local_port
        Local socket port
*/
SOCKET get_socket_as_server(const char* local_ip_addr, const int& local_port){
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(result != NO_ERROR){
        printf("Initialization error.\n");
        throw;
    }

    SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(mainSocket==INVALID_SOCKET)
    {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        throw;
    }

    sockaddr_in service;
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(local_ip_addr);
    service.sin_port = htons(local_port);

    if(bind(mainSocket,(SOCKADDR*)&service, sizeof(service))==SOCKET_ERROR)
    {
        printf( "bind failed.\n" );
        closesocket(mainSocket);
        throw;
    }

    if(listen(mainSocket, 1) == SOCKET_ERROR){
        printf("Error listening on socket.\n" );
        throw;
    }

    SOCKET acceptSocket = SOCKET_ERROR;
    printf("Waiting for a client to connect...\n");
    while(acceptSocket==(unsigned long long)SOCKET_ERROR)
    {
        acceptSocket = accept(mainSocket,NULL,NULL);
    }
    printf("Client connected.\n");
    mainSocket = acceptSocket;

    return mainSocket;
}

/**
    @brief
        Get a TCP/IP socket, which is connected to the server
    @param remote_ip_addr
        Remote socket IPv4 address
    @param remote_port
        Remote socket port
*/
SOCKET get_socket_as_client(const char* remote_ip_addr, const int& remote_port){
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(result != NO_ERROR){
        printf( "Initialization error.\n" );
        throw;
    }

    SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(mainSocket==INVALID_SOCKET)
    {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        throw;
    }

    sockaddr_in service;
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(remote_ip_addr);
    service.sin_port = htons(remote_port);

    if(connect(mainSocket,(SOCKADDR*)&service, sizeof(service))==SOCKET_ERROR)
    {
        printf( "Failed to connect to the server.\n" );
        WSACleanup();
        throw;
    }
    printf("Successfully connected to the server.\n");

    return mainSocket;
}
