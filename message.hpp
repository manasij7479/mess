#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include<string>
namespace Mess
{
	class Message
	{
	public:
		Message(std::string u,std::string t,long long o):user_(u),text_(t),ord(o){}
		Message(char data[1024],long long o);
		long long order()const 
		{
			return ord;
		}
		int ack()const
		{
			return ack_;
		}
		std::string user()const
		{
			return user_;
		}
		std::string text()const
		{
			return text_;
		}
		
		bool operator < (const Message& m) const
		{
			return order()<m.order();
		}
		void send(std::string ip);
		
		void setOrder(long long l)
		{
			ord=l;
		}
	private:
		std::string user_;
		std::string text_;
		long long ord;
		int ack_;
	};
}
#endif