#pragma once

#include "WebPage.h"

#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

using std::set;
using std::unordered_set;
using std::map;
using std::unordered_map;
using std::pair;

class PageLibrary
{
public:
    static PageLibrary *GetInstance(); 
    string &GetPageLibPath();
    unordered_set<string> &GetStopwords();
    unordered_map<int,pair<size_t,size_t>> &GetOffsetLib();
    unordered_map<string,unordered_map<int,double>> &GetInvertIndex();

private:
    PageLibrary();
    ~PageLibrary();
    void LoadStopWords(const string &stop_words_path);
    void LoadOffsetLib(const string &offset_lib_path);
    void LoadInvertIndex(const string &invert_index_path);
    static void destroy();
    PageLibrary(const PageLibrary&) = delete ;
    PageLibrary &operator=(const PageLibrary&) = delete ;

private:
    static PageLibrary *_p_instance;
    string _page_lib_path;
    unordered_set<string> _stopwords;
    unordered_map<int,pair<size_t,size_t>> _offset_lib;
    unordered_map<string,unordered_map<int,double>> _invert_index;
};//读取网页库路径、停用词库、网页偏移库、倒排索引库

