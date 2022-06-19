#ifndef __ETHERNET_MANAGER_H__
#define __ETHERNET_MANAGER_H__

#include <stdint.h>
#include <string>
#include <net/ethernet.h>
using namespace std;

class CEthernetManager{
protected:
    uint8_t     DstMAC[6];
    uint8_t     SrcMAC[6];
    uint16_t    EtherType;
    ssize_t     Length;
    uint8_t     EtherHeaderBuff[ETH_MAX_MTU];
    uint8_t*    pEtherPayload;
public:
    CEthernetManager(uint8_t* pPacketBuff, ssize_t PacketLength);
    CEthernetManager(CEthernetManager& ref);
public:
    string GetDstMAC() const;
    string GetSrcMAC() const;
    string GetEtherType() const;
    ssize_t GetPacketLen() const;
public:
    virtual string WhoAmI() const;
};

#endif