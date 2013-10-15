#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include<string>
namespace Mess
{
	class Message
	{
	public:
		Message(std::string u,std::string t,long long o):user(u),text(t),ord(o){}
		long long order()const 
		{
			return ord;
		}
		bool operator < (const Message& m) const
		{
			return order()<m.order();
		}
		void send(std::string ip);
	private:
		std::string user;
		std::string text;
		long long ord;
	};
}
#endif