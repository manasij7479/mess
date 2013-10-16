#include <iostream>
#include <thread>
#include "mess.hpp"
#include "comm.hpp"
// int main(int argc,char** argv)
// {
// 	std::string user,ip;
// 	 
// 	switch(argc)
// 	{
// 		case 3:ip=argv[2];
// 		case 2:user=argv[1];break;
// 		default: std::cerr<<"Invalid Options.";return -1;
// 	}
// 	
// 	Mess::Peer peer(user);
// 	if(ip!="")
// 		peer.sync(ip);
// 	
// 	return peer(); 
// }
int main()
{
	Mess::Peer p("manasij7479","192.168.2.134");
	
// 	std::thread t(Mess::Protocol::Listen,std::ref(p));
// 	
// 	p.newUser("foo","115.187.43.148");
// 	std::this_thread::sleep_for(std::chrono::milliseconds(500));
// 	Mess::Protocol::SendMessage("foo","Hello World","115.187.43.148");
// 	
// 	for(auto m:p.copyMessages())
// 	{
// 		std::cout<<m.user()<<" : "<<m.text()<<std::endl;
// 	}
// 	
// 	p.quit();
// 	t.join();
	return p();
	
}