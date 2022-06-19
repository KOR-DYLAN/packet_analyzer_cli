#include "../header/EthernetManager.h"
#include <stdint.h>
#include <iostream>
#include <string.h> 
#include <arpa/inet.h>

CEthernetManager::CEthernetManager(uint8_t* pPacketBuff, ssize_t PacketLength){
    struct ether_header* pEH = nullptr;

    memcpy(this->EtherHeaderBuff, pPacketBuff, PacketLength);       
    this->Length = PacketLength;                                       

    pEH = (struct ether_header*)this->EtherHeaderBuff;                 
    memcpy(this->DstMAC, pEH->ether_dhost, sizeof(pEH->ether_dhost));  
    memcpy(this->SrcMAC, pEH->ether_shost, sizeof(pEH->ether_shost));  
    this->EtherType = ntohs(pEH->ether_type);                        
    this->pEtherPayload = this->EtherHeaderBuff + sizeof(struct ether_header);
}

CEthernetManager::CEthernetManager(CEthernetManager& ref){
    struct ether_header* pEH = nullptr;

    memcpy(this->EtherHeaderBuff, ref.EtherHeaderBuff, ref.Length);    
    this->Length = ref.Length;                                         

    pEH = (struct ether_header*)this->EtherHeaderBuff;               
    memcpy(this->DstMAC, pEH->ether_dhost, sizeof(pEH->ether_dhost));  
    memcpy(this->SrcMAC, pEH->ether_shost, sizeof(pEH->ether_shost));   
    this->EtherType = ntohs(pEH->ether_type);                      
    this->pEtherPayload = ref.pEtherPayload;                        
}

string CEthernetManager::GetDstMAC() const{
    char buff[20];

    sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X",
        this->DstMAC[0],
        this->DstMAC[1],
        this->DstMAC[2],
        this->DstMAC[3],
        this->DstMAC[4],
        this->DstMAC[5]
    );

    return string(buff);
}

string CEthernetManager::GetSrcMAC() const{
    char buff[20];

    sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X",
        this->SrcMAC[0],
        this->SrcMAC[1],
        this->SrcMAC[2],
        this->SrcMAC[3],
        this->SrcMAC[4],
        this->SrcMAC[5]
    );

    return string(buff);
}

string CEthernetManager::GetEtherType() const{
    switch(this->EtherType){
    case ETHERTYPE_PUP:     /* Xerox PUP */
        return string("Xerox PUP");
    case ETHERTYPE_SPRITE:  /* Sprite */
        return string("Sprite");
    case ETHERTYPE_IP:      /* IPv4 */
        return string("IPv4");
    case ETHERTYPE_ARP:     /* Address resolution */
        return string("Address resolution");
    case ETHERTYPE_REVARP:  /* Reverse ARP */
        return string("Reverse ARP");
    case ETHERTYPE_AT:      /* AppleTalk protocol */
        return string("AppleTalk protocol");
    case ETHERTYPE_AARP:    /* AppleTalk ARP */
       return string("AppleTalk ARP");
    case ETHERTYPE_VLAN:    /* IEEE 802.1Q VLAN tagging */
        return string("IEEE 802.1Q VLAN tagging");
    case ETHERTYPE_IPX:     /* IPX */
        return string("IPX");
    case ETHERTYPE_IPV6:    /* IPv6 */
        return string("IPv6");
    case ETHERTYPE_LOOPBACK:/* used to test interfaces */
        return string("Loopback");
    default:                /* Unknown */
        return string("Unknown");
    }
}

ssize_t CEthernetManager::GetPacketLen() const {
    return int16_t(this->Length);
}

string CEthernetManager::WhoAmI() const{
    return string("CEthernetManager");
}