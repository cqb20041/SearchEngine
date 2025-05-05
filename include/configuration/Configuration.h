#pragma once

#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

class Configuration
{
public:
    static Configuration *GetInstance();
    unordered_map<string,string> &GetConfmap();
private:
    Configuration(const string &file_path);
    ~Configuration();
    void LoadConfmap();
    static void destroy();
    Configuration(const Configuration&) = delete ;
    Configuration &operator=(const Configuration&) = delete ;
private:
    static Configuration *_p_instance;
    string _file_path;
    unordered_map<string,string> _confmap;
};//读取配置文件

