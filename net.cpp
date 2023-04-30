#include <cstdio>
#include <cstdlib>
#include <winsock2.h>

SOCKET get_socket_as_server(){
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
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(27015);



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


SOCKET get_socket_as_client(const char* ip_addr){
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
    service.sin_addr.s_addr = inet_addr(ip_addr);
    service.sin_port = htons(27015);




    if(connect(mainSocket,(SOCKADDR*)&service, sizeof(service))==SOCKET_ERROR)
    {
        printf( "Failed to connect to the server.\n" );
        WSACleanup();
        throw;
    }
    printf("Successfully connected to the server");

    return mainSocket;
}





