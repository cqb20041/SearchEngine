#include "../../include/offline/PageLibProducer.h"
#include "../../include/configuration/Configuration.h"
#include "../../include/mylogger/MyLogger.h"

#include <dirent.h>
#include <regex>

PageLibProducer::PageLibProducer(SplitTool &st,CutPageTool &cpt)
:_st(st)
,_cpt(cpt)
{
    string page_lib_path = Configuration::GetInstance()->GetConfmap()["page_lib_path"];
    string offset_lib_path = Configuration::GetInstance()->GetConfmap()["offset_lib_path"];
    string invert_index_path = Configuration::GetInstance()->GetConfmap()["invert_index_path"];

    string xml_path = Configuration::GetInstance()->GetConfmap()["xml_path"];
    GetFiles(xml_path);
    LogInfo("GetFiles successed");

    string en_stopwords_path = 
        Configuration::GetInstance()->GetConfmap()["en_stopwords_path"];
    string cn_stopwords_path = 
        Configuration::GetInstance()->GetConfmap()["cn_stopwords_path"];
    BuildStopwords(en_stopwords_path);
    BuildStopwords(cn_stopwords_path);
    LogInfo("BuildStopwords successed");

    BuildPageLib();
    LogInfo("BuildPageLib successed");
    BuildOffsetLib();
    LogInfo("BuildOffsetLib successed");
    BuildInvertIndex();
    LogInfo("BuildInvertIndex successed");
    
    StorePageLib(page_lib_path);
    LogInfo("StorePageLib successed");
    StoreOffsetLib(offset_lib_path);
    LogInfo("StoreOffsetLib successed");
    StoreInvertIndex(invert_index_path);
    LogInfo("StoreInvertIndex successed");
}

void PageLibProducer::GetFiles(const string &xml_path)
{
    DIR *pdir = opendir(xml_path.c_str());
    if(pdir == nullptr)
    {
        LogError("GetFiles failed");
        return;
    }
    struct dirent *pdirent;
    while((pdirent = readdir(pdir)) != nullptr)
    {
        if(strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") == 0)
            continue;
        _files.push_back(string(xml_path + "/" + pdirent->d_name));
    }
    closedir(pdir);
}

void PageLibProducer::BuildStopwords(const string &stopwords_path)
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

void PageLibProducer::BuildPageLib()
{
    int temp_id = 1;
    for(auto &file:_files)
    {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(file.c_str());
        if(doc.ErrorID())
        {
            LogError("BuildPageLib failed");
            return;
        }
        else
        {
            tinyxml2::XMLElement *p_node = doc.FirstChildElement("rss")
                                            -> FirstChildElement("channel")
                                            -> FirstChildElement("item");
            while(p_node)
            {
                regex reg("<[^>]+>");
                RssItem temp_rss;
                string link,title,description,content; 
                
                if(p_node -> FirstChildElement("link") != nullptr) 
                    link =  p_node -> FirstChildElement("link") -> GetText();
                else string link = "";
                
                if(p_node -> FirstChildElement("title") != nullptr) 
                    title =  p_node -> FirstChildElement("title") -> GetText();
                else string title = "";

                if(p_node -> FirstChildElement("description") != nullptr) 
                    description =  p_node -> FirstChildElement("description") -> GetText();
                else string description = "";

                if(p_node -> FirstChildElement("content:encoded") != nullptr)
                    content = p_node -> FirstChildElement("content:encoded") -> GetText();
                else string content = "";

                temp_rss = {regex_replace(link,reg,""),
                            regex_replace(title,reg,""),
                            regex_replace(description,reg,"") + 
                            regex_replace(content,reg,"")};
                WebPage temp_wp(temp_id,temp_rss,_stopwords,_st);

                if(!_cpt.CutPage(temp_wp)) 
                {
                    _page_lib.push_back(temp_wp);
                    temp_id++;
                }
                p_node = p_node -> NextSiblingElement("item");
            }
        }
    }
}

void PageLibProducer::BuildOffsetLib()
{
    size_t begin_byte = 0;
    for(auto &elem:_page_lib)
    {
        size_t byte_num = elem.GetDoc().size();
        _offset_lib.push_back(make_pair(begin_byte,byte_num));
        begin_byte += byte_num;
    }
}

void PageLibProducer::BuildInvertIndex()
{
    int web_page_num = _page_lib.size();
    
    map<string,int> words_df;
    for(auto &wb:_page_lib)
    {
        for(auto &elem:wb.GetDocDict())
        {
            string word = elem.first;
            if(words_df.find(word) == words_df.end()) words_df[word] = 1;
            else words_df[word]++;
        }
    }//统计包含词语的文章数量

    for(auto &wp:_page_lib)
    {
        int doc_id = wp.GetId();
        
        map<string,double> words_weight;
        words_weight.clear();
        for(auto &elem:wp.GetDocDict())
        {
            string word = elem.first;
            int tf = elem.second;
            int df = words_df[word];
            double w = static_cast<double>(tf) * 
                log2 (static_cast<double>(web_page_num) / (static_cast<double>(df) + 1));
            words_weight.insert(make_pair(word,w));
        }//计算一篇文章中每个词语的权重

        double w_sum = 0; 
        for(auto &elem:words_weight) w_sum += (elem.second * elem.second);
        //计算一篇文章全部词语的权重平方和

        for(auto &elem:wp.GetDocDict())
        {
            string word = elem.first;
            double w = words_weight[word];
            double w_coefficient =  w / sqrt(w_sum);
            if(_invert_index.find(word) == _invert_index.end())
            {
                _invert_index[word].clear();
                _invert_index[word].insert(make_pair(doc_id,w_coefficient));
            }
            else _invert_index[word].insert(make_pair(doc_id,w_coefficient));
        }//计算一篇文章每个词语的权重系数并存入类的对应容器
    }
}

void PageLibProducer::StorePageLib(const string &page_lib_path)
{
    ofstream ofs;
    ofs.open(page_lib_path,std::ios::trunc);
    if(!ofs.good())
    {
        LogError("StorePageLib failed");
        return;
    }
    for(auto &elem:_page_lib) ofs << elem.GetDoc();
    ofs.close();
}

void PageLibProducer::StoreOffsetLib(const string &offset_lib_path)
{
    ofstream ofs;
    ofs.open(offset_lib_path,std::ios::trunc);
    if(!ofs.good())
    {
        LogError("StoreOffsetLib failed");
        return;
    }
    for(size_t i = 0;i<_offset_lib.size();i++){
        ofs << i + 1 << " " 
            << _offset_lib[i].first << " " 
            << _offset_lib[i].second << endl;
    }
    ofs.close();
}

void PageLibProducer::StoreInvertIndex(const string &invert_index_path)
{
    ofstream ofs;
    ofs.open(invert_index_path,std::ios::trunc);
    if(!ofs.good())
    {
        LogError("StoreInvertIndex failed");
        return;
    }
    for(auto &elem_A:_invert_index)
    {
        ofs << elem_A.first;
        for(auto &elem_B:elem_A.second)
        {
            ofs << " " << elem_B.first << " " << elem_B.second;
        }
        ofs << endl;
    }
    ofs.close();
}



