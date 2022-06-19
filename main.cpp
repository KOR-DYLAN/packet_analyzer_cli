#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "./header/OperatingManager.h"

int main(int argc, char* argv[]) {
    char* NIC = (argc !=2) ? nullptr : argv[1];
    if(getuid() != 0) {
        std::string path(argv[0]);
        std::string cmd("sudo ");
        cmd += path;
        if(argc > 1) {
            cmd += " " + std::string(argv[1]);
        }
        system(cmd.c_str());
        exit(1);
    }

    COperatingManager OperatingManager(NIC);

    return OperatingManager.Run();
}
