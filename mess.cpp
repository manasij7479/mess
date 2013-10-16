#include "mess.hpp"
#include "comm.hpp"
#include <iostream>
#include <thread>
#include <stdexcept>
namespace Mess
{
	void Peer::sync(std::string ip)
	{
		Protocol::SendInfo(user,addr,ip);
		Protocol::RequestInfo(user,ip);
	}
	void Peer::sendUserData(std::string ip)
	{
		m.lock();
		auto copy=userip;
		m.unlock();
		for (auto u:copy)
		{
			Protocol::SendInfo(u.first,u.second,ip);
		}
	}

	int Peer::operator()()
	{
		std::thread listener(Protocol::Listen,std::ref(*this));
		
		while(true)
		{
			std::cout<<"mess# ";
			std::string input;
			if(!std::getline(std::cin,input))
			{
				quit();
				break;
			}
			for(auto x:userip)
			{
				Protocol::SendMessage(user,input,x.second);
			}
			m.lock();
			auto copy =messages;
			m.unlock();
			for (auto m:copy)
			{
				std::cout<<m.user()<<" : "<<m.text()<<" \n";
			}
		}
		listener.join();
		return 0;
	}
	bool Peer::active()
	{
		bool result;
		m.lock();
		result=act;
		m.unlock();
		return result;
	}
	void Peer::quit()
	{
		m.lock();
		act=false;
		m.unlock();
	}

	long long int Peer::count()
	{
		return messages.size();
	}
	void Peer::newMessage(Message msg)
	{
		m.lock();
		messages.insert(msg);
		m.unlock();
	}
	void Peer::newUser(std::string name, std::string ip)
	{
		m.lock();
		userip[name]=ip;
		m.unlock();
	}
}