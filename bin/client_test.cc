#include "../include/client/Client.h"

void test(){
    Client client("192.168.48.128",8888);
    client.Start();
}
int main(int argc,char *argv[])
{
    test();
    return 0;
}

