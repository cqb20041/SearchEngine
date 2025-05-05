#pragma once

#include "Dictionary.h"
#include <nlohmann/json.hpp>

#include <string>
#include <queue>
#include <vector>

using std::string;
using std::priority_queue;
using std::vector;

class WordResult
{
    friend class WordCompare;
public:
    WordResult(string word,int freg,int dist);
    ~WordResult();
    string GetWord() const;
private:
    string _word;
    int _freq;
    int _dist;
};//单一查询结果类:单词，词频和最小编辑距离

class WordCompare
{
public:
    bool operator()(WordResult &lhs,WordResult &rhs);
};

class KeyRecommender
{
public:
    KeyRecommender();
    ~KeyRecommender();
    
    string DoQuery(const string &query_word);
    //查询词语：1)查询词典索引；2)查询词典；3)计算查询结果的最小编辑距离
    
private:
    set<int> GetIndexIDs(const string &query_word);//查询字典索引ID
    size_t GetByteNum_UTF8(const char byte);
    size_t GetLength(const string &str);
    int Distance(const string &lhs,const string &rhs);//最小编辑距离算法

    string SerializeForNothing();
    string Serialize(const vector<string> &wordresult_vec);
    //json打包工具
    
private:
    vector<pair<string,int>> &_dict;
    unordered_map<string,set<int>> &_index;
//    PtrTcpConnection _ptc;客户端接口
};

