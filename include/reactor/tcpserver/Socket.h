#pragma once
 #include "../NonCopyable.h"

class Socket
:NonCopyable
{
public:
    Socket();
    explicit Socket(int fd); 
    ~Socket();
    int fd() const; 
private:
    int _fd; 
}; 
