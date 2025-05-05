#include "../../include/online/PageSearcher.h"
#include "../../include/mylogger/MyLogger.h"

PageResult::PageResult(WebPage wp,double cos)
    :_wp(wp),
    _cos(cos)
{

}

PageResult::~PageResult(){

}

WebPage PageResult::GetPage() const
{
    return _wp;
}

bool PageCompare::operator()(PageResult &lhs,PageResult &rhs)
{
    if(lhs._cos < rhs._cos) return true;
    else if(lhs._cos == rhs._cos)
    {
        if(lhs._wp.GetId() > rhs._wp.GetId()) return true;
        else return false;
    }
    else return false;
}

PageSearcher::PageSearcher(SplitTool &st)
    :_st(st),
    _page_lib_path(PageLibrary::GetInstance()->GetPageLibPath()),
    _stopwords(PageLibrary::GetInstance()->GetStopwords()),
    _offset_lib(PageLibrary::GetInstance()->GetOffsetLib()),
    _invert_index(PageLibrary::GetInstance()->GetInvertIndex())
{

}

PageSearcher::~PageSearcher(){

}

string PageSearcher::SerializeForNothing(){
    nlohmann::json root;
    root["msgID"] = 404;
    root["msg"] = "Finding nothing";
    return root.dump();
}

string PageSearcher::Serialize(const vector<ResultItem> &pageresult_vec){
    nlohmann::json root;
    root["msgID"] = 2;
    root["msg"] = pageresult_vec;
    return root.dump();
}

string PageSearcher::DoQuery(const string &query_word)
{

    map<string,int> words_msg;
    vector<string> words_temp = _st.Split(query_word);
    for(auto &word:words_temp)
    {
        if(word[0] != '\n' && word[0] != ' ' &&
           _stopwords.find(word) == _stopwords.end())
        {
            if(words_msg.find(word) == words_msg.end()) words_msg[word] = 1;
            else words_msg[word]++;
        }
    }
        //为查询的信息分词

    set<int> ids_msg;
    set<int> ids_temp1;
    set<int> ids_temp2;
    for(auto &elem_A:words_msg)
    {
        string word = elem_A.first;
        if(ids_msg.empty())
            for(auto &elem_B:_invert_index[word])
                ids_msg.insert(elem_B.first);
        else if(!ids_msg.empty())
        {
            ids_temp1.clear();
            ids_temp2.clear();
            ids_temp1 = ids_msg;
            ids_msg.clear();
            for(auto &elem_B:_invert_index[word])
                ids_temp2.insert(elem_B.first);
            set_intersection(ids_temp1.begin(),ids_temp1.end(),
                             ids_temp2.begin(),ids_temp2.end(),
                             inserter(ids_msg,ids_msg.begin()));
        }
    }
    //查询词典索引,获得查找到的所有文章id
    
    if(ids_msg.size() == 0){
        LogInfo("PageSearcher nothing");
        return SerializeForNothing();
    }
    //没有找到网页
    
    map<string,double> X;
    map<string,double> words_weight;
    double w_sum = 0;
    int web_page_num = _offset_lib.size();
    for(auto &elem:words_msg)
    {
        string word = elem.first;
        int tf = elem.second;
        int df = ids_msg.size();
        double w = static_cast<double>(tf) *
            log2 (static_cast<double>(web_page_num) / (static_cast<double>(df) + 1));
        words_weight[word] = w;
        w_sum += (w * w);
    }
    for(auto &elem:words_weight) X.insert(make_pair(elem.first,elem.second/sqrt(w_sum)));
    //计算所查询的信息的X向量

    map<int,map<string,double>> wps_Y;
    for(auto &id:ids_msg)
    {
        for(auto &elem:words_msg)
        {
            string word = elem.first;
            wps_Y[id][word] = _invert_index[word][id];
        }
    }
    //记录文章中每个查询词的权重系数,形成Y向量
    
    priority_queue<PageResult,vector<PageResult>,PageCompare> pageresult_que; 
    ifstream ifs;
    ifs.open(_page_lib_path);
    for(auto &elem:wps_Y)
    {
        ifs.seekg(_offset_lib[elem.first].first,std::ios::beg);
        char *temp = new char[_offset_lib[elem.first].second + 1];
        ifs.read(temp,_offset_lib[elem.first].second);
        string doc(temp);
        delete [] temp;
        WebPage wp(doc);
        pageresult_que.push(PageResult(wp,CulPageFeature(X,elem.second)));
    }
    ifs.close();
    //查询网页库,并将查询到的网页排序
    //此处采用文件流的方式读取网页库，减少内存占用
    
    vector<ResultItem> pageresult_vec;
    int result_num = 10;
    for(int idx = 0;idx < result_num;++idx){
        ResultItem temp = {pageresult_que.top().GetPage().GetLink(),
                           pageresult_que.top().GetPage().GetTitle(),
                           pageresult_que.top().GetPage().GetBegContent(30)};
        pageresult_que.pop();
        pageresult_vec.push_back(temp);
        if(pageresult_que.empty()) break;
    }

    LogInfo("PageSearcher successed");
    return Serialize(pageresult_vec);
    //找到关键字，用json库打包
}

double PageSearcher::CulPageFeature(map<string,double> &lhs,map<string,double> &rhs)
{
    auto lit = lhs.begin();
    double ll = 0.000,rr = 0.000,lr = 0.000;
    while(lit != lhs.end())
    {
        ll += (lit->second * lit->second);
        rr += (rhs[lit->first] * rhs[lit->first]);
        lr += (lit->second * rhs[lit->first]);
        lit++;
    }
//    cout << lr/(sqrt(ll)*sqrt(rr)) << endl;
    return lr/(sqrt(ll)*sqrt(rr));
}
