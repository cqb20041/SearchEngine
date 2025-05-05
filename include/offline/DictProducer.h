#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "./SplitTool.h"

using std::vector;
using std::pair;
using std::string;
using std::map;

class DictProducer
{
public:
    DictProducer();
    DictProducer(SplitTool &st);
private:
    void GetFiles(const string &copora_path);//获取语料库
    void BuildStopwords(const string &stopwords_path);//创建停用词库
    
    void BuildEnDict();//创建英文词典
    void DealEnLine(string &line);//去除英文大写和标点符号
    void BuildCnDict();//创建词典
    void BuildIndex();//创建词典索引
    size_t GetByteNum_UTF8(const char byte);//获取字节数

    void StoreDict(const string &dict_path);//存储词典
    void StoreIndex(const string &index_path);//存储词典索引
private:
    vector<string> _files;//语料文件
    unordered_set<string> _stopwords;//停用词
    map<string,int> _dict;//词典
    unordered_map<string,set<int>> _index;//词典索引
    SplitTool &_st;//分词工具
};

