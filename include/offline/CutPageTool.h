#pragma once
#include "../../thirdparty/simhash/Simhasher.hpp"
#include "WebPage.h"

using namespace simhash;

class HashKey
{
    friend class CutPageTool;
private:
    HashKey(uint64_t i);
private:
    uint64_t key_A;
    uint64_t key_B;
    uint64_t key_C;
    uint64_t key_D;
    vector<uint64_t> hash_A;
    vector<uint64_t> hash_B;
    vector<uint64_t> hash_C;
    vector<uint64_t> hash_D;
};

class CutPageTool
{
public:
    CutPageTool();
    bool CutPage(WebPage &wb);
private:
    vector<HashKey> _key_list;
    Simhasher _sim;
    const size_t _top_n = 5;
};

