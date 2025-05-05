#include "../../include/configuration/Configuration.h"

#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using std::cout;
using std::endl;
using std::cerr;

Configuration *Configuration::_p_instance = Configuration::GetInstance();

Configuration *Configuration::GetInstance()
{
    if(_p_instance == nullptr)
    {
        _p_instance = 
            new Configuration("/home/chenqinbin/learn.C++/8_SearchEngine/conf/conf.txt");
        atexit(destroy);
    }
    return _p_instance;
}

Configuration::Configuration(const string &file_path)
    :_file_path(file_path)
{
    LoadConfmap();
}

Configuration::~Configuration()
{

}

void Configuration::LoadConfmap()
{
    ifstream ifs;
    ifs.open(_file_path,ifstream::in);
    if(!ifs.good())
    {
        return;
    }
    string line;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        string first,second;
        iss >> first >> second;
        _confmap[first] = second;
    }
    ifs.close();
}

void Configuration::destroy()
{
    if(_p_instance)
    {
        delete _p_instance;
        _p_instance = nullptr;
    }
}

unordered_map<string,string> &Configuration::GetConfmap()
{
    return _confmap;
}
