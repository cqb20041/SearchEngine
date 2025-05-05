#include "../../include/online/SplitTool.h"

SplitTool::SplitTool()
    :_jb("/home/chenqinbin/learn.C++/8_SearchEngine/thirdparty/simhash/dict/jieba.dict.utf8",
         "/home/chenqinbin/learn.C++/8_SearchEngine/thirdparty/simhash/dict/hmm_model.utf8",
         "/home/chenqinbin/learn.C++/8_SearchEngine/thirdparty/simhash/dict/user.dict.utf8")
{
    
}

vector<string> SplitTool::Split(const string &line)
{
    vector<string> words;
    _jb.CutForSearch(line,words);
    return words;
}

SplitTool::~SplitTool()
{

}
