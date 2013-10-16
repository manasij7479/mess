#ifndef MESS_HPP
#define MESS_HPP
#include<map>
#include<string>
#include<set>
#include "message.hpp"
#include <mutex>
namespace Mess
{
	class Peer
	{
	public:
		Peer(std::string u):user(u),act(true)
		{
			userip[u]="127.0.0.1";
		};
		void sync(std::string ip);
		int operator()();
		bool active();
		void quit();
		long long count(); 
		void newMessage(Mess::Message msg);
		void newUser(std::string name,std::string ip);
		std::string ipOf(std::string user)
		{
			return userip[user];
		}
		std::set<Message> copyMessages()
		{
			return messages;
		}
	private:
		std::string user;
		std::map<std::string,std::string> userip;
		std::set<Message> messages;
		std::mutex m;
		bool act;
	};
}
#endif