#pragma once

#include "ClientConnection.h"
#include "Accepted.h"

#include <string>
#include <nlohmann/json.hpp>

struct ResultItem{
    string _link;
    string _title;
    string _beg_content;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResultItem, _link, _title, _beg_content)

using std::string;

class Client
{
public:
    Client(const string& ip,unsigned short port);
    ~Client();

    void Start();

    string Serialize(size_t msgID,const string& msg);

    void ShowMenu();
    void ShowKRResult(const string& msg);
    void ShowPSResult(const string& msg);

private:
    Accepted _accepted;
    ClientConnection _con;
};

