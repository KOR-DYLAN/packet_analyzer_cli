#ifndef __APPLICATION_MANAGER_H__
#define __APPLICATION_MANAGER_H__

#include <stdint.h>

enum class AppProtocol : int {
    TELNET = 1,
    FTP = 2,
    HTTP = 3,
    DNS = 4,
    All = 5
};

class CApplicationManager {
private:
    AppProtocol Protocol;
public:
    CApplicationManager();
public:
    void SetScopeAppProtocol();
    bool IsProtocol(uint16_t PortNum) const;
    AppProtocol WhoAreYou() const;
};

#endif