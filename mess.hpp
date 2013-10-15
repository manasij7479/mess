#ifndef MESS_HPP
#define MESS_HPP
#include<map>
#include<string>
#include<set>
#include "message.hpp"
namespace Mess
{
	class Peer
	{
	public:
		Peer(std::string u):user(u){};
		void sync(std::string ip);
		int operator()();
	private:
		std::string user;
		std::map<std::string,std::string> userip;
		std::set<Message> text;
	};
}
#endif