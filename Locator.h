#ifndef LOCATOR_H_
#define LOCATOR_H_

#include <string>

#include "Log.h"
#include "Config.h"

class Locator
{
private:
    static Log * _providerLog;
    static Config * _providerConfig;

    Locator() = delete;
    ~Locator() = delete;
public:
    static void regProvider( Log *log )               { _providerLog    = log; }

    // Log provider methods
    static void log( logLevel lvl, const char * msg, int errCode = 0 );
    static void log( logLevel lvl, const std::string & str, int errCode = 0 );

    // Config provider methods
    static std::string getParam( const std::string & name );
};

#endif // LOCATOR_H_

