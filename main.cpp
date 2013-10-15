#include<iostream>
#include "mess.hpp"

int main(int argc,char** argv)
{
	std::string user,ip;
	 
	switch(argc)
	{
		case 3:ip=argv[2];
		case 2:user=argv[1];break;
		default: std::cerr<<"Invalid Options.";return -1;
	}
	
	Mess::Peer peer(user);
	if(ip!="")
		peer.sync(ip);
	
	return peer(); 
}