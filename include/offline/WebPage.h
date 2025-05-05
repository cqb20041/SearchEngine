#pragma once 

#include "../../include/offline/SplitTool.h" 

struct RssItem 
{ 
    string _link;//网页链接
    string _title;//网页标题 
    string _content;//网页内容 
}; 

class WebPage
{
public:
    WebPage(const int &id,
            const RssItem &rss,
            const unordered_set<string> &stopwords,
            SplitTool &st);
    void BuildDocDict(const unordered_set<string> &stopwords);
    //创建文章分词库以及对应词频
    
    string &GetDoc();
    int &GetId();
    string &GetLink();
    string &GetTitle();
    string &GetContent();
    map<string,int> &GetDocDict();

private:
    int _id;//网页id
    string _doc;//整篇文章
    RssItem _rss;
    map<string,int> _doc_dict;//文章分词库以及对应词频
    SplitTool &_st;//分词工具
};//网页类 
