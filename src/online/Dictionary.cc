#include "../../include/online/Dictionary.h"
#include "../../include/configuration/Configuration.h"
#include "../../include/mylogger/MyLogger.h"

#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using std::cerr;
using std::endl;

Dictionary *Dictionary::_p_instance = Dictionary::GetInstance();

Dictionary *Dictionary::GetInstance()
{    
    if(_p_instance == nullptr)                                                                     
    {
        string dict_path = Configuration::GetInstance()->GetConfmap()["dict_path"];
        string index_path = Configuration::GetInstance()->GetConfmap()["index_path"];

        _p_instance = new Dictionary(dict_path,index_path);
        atexit(destroy);
    }
    return _p_instance;
}

vector<pair<string,int>> &Dictionary::GetDict()
{
    return _dict;
}

unordered_map<string,set<int>> &Dictionary::GetIndex()
{
    return _index;
}

Dictionary::Dictionary(const string &dict_path,const string &index_path)
{
    LoadDict(dict_path);
    LogInfo("LoadDict successed");
    LoadIndex(index_path);
    LogInfo("LoadIndex successed");
}

Dictionary::~Dictionary()
{

}

void Dictionary::LoadDict(const string &dict_path)
{
    _dict.clear();
    ifstream ifs;
    ifs.open(dict_path);
    if(!ifs.good())
    {
        LogError("LoadDict failed");
        return;
    }
    string line,word,frequency_str;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        iss >> word >> frequency_str;
        _dict.push_back(make_pair(word,stoi(frequency_str)));
    }
    ifs.close();
}

void Dictionary::LoadIndex(const string &index_path)
{
    _index.clear();
    ifstream ifs;
    ifs.open(index_path);
    if(!ifs.good())
    {
        LogError("LoadIndex failed");
        return;
    }
    string line,word,word_num_str;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        iss >> word;
        _index[word].clear();
        while(iss >> word_num_str)
        {
            _index[word].insert(stoi(word_num_str));
        }
    }
    ifs.close();
}

void Dictionary::destroy()
{
    if(_p_instance)
    {
        delete _p_instance;
        _p_instance = nullptr;
    }
}
