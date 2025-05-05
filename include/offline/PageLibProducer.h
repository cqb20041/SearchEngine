#pragma once

#include "WebPage.h"
#include "CutPageTool.h"
#include "../../thirdparty/tinyxml2/tinyxml2.h"

class PageLibProducer
{
public:
    PageLibProducer(SplitTool &st,CutPageTool &cpt);
private:
    void GetFiles(const string &xml_path);//获取xml文件库
    void BuildStopwords(const string &stopwords_path);//创建停用词库

    void BuildPageLib();//创建网页库,同步完成去重
    void BuildOffsetLib();//创建网页偏移库
    void BuildInvertIndex();//创建倒排索引库

    void StorePageLib(const string &page_lib_path);//存储网页库
    void StoreOffsetLib(const string &offst_lib_path);//存储网页偏移库
    void StoreInvertIndex(const string &invert_index_path);//存储倒排索引库
private:
    vector<string> _files;//xml文件
    unordered_set<string> _stopwords;//停用词
    vector<WebPage> _page_lib;//网页库
    vector<pair<size_t,size_t>> _offset_lib;//网页偏移库
    map<string,set<pair<int,double>>> _invert_index;//倒排索引库
    SplitTool &_st;
    CutPageTool &_cpt;
};

