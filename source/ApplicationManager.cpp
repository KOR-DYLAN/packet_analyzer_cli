#include "../header/ApplicationManager.h"
#include <iostream>
#include <string>
using namespace std;

CApplicationManager::CApplicationManager() {
    this->SetScopeAppProtocol();
}

void CApplicationManager::SetScopeAppProtocol() {
    int sel;
    string Input;

    while(true) {
        cout << "[ 1.Telnet  2.FTP  3.HTTP  4.DNS  5.All ]" << endl;
        cout << ">> ";
        getline(cin, Input);

        if(Input.length() != 1)
            goto ExceptionHandling;

        try{
            sel = stoi(Input);
        }
        catch(...){
            goto ExceptionHandling;
        }
        if((sel < 1) || (sel > 5))
            goto ExceptionHandling;

        Protocol = AppProtocol(sel);
        break;

    ExceptionHandling:
        cout << "Wrong Input..." << endl;
        continue;
    }
}

bool CApplicationManager::IsProtocol(uint16_t PortNum) const {
    if(Protocol == AppProtocol::All)
        return true;

    switch(PortNum){
    case 23:            // Telnet
        return (Protocol == AppProtocol::TELNET);
    case 20: case 21:   // FTP
        return (Protocol == AppProtocol::FTP);
    case 80:            // HTTP    
        return (Protocol == AppProtocol::HTTP);
    case 53:            // DNS
        return (Protocol == AppProtocol::DNS);
    default:
        return false;
    }
}

AppProtocol CApplicationManager::WhoAreYou() const {
    return AppProtocol(Protocol);
}