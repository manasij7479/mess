#include <iostream>
#include <thread>
#include "mess.hpp"
#include "comm.hpp"
int main(int argc,char** argv)
{
	std::string user,ip,extip;;
	 
	switch(argc)
	{
		case 4:extip=argv[3];
		case 3:user=argv[1];ip=argv[2];break;
		default: std::cerr<<"Invalid Options.";return -1;
	}
	
	Mess::Peer peer(user,ip);
	if(extip!="")
		peer.sync(ip);
	
	return peer(); 
}