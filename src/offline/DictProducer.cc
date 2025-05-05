#include <iostream>
#include <dirent.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iterator>

#include "../../include/offline/DictProducer.h"
#include "../../include/configuration/Configuration.h"
#include "../../include/mylogger/MyLogger.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;

DictProducer::DictProducer(SplitTool &st)
:_st(st)
{
    string en_corpora_path = Configuration::GetInstance()->GetConfmap()["en_corpora_path"];
    string cn_corpora_path = Configuration::GetInstance()->GetConfmap()["cn_corpora_path"];
    GetFiles(en_corpora_path);
    GetFiles(cn_corpora_path);
    LogInfo("GetFiles successed");

    string en_stopwords_path = Configuration::GetInstance()->GetConfmap()["en_stopwords_path"];
    string cn_stopwords_path = Configuration::GetInstance()->GetConfmap()["cn_stopwords_path"];
    BuildStopwords(en_stopwords_path);
    BuildStopwords(cn_stopwords_path);
    LogInfo("BuildStopwords successed");

    BuildEnDict();
    BuildCnDict();
    LogInfo("BuildDict successed");

    BuildIndex();
    LogInfo("BuildIndex successed");

    string dict_path = Configuration::GetInstance()->GetConfmap()["dict_path"];
    StoreDict(dict_path);
    LogInfo("StoreDict successed");

    string index_path = Configuration::GetInstance()->GetConfmap()["index_path"];
    StoreIndex(index_path);
    LogInfo("StoreIndex successed");
}

void DictProducer::GetFiles(const string &corpora_path)
{
    DIR *pdir = opendir(corpora_path.c_str());
    if(pdir == nullptr)
    {
        LogError("GetFiles failed");
        return;
    }
    struct dirent *pdirent;
    while((pdirent = readdir(pdir)) != nullptr)
    {
        if(strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") == 0){
            continue;
        }
        _files.push_back(string(corpora_path + "/" + pdirent->d_name));
    }
    closedir(pdir);
}

void DictProducer::BuildStopwords(const string &stopwords_path)
{
    ifstream ifs;
    ifs.open(stopwords_path);
    if(!ifs.good())
    {
        LogError("BuildStopwords failed");
        return;
    }
    string line,word;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        while(iss >> word) _stopwords.insert(word);
    }
    ifs.close();
}

void DictProducer::BuildEnDict()
{
    for(auto &file:_files)
    {
        ifstream ifs;
        ifs.open(file);
        if(!ifs.good())
        {
            LogError("BuildEnDict failed");
            return;
        }
        string line;
        while(getline(ifs,line))
        {
            DealEnLine(line);
            istringstream iss(line);
            string word;
            while(iss >> word)
            {
                if(word[0] != ' ' && word[0] != '\n' && 
                   _stopwords.find(word) == _stopwords.end())
                {
                     if(_stopwords.find(word) == _stopwords.end())
                     {
                         if(_dict.find(word) != _dict.end()){
                             _dict[word]++;
                         }else{
                             _dict[word] = 1;
                         }
                     }
                }
            }
        }
        ifs.close();
    }
}

void DictProducer::DealEnLine(string &line)
{
    for(auto &elem:line)
    {
        if(!isalpha(elem)) elem = ' ';
        else if(isupper(elem)) elem = tolower(elem);
    }
}

void DictProducer::BuildCnDict()
{
    for(auto &file:_files)
    {
        ifstream ifs;
        stringstream ss;
        string line;
        ifs.open(file);
        if(!ifs.good())
        {
            LogError("BuildCnDict failed");
            return;
        }
        ss << ifs.rdbuf();
        line = ss.str();
        vector<string> words = _st.Split(line);
        for(auto &word:words)
        {
            if(3 == GetByteNum_UTF8(word[0]) && 
               _stopwords.find(word) == _stopwords.end())
            {
                if(_dict.find(word) != _dict.end()){
                    _dict[word]++;
                }else{
                    _dict[word] = 1;
                }
            }
        }
        ifs.close();
    }
}

void DictProducer::BuildIndex()
{
    int word_num = 1;
    for(auto &elem:_dict)
    {
        string word = elem.first;
        size_t loc = 0;
        while(loc < word.size())
        {
            size_t byte_num = GetByteNum_UTF8(word[loc]);
            string subword = word.substr(loc,byte_num);
            _index[subword].insert(word_num);
            loc += byte_num;
        }
        word_num++;
    }
    return;
}

size_t DictProducer::GetByteNum_UTF8(const char byte)
{
    if ((byte & 0b10000000) == 0b00000000) return 1;
    if ((byte & 0b11100000) == 0b11000000) return 2;
    if ((byte & 0b11110000) == 0b11100000) return 3;
    if ((byte & 0b11111000) == 0b11110000) return 4;
    return 1;
}

void DictProducer::StoreDict(const string &dict_path)
{
    ofstream ofs;
    ofs.open(dict_path,std::ios::trunc);
    if(!ofs.good())
    {
        LogError("StoreDict failed");
        return;
    }
    for(auto &elem:_dict) ofs << elem.first << " " << elem.second << endl;
    ofs.close();
}

void DictProducer::StoreIndex(const string &index_path)
{
    ofstream ofs;
    ofs.open(index_path,std::ios::trunc);
    if(!ofs.good())
    {
        LogError("StoreIndex failed");
        return;
    }
    for(auto &elem:_index)
    {
        ofs << elem.first;
        for(auto &num:elem.second) ofs << " " << num;
        ofs << endl;
    }
    ofs.close();
}
