#include "../../include/offline/CutPageTool.h"

HashKey::HashKey(uint64_t i)
    :key_A(i & 0xff000000),
    key_B(i & 0x00ff0000),
    key_C(i & 0x0000ff00),
    key_D(i & 0x000000ff)
{
    hash_A.clear();
    hash_A.push_back(i);
    hash_B.clear();
    hash_B.push_back(i);
    hash_C.clear();
    hash_C.push_back(i);
    hash_D.clear();
    hash_D.push_back(i);
}

CutPageTool::CutPageTool()
    :_sim("/home/chenqinbin/learn.C++/8_SearchEngine/thirdparty/simhash/dict/jieba.dict.utf8",
         "/home/chenqinbin/learn.C++/8_SearchEngine/thirdparty/simhash/dict/hmm_model.utf8",
         "/home/chenqinbin/learn.C++/8_SearchEngine/thirdparty/simhash/dict/idf.utf8",
         "/home/chenqinbin/learn.C++/8_SearchEngine/thirdparty/simhash/dict/stop_words.utf8")
{
    _key_list.clear();
}

bool CutPageTool::CutPage(WebPage &wb)
{
    uint64_t i = 0;
    _sim.make(wb.GetTitle() + wb.GetContent(),_top_n,i);//求文章的64位hash值

    uint64_t A = 0xff000000;
    uint64_t B = 0x00ff0000;
    uint64_t C = 0x0000ff00;
    uint64_t D = 0x000000ff;

    for(auto &hk:_key_list)
    {
        if((i & A) == hk.key_A) 
        {
            for(auto &hash:hk.hash_A) return _sim.isEqual(i,hash);
            hk.hash_A.push_back(i);
        }
        if((i & B) == hk.key_B) 
        {
            for(auto &hash:hk.hash_B) return _sim.isEqual(i,hash);
            hk.hash_B.push_back(i);
        }
        if((i & C) == hk.key_C) 
        {
            for(auto &hash:hk.hash_C) return _sim.isEqual(i,hash);
            hk.hash_C.push_back(i);
        }
        if((i & D) == hk.key_D) 
        {
            for(auto &hash:hk.hash_D) return _sim.isEqual(i,hash);
            hk.hash_D.push_back(i);
        }
    }
    HashKey hk(i);
    _key_list.push_back(hk);
    return false;
}
