#include "../include/engine/SearchEngineServer.h"

using namespace std;

void test(){
    SplitTool st;
    KeyRecommender kr;
    PageSearcher ps(st);
    SearchEngineServer server(4,10,"192.168.48.128",8888,kr,ps);
    server.Start();
}
int main(int argc,char *argv[])
{
    test();
    return 0;
}

