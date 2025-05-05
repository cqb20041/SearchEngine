#include "../../include/online/PageLibrary.h"
#include "../../include/configuration/Configuration.h"
#include "../../include/mylogger/MyLogger.h"

#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using std::cerr;
using std::endl;
using std::make_pair;

PageLibrary *PageLibrary::_p_instance = PageLibrary::GetInstance();

PageLibrary *PageLibrary::GetInstance()
{
    if(_p_instance == nullptr)
    {
        _p_instance = new PageLibrary();
        atexit(destroy); 
    }
    return _p_instance;
}

string &PageLibrary::GetPageLibPath()
{
    return _page_lib_path;
}

unordered_set<string> &PageLibrary::GetStopwords()
{
    return _stopwords;
}

unordered_map<int,pair<size_t,size_t>> &PageLibrary::GetOffsetLib()
{
    return _offset_lib;
}

unordered_map<string,unordered_map<int,double>> &PageLibrary::GetInvertIndex()
{
    return _invert_index;
}

PageLibrary::PageLibrary()
    :_page_lib_path(Configuration::GetInstance()->GetConfmap()["page_lib_path"])
{
    string en_stopwords_path =
        Configuration::GetInstance()->GetConfmap()["en_stopwords_path"];       
    string cn_stopwords_path =
        Configuration::GetInstance()->GetConfmap()["cn_stopwords_path"];       
    string offset_lib_path = 
        Configuration::GetInstance()->GetConfmap()["offset_lib_path"];       
    string invert_index_path = 
        Configuration::GetInstance()->GetConfmap()["invert_index_path"];       

    LoadStopWords(en_stopwords_path);
    LoadStopWords(cn_stopwords_path);
    LogInfo("LoadStopWords successed");

    LoadOffsetLib(offset_lib_path);
    LogInfo("LoadOffsetLib successed");
    LoadInvertIndex(invert_index_path);
    LogInfo("LoadInvertIndex successed");
}

PageLibrary::~PageLibrary()
{

}

void PageLibrary::LoadStopWords(const string &stopwords_path)
{
    ifstream ifs;
    ifs.open(stopwords_path);
    if(!ifs.good())
    {
        LogError("LoadStopWords failed");
        return;
    }
    string line,word;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        iss >> word;
        _stopwords.insert(word);
    }
}

void PageLibrary::LoadOffsetLib(const string &offset_lib_path)
{
    _offset_lib.clear();
    ifstream ifs;
    ifs.open(offset_lib_path);
    if(!ifs.good())
    {
        LogError("LoadOffsetLib");
        return;
    }
    string line,id,loc,len;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        iss >> id >> loc >> len;
        _offset_lib[stoi(id)] = make_pair(stoi(loc),stoi(len));
    }
    ifs.close();
}

void PageLibrary::LoadInvertIndex(const string &invert_index_path)
{
    _invert_index.clear();
    ifstream ifs;
    ifs.open(invert_index_path);
    if(!ifs.good())
    {
        LogError("LoadInvertIndex failed");
        return;
    }
    string line,id,loc,len;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        string word,id,weight_coefficient;
        iss >> word;
        _invert_index[word].clear();
        while(iss >> id >> weight_coefficient)
            _invert_index[word].insert(make_pair(stoi(id),stod(weight_coefficient)));
    }
    ifs.close();
}

void PageLibrary::destroy()
{
    if(_p_instance)
    {
        delete _p_instance;
        _p_instance = nullptr;
    }
}
