#include "comm.hpp"

#include<Poco/Net/DatagramSocket.h>
#include<Poco/Net/SocketAddress.h>
#include<Poco/Net/NetException.h>

#include<unordered_map>
#include<mutex>
#include<thread>



#include<iostream>
#include<sstream>

#include<cstdio>


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
	
	int random_int() //dangerous!!..modify later 
	{
		static int result =0;
		int ret;
		ack_m.lock();
		ret= result++;
		ack_m.unlock();
		return ret;
	}
	
	
	void Protocol::SendAck(int n,std::string ip)
	{
		char data[1024];
		std::snprintf(data,1024,"A\n%d",n);
		std::cerr<<"AS\n";
		dgs.sendTo(data,1024,{ip,7479});
	}
	
	void Protocol::SendMessage(std::string user, std::string text, std::string ip)
	{
		char data[1024];
		int n=random_int();
		std::snprintf(data,1024,"M\n%s\n%s\n%d",user.c_str(),text.c_str(),n);
		while(!ack_check(n))
		{
			std::cerr<<"MS\n";
			dgs.sendTo(data,1024,{ip,7479});
			ack_modify(n,false);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}
	void Protocol::SendInfo(std::string user, std::string userip, std::string ip)
	{
		char data[1024];
		int n=random_int();
		std::snprintf(data,1024,"I\n%s\n%s\n%d",user.c_str(),userip.c_str(),n);
		while(!ack_check(n))
		{
			std::cerr<<n<<"IS\n";
			dgs.sendTo(data,1024,{ip,7479});
			ack_modify(n,false);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}

	void Protocol::RequestInfo(std::string user,std::string ip)
	{
		char data[1024];
// 		int n=random_int();
		std::snprintf(data,1024,"R\n%s",user.c_str()/*,n*/);
// 		while(!ack_check(n))
// 		{
			std::cerr<<"RS\n";
			dgs.sendTo(data,1024,{ip,7479});
// 			ack_modify(n,false);
// 			std::this_thread::sleep_for(std::chrono::milliseconds(300));
// 		}
	}

	
	void Protocol::Listen(Peer& peer)
	{
		try
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
// 				std::cout<<"|<\n";
// 				for(auto i=0;i<100;++i)
// 					std::cout<<data[i];
// 				std::cout<<">|"<<std::endl;
				switch(data[0])
				{
					case 'M': //Message
					{
						std::cerr<<"M\n";
						Message m(data,peer.count());
						SendAck(m.ack(),peer.ipOf(m.user()));
						peer.newMessage(m);
						break;

					}				
					case 'A': //Ack
					{
						std::cerr<<"A\n";
						int n;
						std::sscanf(data,"A\n%d",&n);
						ack_modify(n,true);
						break;
					}
					case 'I'://Info
					{
						std::cerr<<"I\n";
						std::istringstream in(data);
						std::string i,user,ip;
						std::getline(in,i);
						std::getline(in,user);
						std::getline(in,ip);
						std::getline(in,i);
						SendAck(std::stoi(i),ip);
						peer.newUser(user,ip);
						break;
					}
					case 'R'://Request for Info
					{
						std::cerr<<"R\n";
						std::istringstream in(data);
						std::string i,user;
						std::getline(in,i);
						std::getline(in,user);
// 						SendAck(std::stoi(i),peer.ipOf(user));
						peer.sendUserData(peer.ipOf(user));
					}
				}
			}
		}
		catch(const Poco::Net::NetException& e)
		{
			std::cout<<e.what()<<e.displayText();
		}
			
	}

}