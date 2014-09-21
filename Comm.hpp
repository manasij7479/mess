#ifndef MESS_COMM_HPP
#define MESS_COMM_HPP
#include <string>
#include "UserMap.hpp"
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/ServerSocket.h>
#include <thread>
#include <iostream>
namespace mess
{
    class Comm
    {
    public:
        Comm(std::string port, std::string name = mess::Self.name):srv(std::stoi(port))
        {
            map[name] = {name, srv.address().toString(), mess::UserType::Self};
        }
        void sendMessageToAddress(std::string msg, std::string addr, std::string from )
        {
            Poco::Net::SocketAddress sa(addr);
            Poco::Net::StreamSocket socket(sa);
            Poco::Net::SocketStream str(socket);
            str << "MSG "<<from<<" "<<msg;
            str.flush();
        }
        void sendMessageToAll(std::string msg, std::string from = mess::Self.name)
        {
            for (auto user: map)
                sendMessageToAddress(msg, user.second.addr, from);  
        }
        void sendMessageToType(std::string msg, mess::UserType type, std::string from )
        {
            for (auto user: map)
                if (user.second.type == type)
                    sendMessageToAddress(msg, user.second.addr, from);     
        }
        void sendMessageToAllExcept(std::string msg, std::string username, std::string from)
        {
            for (auto user: map)
                if (user.second.name != username && user.second.name != mess::Self.name)
                    sendMessageToAddress(msg, user.second.addr, from);
        }
        
        void registerRemote(std::string addr)
        {
            Poco::Net::SocketAddress sa(addr);
            Poco::Net::StreamSocket socket(sa);
            Poco::Net::SocketStream str(socket);
            str << "NEW "<<mess::Self.name<<' '<<srv.address().toString()<<std::endl;
            std::string remoteusername;
            str >> remoteusername;
            map[remoteusername] = {remoteusername, addr , mess::UserType::Parent};
            mess::Parent = {remoteusername, addr , mess::UserType::Parent};
        }
        
        void listen()
        {
            auto server = [&]()
            {
                for (;;)
                {
                    Poco::Net::StreamSocket ss = srv.acceptConnection();
                    Poco::Net::SocketStream str(ss);
                    std::string cmd;
                    str >> cmd;
                    if (cmd == "NEW")
                    {
                        std::string nuser;
                        str >> nuser;
//                         std::cerr<<nuser<<std::endl;
                        std::string naddr;
                        str >> naddr;
//                         std::cerr<<naddr<<std::endl;
                        map[nuser] = {nuser, naddr, mess::UserType::Child};
                        str << mess::Self.name;
//                         std::cout << "Handled NEW"<<std::endl;
                    }
                    else if (cmd == "MSG")
                    {
                        std::string username;
                        str >> username;
                        std::string line;
                        std::getline(str,line);
                        
                        auto pit = map.find(username);
                        if (pit == map.end())
                        {
                            std::cout << "[Unknown User]";
                        }
                        else
                        {
                            UserInfo peer = pit->second;
                            if (peer.type == mess::UserType::Self)
                                std::cout << "[Self]";
                            if (peer.type == mess::UserType::Child)
                            {
                                sendMessageToAllExcept(line, peer.name, peer.name);
                            }
                            if (peer.type == mess::UserType::Parent)
                                sendMessageToType(line, mess::UserType::Child, peer.name);
                        }
                        std::cout <<'['<<username<<']'<<line<<std::endl;
//                         std::cout << "Handled MSG"<<std::endl;
                        
                        
                    }
                    else 
                    {
                        str << "BAD";
                        std::cout << "Handled BAD"<<std::endl;
                    }
                }
                
            };
            listenThread = new std::thread(server);
        }
        UserMap getUserMap(){return map;}
    private:
        UserMap map;
        Poco::Net::ServerSocket srv;
        std::thread* listenThread;
    };
}
    
#endif