#ifndef MESS_USERMAP_HPP
#define MESS_USERMAP_HPP
#include <map>
namespace mess
{
    enum class UserType
    {
        Self,
        Parent,
        Child
    };
    struct UserInfo
    {
        std::string name;
        std::string addr;
        UserType type;
        
    };
    // username -> UserInfo
    typedef std::map<std::string,UserInfo> UserMap;
    //Design a new class when needed
    
    UserInfo Self;
    UserInfo Parent;
    
}
#endif