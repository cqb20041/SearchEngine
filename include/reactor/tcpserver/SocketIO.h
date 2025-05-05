#pragma once

class SocketIO
{
public:
    SocketIO(int fd);
    ~SocketIO();
    int Readn(char* buf, int len);
    int Writen(const char* buf, int len);
    int ReadLine(char* buf,int len);
private:
    int _fd;
};

