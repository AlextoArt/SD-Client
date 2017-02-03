#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <string>
#include <vector>
#include <mutex>

#define _WIN32_WINNT 0x501

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <windows.h>

#include "Locator.h"
#include "TaskTable.h"

#define MAX_PACKAGE_SIZE 512

const std::string __cmd_start("START");
const std::string __cmd_connected("CONNECTED");
const std::string __cmd_unauthorized("UNAUTHORIZED");
const std::string __cmd_wait("WAIT");
const std::string __cmd_over("OVER");

void finalizePackage( char * str );

class Client
{
private:
    bool _working;
    SOCKET _socket;
    HANDLE _stopEvent;

    HANDLE _worker;
    unsigned int _workerId;

    std::string _user;
    std::string _server;
    std::string _port;

    std::mutex _locker;
    std::vector<TaskTable> _data;

    char _buffer[MAX_PACKAGE_SIZE];

    bool init();
    bool connect();
    bool work();

    static unsigned int __stdcall workerThread( void * pClient, addrinfo * pAddr );
public:
    Client( const std::string & server, const std::string & port );
    ~Client();

    void start();
    void stop();

};

#endif // CLIENT_H_
