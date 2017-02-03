#include "Locator.h"

#include <iostream>

Log * Locator::_providerLog = nullptr;
Config * Locator::_providerConfig = nullptr;

// Log provider methods
void Locator::log( logLevel lvl, const char * msg, int errCode )
{
    if( _providerLog != nullptr )
        _providerLog -> write( lvl, msg, errCode );
}

void Locator::log( logLevel lvl, const std::string & str, int errCode )
{
    if( _providerLog != nullptr )
        _providerLog -> write( lvl, str, errCode );
}

// Config provider methods
std::string Locator::getParam( const std::string & name )
{
    if( _providerConfig == nullptr )
            _providerConfig = Config::instance();
    return _providerConfig -> getParam( name );
}
