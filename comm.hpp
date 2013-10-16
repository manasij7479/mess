#ifndef MESS_COM_HPP
#define MESS_COM_HPP
#include<string>
#include "mess.hpp"
namespace Mess
{
	namespace Protocol
	{
		void SendMessage(std::string user,std::string text,std::string ip);
		void Listen(Peer& peer);
		void SendAck(int n,std::string ip);
	}
}
#endif