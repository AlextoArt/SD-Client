#include <sstream>

#include "Client.h"

Client::Client( const std::string & server, const std::string & port ) :
    _server(server),
    _port(port),
    _working(false)
{
    long unsigned int length;
    char buffer[64];
    GetUserNameA( buffer, &length );
    _user = buffer;
    _socket = INVALID_SOCKET;
}

Client::~Client()
{
    if( _working )
        stop();
}

bool Client::init()
{
    bool success = true;
    WSADATA ws = {0};
    if( FAILED( WSAStartup( MAKEWORD(2, 2), &ws ) ) )
    {
        success = false;
        Locator::log( LOG_ERROR, "WSA Startup error", WSAGetLastError() );
    }
    else
    {
        _socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
        if( _socket == INVALID_SOCKET )
        {
            success = false;
            Locator::log( LOG_ERROR, "Cannot create client socket" , WSAGetLastError() );
            WSACleanup();
        }
    }
    return success;
}

bool Client::connect()
{
    bool success = true;
    addrinfo *result = NULL;
    addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if ( getaddrinfo( _server.c_str(), _port.c_str(), &hints, &result ) != 0 )
    {
        success = false;
        Locator::log( LOG_ERROR, "Cannot resolve address of server : " + _server , WSAGetLastError() );
        WSACleanup();
    }
    else
    {
        if ( SOCKET_ERROR == connect( _socket, result, sizeof(*result) ) )
        {
            success = false;
            Locator::log( LOG_ERROR, "Connection fail", WSAGetLastError() );
            WSACleanup();
        }
        else
        {
            if( SOCKET_ERROR == send( _socket, _user.c_str(), _user.length(), 0 ) )
            {
                success = false;
                Locator::log( LOG_ERROR, "Sending data error" , WSAGetLastError() );
                closesocket( _socket );
                WSACleanup();
            }
            else
            {
                int res = recv( _socket, _buffer, MAX_PACKAGE_SIZE, 1000 );
                if( res <= 0 )
                {
                    success = false;
                    Locator::log( LOG_ERROR, "Receiving data error" , WSAGetLastError() );
                    closesocket( _socket );
                    WSACleanup();
                }
                else
                {
                    finalizePackage();
                    if( _buffer == __cmd_unauthorized )
                    {
                        success = false;
                        Locator::log( LOG_ERROR, "Unauthorized" , WSAGetLastError() );
                        closesocket( _socket );
                        WSACleanup();
                    }
                    else
                        Locator::log( LOG_ERROR, "Connection successfull" );
                }
            }
        }
    }
    return success;
}

bool Client::work()
{
    bool success = true;
    while ( _buffer != __cmd_start )
    {
        int res = recv( _socket, _buffer, MAX_PACKAGE_SIZE, 1000 );
        if( res <= 0 )
        {
            success = false;
            Locator::log( LOG_ERROR, "Receiving data error" , WSAGetLastError() );
            closesocket( _socket );
            WSACleanup();
        }
    }

    if( success )
    {
        _locker.lock();
        _data.clear();
        while ( _buffer != __cmd_over )
        {
            int res = recv( _socket, _buffer, MAX_PACKAGE_SIZE, 1000 );
            if( res <= 0 )
            {
                success = false;
                Locator::log( LOG_ERROR, "Receiving data error" , WSAGetLastError() );
                closesocket( _socket );
                WSACleanup();
            }
            else
            {
                std::cout << _buffer << std::endl;
            }
        }
        _locker.unlock();
    }
    return success;
}

void Client::start()
{
    _worker = (HANDLE)_beginthreadex( 0, 0, workerThread, (void *)this, result, 0, &_workerId )
}

void Client::stop();
{
    if( _working )
    {
        WSACleanup();
    }
}

unsigned int __stdcall workerThread( void * pClient )
{


}

void finalizePackage( char * str )
{
    int i = 0;
    while( i < MAX_PACKAGE_SIZE && !( str[i] == '\0' || str[i] == '\n') )
        ++i;
    if( i < MAX_PACKAGE_SIZE )
        str[i] = '\0';
}





















