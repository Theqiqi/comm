//#define UNICODE
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <string>
#include <iostream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


char addr_ip[30] = { "0" };
char senddata[65523] = { "0" };
char recvbuf[DEFAULT_BUFLEN];

int recvbuflen = DEFAULT_BUFLEN;
void send_word(SOCKET* socket, int iResult);
void recv_word(SOCKET* socket, int iResult);

int __cdecl main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    //const char* sendbuf = "this is a test";
    //char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    //int recvbuflen = DEFAULT_BUFLEN;

    // Validate the parameters
   // if (argc != 2) {
    //    printf("usage: %s server-name\n", argv[0]);
   //     return 1;
   // }

    printf("请输入服务器地址：");
     //printf("ip:");
    gets_s(addr_ip);
    //scanf("%s", addr_ip);
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    //iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    iResult = getaddrinfo(addr_ip, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    std::thread th_send(send_word, &ConnectSocket, iResult);
    th_send.detach();
    /*
    printf("鍙戦�佹秷鎭粰鏈嶅姟鍣? %s");
    char sendbuf[] = {0};
    gets_s(sendbuf);

    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    */
    //printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    //iResult = shutdown(ConnectSocket, SD_SEND);
    //if (iResult == SOCKET_ERROR) {
     //   printf("shutdown failed with error: %d\n", WSAGetLastError());
     //   closesocket(ConnectSocket);
     //   WSACleanup();
     //   return 1;
   // }

    // Receive until the peer closes the connection
    recv_word(&ConnectSocket, iResult);
    /*
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);
    */
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    getchar();
    return 0;
}

void send_word(SOCKET* socket, int iResult)
{
    while (true)
    {
        while (true)
        {
            printf("发送消息给服务器：");
            memset(senddata, 0, sizeof senddata);
            scanf("%s", senddata);
            char* sendbuf = senddata;

            iResult = send(*socket, sendbuf, (int)strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(*socket);
                WSACleanup();
                // return 1;
            }

        }

    }
}
void recv_word(SOCKET* socket, int iResult)
{
    while (true)
    {
        do {


            iResult = recv(*socket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
            {

                printf("\n从服务器接受消息：%s", recvbuf);
                printf("\n发送消息给服务器：");
                memset(recvbuf, 0, sizeof  recvbuf);//清空数组缓存

            }
            else if (iResult == 0)
                printf("Connection closed\n");
            else
                printf("recv failed with error: %d\n", WSAGetLastError());

        } while (iResult > 0);

    }
}