#include "../include/offline/DictProducer.h"
#include "../include/offline/PageLibProducer.h"
#include "../include/offline/SplitTool.h"
#include "../include/offline/CutPageTool.h"
#include "../thirdparty/tinyxml2/tinyxml2.cpp"

void TestDict()//测试词典生成
{
    SplitTool st;
    DictProducer dict(st);
}

void TestPage()//测试网页库生成
{
    SplitTool st;
    CutPageTool cpt;
    PageLibProducer page_lib(st,cpt);
}

int main(int argc,char *argv[])
{
    TestDict();
    TestPage();
    return 0;
}

