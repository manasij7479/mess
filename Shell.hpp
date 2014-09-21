#ifndef MESS_SHELL_HPP
#define MESS_SHELL_HPP
#include "Comm.hpp"
namespace mess
{
    class Shell
    {
    public:
        Shell(Comm& c):comm(c){}
        void operator()()
        {
            for(;;)
            {
                std::string input;
                std::getline(std::cin,input);
                comm.sendMessageToAll(input);
            }
        }
    private:
        Comm& comm;
    };
}

#endif