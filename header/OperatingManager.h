#ifndef __OPERATING_MANAGER_H__
#define __OPERATING_MANAGER_H__

#include <linux/if.h>

#include "./IP_Manager.h"
#include "./UDP_Manager.h"
#include "./TCP_Manager.h"
#include "./DNS_Manager.h"
#include "./FTP_Manager.h"
#include "./Telnet_Manager.h"
#include "./HTTP_Manager.h"

class COperatingManager {
private:
    char NIC_Name[32];
    int RawSockFD;
    struct ifreq eth;
public:
    COperatingManager(char NIC[]);
    ~COperatingManager();
public:
    bool EnablePromiscMode(bool bEnable);
    int Run();
public:
    void ShowEthernet_Header_Info(CEthernetManager& ref) const;
    void ShowIP_Header_Info(CIP_Manager& ref) const;
    void ShowUDP_Header_Info(CUDP_Manager& ref) const;
    void ShowTCP_Header_Info(CTCP_Manager& ref) const;
    void ShowPayload(CEthernetManager& ref) const;
};

#endif