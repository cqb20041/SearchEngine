#include "../../include/client/Client.h"
#include "../../include/mylogger/ClientLogger.h"

#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

Client::Client(const string& ip,unsigned short port)
:_accepted(ip,port),
_con(_accepted.fd())
{

}

void Client::Start(){
    _accepted.Connect();
    int msgID;
    string msg;
    cout << endl << "|-----------------------------BEGIN-----------------------------|" << endl;
    while(1){
        ShowMenu();
        cout << "|Please input '1','2' or '3':                                   |" << endl;
        cout << '|';
        cin >> msgID;
        if(1 == msgID){
            cout << "|Please input a word:                                           |" << endl;
            cout << '|';
            cin >> msg;
            while("" == msg){
                cout << "|Don't input none,input again:                                  |" << endl;
                cin >> msg;
            }
            _con.Send(Serialize(msgID,msg));
            ShowKRResult(_con.Receive());
        }else if(2 == msgID){
            cout << "|Please input a word:                                           |" << endl;
            cout << '|';
            cin >> msg;
            while("" == msg){
                cout << "|Don't input none,input again:                                  |" << endl;
                cin >> msg;
            }
            _con.Send(Serialize(msgID,msg));
            ShowPSResult(_con.Receive());
        }else if(3 == msgID){
            cout << "|------------------------------END------------------------------|" << endl;
            break;
        }else{    
            cout << endl;
            cout << "|Wrong key                                                      |" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

Client::~Client(){

}

string Client::Serialize(size_t msgID,const string& msg){
    nlohmann::json root;
    root["msgID"] = msgID;
    root["msg"] = msg;
    return root.dump();
}

void Client::ShowMenu(){
    cout << "|-------------------------SEARCH ENGINE-------------------------|" << endl
         << "|                                                               |" << endl
         << "|---------------------1: Key Recommendation---------------------|" << endl
         << "|---------------------2: Page Searcher--------------------------|" << endl
         << "|---------------------3: Close Search Engine--------------------|" << endl
         << "|                                                               |" << endl
         << "|---------------------------------------------------------------|" << endl;
}

void Client::ShowKRResult(const string& msg){
    nlohmann::json root = nlohmann::json::parse(msg);
    cout << "|---------------------------------------------------------------|" << endl;
    if(404 == root["msgID"]){
        cout << "|Finding nothing                                                |" << endl;
        cout << "|---------------------------------------------------------------|" << endl;
    }else if(1 == root["msgID"]){
        vector<string> wordresult = root["msg"].get<vector<string>>();        
        cout << "|Key Recommendation Result:                                     |" << endl;
        for(auto&word:wordresult){
            cout << "|" << word << endl;
        }
        cout << "|---------------------------------------------------------------|" << endl;
    }else{
        LogError("ShowKRResult failed");
        cout << "|-----------------------------ERROR-----------------------------|" << endl;
    }
}

void Client::ShowPSResult(const string& msg){
    nlohmann::json root = nlohmann::json::parse(msg);
    cout << "|---------------------------------------------------------------|" << endl;
    if(404 == root["msgID"]){
        cout << "|Finding nothing                                                |" << endl;
        cout << "|---------------------------------------------------------------|" << endl;
    }else if(2 == root["msgID"]){
        vector<ResultItem> wordresult = root["msg"].get<vector<ResultItem>>();        
        cout << "|Page Search Result:                                            |" << endl;
        int count = 0;
        for(auto& page:wordresult){
            cout << "|---------------------------------------------------------------|" << endl;
            count++;
            if(5 == count){
                cout << "|Input 'n' to show more, or 'c' to continue                     |" << endl;
                cout << '|';
                string flag;
                cin >> flag;
                if(flag == "n"){
                    continue;
                }else{
                    break;
                }
                count = 0;
            }
            cout << "|[Link]" << page._link << endl
                 << "|[Title]" << page._title << endl
                 << "|[Summary]" << page._beg_content << endl;
        }
        cout << "|---------------------------------------------------------------|" << endl;
    }else{
        LogError("ShowPSResult failed");
        cout << "|-----------------------------ERROR-----------------------------|" << endl;
    }
}
