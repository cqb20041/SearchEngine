#pragma once

#include "../../thirdparty/simhash/cppjieba/Jieba.hpp"

#include <vector>
#include <string>

using std::vector;
using std::string;

using namespace cppjieba;

class SplitTool
{
public:
    SplitTool();
    ~SplitTool();
    vector<string> Split(const string &line);
private:
    Jieba _jb;
};//分词工具
