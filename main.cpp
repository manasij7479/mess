#include "UserMap.hpp"
#include "Comm.hpp"
#include "Shell.hpp"
int main(int argc, char** argv)
{
    std::string listenport;
    std::string remoteaddr = "";
    switch (argc)
    {
        case 4 : remoteaddr = argv[3];
        case 3 : listenport = argv[2];
                 mess::Self.name = argv[1];
                 break;
        default: std::cerr<<"Invalid OPs\n.";return -1;
    }
    
    mess::Comm comm(listenport);
    mess::Shell shell(comm);
    if (remoteaddr != "")
        comm.registerRemote(remoteaddr);
    comm.listen();
    shell();

    
    
}