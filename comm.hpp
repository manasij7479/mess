#ifndef MESS_COM_HPP
#define MESS_COM_HPP
#include<string>
#include "mess.hpp"
namespace Mess
{
	namespace Protocol
	{
		void Listen(Peer& peer);
		void SendMessage(std::string user,std::string text,std::string ip);		
		void SendAck(int n,std::string ip);
		void SendInfo(std::string user,std::string userip,std::string ip);
		void RequestInfo(std::string user, std::string ip);
	}
}
#endif