#include "../include/online/SplitTool.h"
#include "../include/online/KeyRecommender.h"
#include "../include/online/PageSearcher.h"

void test_kr()//测试关键字推荐功能
{
    KeyRecommender kr;
    cout << kr.DoQuery("hello") << endl;
    cout << "<------------------------------------------------------------------->" << endl;
    cout << kr.DoQuery("武汉") << endl;
    cout << "<------------------------------------------------------------------->" << endl;
}

void test_ps()//测试网页查询功能
{
    SplitTool st;
    PageSearcher ps(st);
    cout << ps.DoQuery("GDP") << endl;
    cout << "<------------------------------------------------------------------->" << endl;
    cout << ps.DoQuery("科技") << endl;
    cout << "<------------------------------------------------------------------->" << endl;
}

int main(int argc,char *argv[])
{
    test_kr();
    test_ps();
    return 0;
}

