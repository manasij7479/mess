#include "mess.hpp"
#include "comm.hpp"
#include <iostream>
#include <thread>
#include <stdexcept>
namespace Mess
{
	void Peer::sync(std::string ip)
	{
		throw std::runtime_error("Sync Not implemented yet.");
	}
	
	int Peer::operator()()
	{
		std::thread listener(Protocol::Listen,std::ref(*this));
		
		while(true)
		{
			std::cout<<"mess# ";
			std::string input;
			std::getline(std::cin,input);
			Protocol::SendMessage(user,input,userip.begin()->second);
			m.lock();
			for (auto m:messages)
			{
				std::cout<<m.user()<<" : "<<m.text()<<" \n";
			}
			m.unlock();
		}
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