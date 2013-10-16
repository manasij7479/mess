#include "comm.hpp"

#include<Poco/Net/DatagramSocket.h>
#include<Poco/Net/SocketAddress.h>
#include<Poco/Net/NetException.h>

#include<unordered_map>
#include<mutex>
#include<thread>

#include<stdio.h>

#include<iostream>

#include<ctime>
#include<cstdlib>

namespace Mess
{
	Poco::Net::DatagramSocket dgs ;
	
	std::unordered_map<int,bool> ack;
	std::mutex ack_m;
	
	static void ack_modify(int n,bool b)
	{
		ack_m.lock();
		ack[n]=b;
		ack_m.unlock();
	}
	static bool ack_check(int n)
	{
		bool result;
		ack_m.lock();
		auto it=ack.find(n);
		if(it!=ack.end())
			result=ack[n];
		else result=false;
		ack_m.unlock();
		
		return result;
	}
	
	int random_int() //not good..modify later 
	{
		static bool ini=false;
		if(!ini)
		{
			ini=true;
			std::srand(std::time(nullptr));
		}
		return std::rand();
	}
	
	
	void Protocol::SendAck(int n,std::string ip)
	{
		char data[1024];
		std::snprintf(data,1024,"A\n%d",n);
		dgs.sendTo(data,1024,{ip,7479});
	}
	
	
	void Protocol::SendMessage(std::string user, std::string text, std::string ip)
	{
		char data[1024];
		int n=random_int();
		std::snprintf(data,1024,"M\n%d\n%s\n%s",n,user.c_str(),text.c_str());
		while(!ack_check(n))
		{
			std::cerr<<"S\n";
			dgs.sendTo(data,1024,{ip,7479});
			ack_modify(n,false);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}
	void Protocol::Listen(Peer& peer)
	{
		Poco::Net::SocketAddress sa(Poco::Net::IPAddress(),7479);
		Poco::Net::DatagramSocket dgs(sa);
		dgs.setReceiveTimeout(Poco::Timespan(1,0));
		while(peer.active())
		{
			char data[1024];
			Poco::Net::SocketAddress sender;
			try
			{
				dgs.receiveFrom(data,1024,sender);
			}
			catch(const Poco::TimeoutException&)
			{
				continue;
			}
			switch(data[0])
			{
				case 'M':
				{
					std::cerr<<"R\n";
					Message m(data,peer.count());
					SendAck(m.ack(),peer.ipOf(m.user()));
					peer.newMessage(m);
					break;

				}				
				case 'A':
				{
					std::cerr<<"A\n";
					int n;
					std::sscanf(data,"A\n%d",&n);
					ack_modify(n,true);
					break;
				}
				
				
			}
			
		}
	}

}