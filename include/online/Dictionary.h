#pragma once

#include <map>
#include <unordered_map>
#include <set>
#include <vector>

using std::string;
using std::map;
using std::unordered_map;
using std::set;
using std::vector;
using std::pair;

class Dictionary
{
public:
    static Dictionary *GetInstance();
    vector<pair<string,int>> &GetDict();
    unordered_map<string,set<int>> &GetIndex();
private:
    Dictionary(const string &dict_path,const string &index_path);
    ~Dictionary();
    void LoadDict(const string &dict_path);
    void LoadIndex(const string &index_path);
    static void destroy();
    Dictionary(const Dictionary&) = delete ;
    Dictionary &operator=(const Dictionary&) = delete ;
private:
    static Dictionary *_p_instance;
    vector<pair<string,int>> _dict;  
    unordered_map<string,set<int>> _index;
};//读取词典和词典索引

