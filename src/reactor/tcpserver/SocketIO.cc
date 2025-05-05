#include "../../../include/reactor/tcpserver/SocketIO.h"
#include "../../../include/mylogger/MyLogger.h"
#include <unistd.h>
#include <sys/socket.h>

SocketIO::SocketIO(int fd)
:_fd(fd)
{

}

SocketIO::~SocketIO(){

}

int SocketIO::Readn(char* buf, int len){
    int left = len;
    char* pstr = buf;
    int nread = 0;
    while(left > 0){
        nread = read(_fd,pstr,left);
        if(-1 == nread && errno == EINTR){
            continue;
        }
        else if(-1 == nread){
            LogError("Readn error");
            return -1;
        }
        else if(0 == nread){
            break;
        }
        else{
            left -= nread;
            pstr += nread;
        }
    }
    return len - left;
}

int SocketIO::ReadLine(char* buf,int len){
    int left = len - 1;
    char* pstr = buf;
    int nread = 0,total = 0;
    
    while(left > 0){
        nread = recv(_fd,pstr,left,MSG_PEEK);
        if(-1 == nread && errno == EINTR){
            continue;
        }
        else if(-1 == nread){
            LogError("ReadLine error");
            return -1;
        }
        else if(0 == nread){
            break;
        }
        else{
            for(int idx = 0;idx < nread;++idx){
                if('\n' == pstr[idx]){
                   Readn(pstr,idx + 1);
                   pstr += idx;
                   *pstr = '\0';
                   return total + idx + 1;
                }
            }
            Readn(pstr,nread);
            left -= nread;
            total += nread;
            pstr += nread;
        }
        *pstr = '\0';
        return total - left;
    }
}

int SocketIO::Writen(const char* buf, int len){
    int left = len;
    const char* pstr = buf;
    int nwrite = 0;

    while(left > 0){
         nwrite = write(_fd,pstr,left);
         if(-1 == nwrite && errno == EINTR){
             continue;
         }
         else if(-1 == nwrite){
             LogError("Writen error");
             return -1;
         }
         else if(0 == nwrite){
             break;
         }
         else{
             left -= nwrite;
             pstr += nwrite;
         }
     }
     return len - left;
}

