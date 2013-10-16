#include "message.hpp"
#include "comm.hpp"
#include<sstream>
#include<stdexcept>
namespace Mess
{
	void Message::send(std::string ip)
	{
		Protocol::SendMessage(user_,text_,ip);
	}
	Message::Message(char data[1024],long long o)
	{
		std::istringstream in(data);
		std::string head;
		std::getline(in,head);
		if(head!="M")
			throw std::runtime_error("Bad Message");
		else
		{
			std::string n;			
			std::getline(in,user_);
			std::getline(in,text_);
			std::getline(in,n);
			ord=o;
			ack_=std::stoi(n);
		}
	}


}