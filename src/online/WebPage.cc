#include "../../include/online/WebPage.h"
#include <iostream>
#include <regex>

using std::regex;
using std::cout;
using std::endl;

WebPage::WebPage(const string &doc)
    :_doc(doc)
{
    size_t beg = 0,end = 0;

    beg = doc.find("<id>");
    end = doc.find("</id>");
    string id_str = doc.substr(beg + 4,end - beg - 4);
    _id = stoi(id_str);

    beg = doc.find("<link>");
    end = doc.find("</link>");
    _rss._link = doc.substr(beg + 6,end - beg - 6);

    beg = doc.find("<title>");
    end = doc.find("</title>");
    _rss._title = doc.substr(beg + 7,end - beg - 7);

    beg = doc.find("<content>");
    end = doc.find("</content>");
    _rss._content = doc.substr(beg + 10,end - beg - 11);
}

const string WebPage::GetDoc() const
{
    return _doc;
}

const int WebPage::GetId() const
{
    return _id;
}

const string WebPage::GetLink() const
{
    return _rss._link;
}

const string WebPage::GetTitle() const
{
    return _rss._title;
}

const string WebPage::GetContent() const
{
    return _rss._content;
}

string WebPage::GetBegContent(size_t n) 
{
    size_t count = 0;
    size_t i = 0;
    while(i < _rss._content.size()){
        if(IsUTF8Lead(_rss._content[i])){
            count++;
            if(n + 1 ==  count) break;
        }
        i++;
    }
    return _rss._content.substr(0,i);
}

bool WebPage::IsUTF8Lead(const char byte)                                              
{
    return (byte & 0xC0) != 0x80;
}
