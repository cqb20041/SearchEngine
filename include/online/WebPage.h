#pragma once 
#include <string>

using std::string;

struct RssItem 
{ 
    string _link;//网页链接
    string _title;//网页标题 
    string _content;//网页内容 
}; 

class WebPage
{
public:
    WebPage(const string &doc);
    const string GetDoc() const;
    const int GetId() const;
    const string GetLink() const;
    const string GetTitle() const;
    const string GetContent() const;
    string GetBegContent(size_t n);
    bool IsUTF8Lead(const char byte);
private:
    int _id;//网页id
    string _doc;//整篇文章
    RssItem _rss;
};//网页类 
