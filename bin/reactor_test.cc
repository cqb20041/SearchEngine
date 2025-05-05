#include "../include/reactor/tcpserver/TcpServer.h"
#include "../include/reactor/threadpool/ThreadPool.h"
#include "../include/mylogger/MyLogger.h"

//测试反应器，包含线程池与网络编程
class EchoServer
{
public:
    EchoServer(size_t thread_num,size_t que_size,const string& ip,unsigned short port);
    ~EchoServer();
    void Start();
    void Stop();

    void OnConnection(const TcpConnectionPtr& con);
    void OnMessage(const TcpConnectionPtr& con);
    void OnClose(const TcpConnectionPtr& con);

private:
    ThreadPool _pool;
    TcpServer _server;
};

class MyTask{
public:
    MyTask(const string& msg,const TcpConnectionPtr& con)
        :_msg(msg)
        ,_con(con)
    {

    }
    void process(){
        for(auto& ch:_msg){
            if(isalpha(ch)) ch = toupper(ch);
        }
        _con->SendToLoop(_msg);
    }
private:
    string _msg;
    TcpConnectionPtr _con;
};

EchoServer::EchoServer(size_t thread_num,
                       size_t que_size,
                       const string& ip,
                       unsigned short port)
:_pool(thread_num,que_size)
,_server(ip,port)
{

}

EchoServer::~EchoServer(){
    
}

void EchoServer::Start(){
    _pool.Start();
    using namespace std::placeholders;
    _server.SetAllCallback(bind(&EchoServer::OnConnection,this,_1),
                           bind(&EchoServer::OnMessage,this,_1),
                           bind(&EchoServer::OnClose,this,_1));
    _server.Start();
}

void EchoServer::Stop(){
    _server.Stop();
    _pool.Stop();
}

void EchoServer::OnConnection(const TcpConnectionPtr& con){
    LogInfo((con->Show() + "has connected").c_str());
}

void EchoServer::OnMessage(const TcpConnectionPtr& con){
    string msg = con->RecLine();
    cout << "Receive message:" + msg << endl;
    LogInfo(("Receive message:" + msg).c_str());
    MyTask task(msg,con);
    _pool.AddTask(bind(&MyTask::process,task));
}

void EchoServer::OnClose(const TcpConnectionPtr& con){
    LogInfo((con->Show() + "has closed").c_str());
}

void test_reactor(){
    EchoServer echo_server(4,10,"192.168.48.128",8888);
    echo_server.Start();
}
int main(int argc,char *argv[])
{
    test_reactor();
    return 0;
}

