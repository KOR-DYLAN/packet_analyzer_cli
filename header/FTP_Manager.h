#ifndef __FTP_MANAGER_H__
#define __FTP_MANAGER_H__

#include "./TCP_Manager.h"
#include <vector>

class CFTP_Manager : public CTCP_Manager {
private:
    bool bFTP_Operating_Mode;
    bool bFTP_Connection_Mode;
    bool bDirectories;
    string FTP_Message;
public:
    CFTP_Manager(CTCP_Manager& ref);
    ~CFTP_Manager() {}
public:
    string GetCommandDiscription();
    string GetReplyCodeDiscription();
    virtual void ShowAppData();
};

#endif