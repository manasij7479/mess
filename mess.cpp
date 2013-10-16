#include "mess.hpp"
#include<iostream>
namespace Mess
{
	void Peer::sync(std::string ip)
	{
		
	}
	
	int Peer::operator()()
	{
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