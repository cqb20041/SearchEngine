#include "../../include/offline/WebPage.h"

#include <regex>

using std::regex;

WebPage::WebPage(const int &id,
                 const RssItem &rss,
                 const unordered_set<string> &stopwords,
                 SplitTool &st)
    :_id(id),
    _doc(string("<doc>\n<id>") + to_string(id) +
         string("</id\n<link>") + rss._link +
         string("</link>\n<title>") + rss._title +
         string("</title>\n<content>\n") + rss._content +
         string("\n</content>\n</doc>\n")),
    _rss(rss),
    _st(st)
{
    BuildDocDict(stopwords); 
}

void WebPage::BuildDocDict(const unordered_set<string> &stopwords)
{
    vector<string> words = _st.Split(_rss._title + _rss._content);
    for(auto &word:words)
    {
        if(word[0] != '\n' && word[0] != ' ' && 
           stopwords.find(word) == stopwords.end())
        {
            if(_doc_dict.find(word) == _doc_dict.end()) _doc_dict[word] = 1;
            else _doc_dict[word]++;
        }
    }
}

string &WebPage::GetDoc()
{
    return _doc;
}

int &WebPage::GetId()
{
    return _id;
}

string &WebPage::GetLink()
{
    return _rss._link;
}

string &WebPage::GetTitle()
{
    return _rss._title;
}

string &WebPage::GetContent()
{
    return _rss._content;
}

map<string,int> &WebPage::GetDocDict()
{
    return _doc_dict;
}
