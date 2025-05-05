#pragma once

#include "PageLibrary.h"
#include "SplitTool.h"

#include <nlohmann/json.hpp>

struct ResultItem{
    string _link;
    string _title;
    string _beg_content;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResultItem, _link, _title, _beg_content)


class PageResult
{
    friend class PageCompare;
public: 
    PageResult(WebPage wp,double cos);
    ~PageResult();
    WebPage GetPage() const;
private:
    WebPage _wp;
    double _cos;
};

class PageCompare
{
public:
    bool operator()(PageResult &lhs,PageResult &rhs);
};

class PageSearcher
{
public:
    PageSearcher(SplitTool &st);
    ~PageSearcher();
    
    string DoQuery(const string &query_word);

private:
    double CulPageFeature(map<string,double> &lhs,map<string,double> &rhs);
    //计算网页特征值
    
    string SerializeForNothing();
    string Serialize(const vector<ResultItem> &pageresult_vec);
    //json打包工具
    
private:
    SplitTool &_st;
    string &_page_lib_path;
    unordered_set<string> &_stopwords;
    unordered_map<int,pair<size_t,size_t>> &_offset_lib;
    unordered_map<string,unordered_map<int,double>> &_invert_index;
};//通过关键词查询网页

