#include "../header/OperatingManager.h"
#include "../header/ApplicationManager.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <typeinfo>
using namespace std;

#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>

COperatingManager* pThis;
void RegisterSignalHandler();
void Signal_Handler(int SIG_Num);

int COperatingManager::Run(){
    ssize_t recvLen;
    uint8_t PacketBuff[ETH_MAX_MTU] = { 0 };

    CApplicationManager AppManager;

    while(true) {
        memset(PacketBuff, 0, sizeof(PacketBuff));
        recvLen = read(this->RawSockFD, PacketBuff, ETH_MAX_MTU);
        if(recvLen < 0)
            continue;

        // Unpack Ether packet.
        CEthernetManager Ether_Packet(PacketBuff, recvLen);
        
        if(Ether_Packet.GetEtherType() == "IPv4"){
            // Unpack IP packet.
            CIP_Manager IP_Packet(Ether_Packet);

            // Loopback Filtering.
            if((IP_Packet.Get_Src_IP_Address() == "127.0.0.1") || (IP_Packet.Get_Dst_IP_Address() == "127.0.0.1"))
                continue;

            if(IP_Packet.Get_IP_Protocol_Identifier() == "[User Datagram Protocol]"){
                // Unpack UDP packet.
                CUDP_Manager UDP_Packet(IP_Packet);
                
                // App Protocol Filtering...
                if(!AppManager.IsProtocol(UDP_Packet.Get_Src_Port()) && !AppManager.IsProtocol(UDP_Packet.Get_Dst_Port()))
                    continue;

                // Show Packet Info.
                cout << "-------------------------------------------------------------------------" << endl;
                this->ShowEthernet_Header_Info(UDP_Packet);
                this->ShowIP_Header_Info(UDP_Packet);
                this->ShowUDP_Header_Info(UDP_Packet);

                if(AppManager.WhoAreYou() == AppProtocol::DNS) {
                    CDNS_Manager DNS_Packet(UDP_Packet);
                    this->ShowPayload(dynamic_cast<CEthernetManager&>(DNS_Packet));
                }
                else
                    this->ShowPayload(dynamic_cast<CEthernetManager&>(UDP_Packet));
                cout << endl;
            }
            else if(IP_Packet.Get_IP_Protocol_Identifier() == "[Transmission Control Protocol]"){
                // Unpack UDP packet.
                CTCP_Manager TCP_Packet(IP_Packet);

                // App Protocol Filtering...
                if(!AppManager.IsProtocol(TCP_Packet.Get_Src_Port()) && !AppManager.IsProtocol(TCP_Packet.Get_Dst_Port()))
                    continue;

                // Show Packet Info.
                cout << "-------------------------------------------------------------------------" << endl;
                this->ShowEthernet_Header_Info(TCP_Packet);
                this->ShowIP_Header_Info(TCP_Packet);
                this->ShowTCP_Header_Info(TCP_Packet);

                if(AppManager.WhoAreYou() == AppProtocol::FTP) {
                    CFTP_Manager FTP_Packet(TCP_Packet);
                    this->ShowPayload(dynamic_cast<CEthernetManager&>(FTP_Packet));
                }
                else if(AppManager.WhoAreYou() == AppProtocol::HTTP) {
                    CHTTP_Manager HTTP_Packet(TCP_Packet);
                    this->ShowPayload(dynamic_cast<CEthernetManager&>(HTTP_Packet));
                }
                else if(AppManager.WhoAreYou() == AppProtocol::TELNET) {
                    CTelnet_Manager Telnet_Packet(TCP_Packet);
                    this->ShowPayload(dynamic_cast<CEthernetManager&>(Telnet_Packet));
                }
                else
                    this->ShowPayload(dynamic_cast<CEthernetManager&>(TCP_Packet));
                cout << endl;
            }
        }
    }

    return 0;
}

void COperatingManager::ShowEthernet_Header_Info(CEthernetManager& ref) const {
    cout << "[ Dest MAC                ] : " << ref.GetDstMAC() << endl;
    cout << "[ Src  MAC                ] : " << ref.GetSrcMAC() << endl;
    cout << "[ Eth Type                ] : " << ref.GetEtherType() << endl;
    cout << "[ Total Packet Length     ] : " << ref.GetPacketLen() << endl << endl;
}

void COperatingManager::ShowIP_Header_Info(CIP_Manager& ref) const {
    cout << "[ IP Ver                  ] : " << ref.Get_IP_Version() << endl;
    cout << "[ IP Header Length        ] : " << ref.Get_IP_Header_Length() << endl;
    cout << "[ IP Type Of Service      ] : " << ref.Get_IP_Type_Of_Service() << endl;
    cout << "[ IP Total Packet Length  ] : " << ref.Get_IP_Total_Packet_Length() << endl;
    cout << "[ IP Fragment Identifier  ] : " << ref.Get_IP_Fragment_Identifier() << endl;
    cout << "[ IP Fragmentation Flags  ] : " << ref.Get_IP_Fragmentation_Flags() << endl;
    cout << "[ IP Fragmentation Offset ] : " << ref.Get_IP_Fragmentation_Offset() << endl;
    cout << "[ IP Time To Live         ] : " << ref.Get_IP_Time_To_Live() << endl;
    cout << "[ IP Option Length        ] : " << ref.Get_IP_Option_Length() << endl;
    cout << "[ IP Protocol             ] : " << ref.Get_IP_Protocol_Identifier() << endl;
    cout << "[ Src IP Address          ] : " << ref.Get_Src_IP_Address() << endl;
    cout << "[ Dst IP Address          ] : " << ref.Get_Dst_IP_Address() << endl << endl;
}

void COperatingManager::ShowUDP_Header_Info(CUDP_Manager& ref) const {
    cout << "[ UDP Payload Length      ] : " << ref.Get_UDP_Payload_Length() << endl;
    cout << "[ Get UDP Checksum        ] : " << ref.Get_UDP_Checksum() << endl;
    cout << "[ Dst Port                ] : " << ref.Get_Src_Port() << endl;    
    cout << "[ Src Port                ] : " << ref.Get_Dst_Port() << endl << endl;
}

void COperatingManager::ShowTCP_Header_Info(CTCP_Manager& ref) const {
    cout << "[ TCP SequenceNumber      ] : " << ref.Get_TCP_Sequence_Number() << endl;
    cout << "[ TCP Ack Number          ] : " << ref.Get_TCP_Acknowledgement_Number() << endl;
    cout << "[ TCP Header Length       ] : " << ref.Get_TCP_Header_Length() << endl;
    cout << "[ TCP Control Flags       ] : " << ref.Get_TCP_Control_Flags() << endl;
    cout << "[ TCP Window Size         ] : " << ref.Get_TCP_Window_Size() << endl;
    cout << "[ TCP Checksum            ] : " << ref.Get_TCP_Checksum() << endl;
    cout << "[ TCP Urgent Point        ] : " << ref.Get_TCP_Urgent_Point() << endl;
    cout << "[ TCP Option Length       ] : " << ref.Get_TCP_Option_Length() << endl;
    cout << "[ TCP Payload Length      ] : " << ref.Get_TCP_Payload_Length() << endl;
    cout << "[ Src Port                ] : " << ref.Get_Src_Port() << endl;
    cout << "[ Dst Port                ] : " << ref.Get_Dst_Port() << endl << endl;
}

void COperatingManager::ShowPayload(CEthernetManager& ref) const {
    if(ref.WhoAmI() == "CUDP_Manager"){
        CUDP_Manager& Packet = dynamic_cast<CUDP_Manager&>(ref);
       Packet.ShowAppData();
    }
    else if(ref.WhoAmI() == "CTCP_Manager"){
        CTCP_Manager& Packet = dynamic_cast<CTCP_Manager&>(ref);
        Packet.ShowAppData();
    }
    else{
        cout << "ShowPayload() Failed...." << endl;
    }
}

COperatingManager::COperatingManager(char NIC[]){
    // Create Raw Socket.(ETH_P_ALL -> Every packet)
    this->RawSockFD = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(NIC != nullptr){
        // Register Exit SignalHandler.(To disable Promisc mode.)
        RegisterSignalHandler();

        // Set NIC Name.
        strcpy(eth.ifr_name, NIC);

        // Enable Promisc Mode.
        this->EnablePromiscMode(true);
    }
    
    // Register Global Pointer
    pThis = this;
}

COperatingManager::~COperatingManager(){

}

bool COperatingManager::EnablePromiscMode(bool bEnable){
    // Get NIC Info.
    if(ioctl(this->RawSockFD, SIOCGIFFLAGS, &eth)){
        printf("EnablePromiscMode() Failed...\n");
        return false;
    }

    switch(bEnable){
    case true:
        eth.ifr_flags |= IFF_PROMISC;
        cout << "[Enable Promisc Mode]" << endl;
        break;
    case false:
        eth.ifr_flags ^= IFF_PROMISC;
        cout << "[Disable Promisc Mode]" << endl;
        break;
    }

    return ioctl(this->RawSockFD, SIOCSIFFLAGS, &eth) ? false : true;
}

void Signal_Handler(int SIG_Num){
    FILE* fp;
    char logTxt[128];

    switch(SIG_Num){
    case SIGHUP:
    case SIGINT:
    case SIGQUIT:
    case SIGFPE:
    case SIGKILL:
    case SIGUSR1:
    case SIGUSR2:
    case SIGPIPE:
    case SIGALRM:
    case SIGTERM:
        fp = fopen("Termination_Log.txt", "wt");
        sprintf(logTxt, "Detected SigNum: %d\n", SIG_Num);
        fputs(logTxt, fp);
        fclose(fp);

        cout << logTxt << endl;
        pThis->EnablePromiscMode(false);
        exit(1);
        break;
    default:
        break;
    }
}

void RegisterSignalHandler(){
    signal(SIGHUP, Signal_Handler);
    signal(SIGINT, Signal_Handler);
    signal(SIGQUIT, Signal_Handler);
    signal(SIGFPE, Signal_Handler);
    signal(SIGKILL, Signal_Handler);
    signal(SIGUSR1, Signal_Handler);
    signal(SIGUSR2, Signal_Handler);
    signal(SIGPIPE, Signal_Handler);
    signal(SIGALRM, Signal_Handler);
    signal(SIGTERM, Signal_Handler);
}