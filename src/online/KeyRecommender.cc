#include "../../include/online/KeyRecommender.h"
#include "../../include/mylogger/MyLogger.h"

#include <iostream>

using std::cout;

WordResult::WordResult(string word,int freg,int dist)
    :_word(word),
    _freq(freg),
    _dist(dist)
{

}

WordResult::~WordResult()
{

}

string WordResult::GetWord() const
{
    return _word;
}

bool WordCompare::operator()(WordResult &lhs,WordResult &rhs)
{
    if(lhs._dist > rhs._dist) return true;
    else if(lhs._dist == rhs._dist) 
    {
        if(lhs._freq > rhs._freq) return true;
        else if(lhs._freq == rhs._freq)
        {
            if(lhs._word > rhs._word) return true;
            else return false;
        }
        else return false;
    }
    else return false;
}

KeyRecommender::KeyRecommender()
    :_dict(Dictionary::GetInstance()->GetDict()),
    _index(Dictionary::GetInstance()->GetIndex())
{

}

KeyRecommender::~KeyRecommender()
{

}

string KeyRecommender::SerializeForNothing(){
    nlohmann::json root;
    root["msgID"] = 404;
    root["msg"] = "Finding nothing";
    return root.dump();
}

string KeyRecommender::Serialize(const vector<string> &wordresult_vec){
    nlohmann::json root;
    root["msgID"] = 1;
    root["msg"] = wordresult_vec;
    return root.dump();
}

string KeyRecommender::DoQuery(const string &query_word)
{
    set<int> index_IDs = GetIndexIDs(query_word);

    if(index_IDs.size() == 0){
        LogInfo("KeyRecommender nothing");
        return SerializeForNothing();
    }
    //没有找到关键字
    
    priority_queue<WordResult,vector<WordResult>,WordCompare> wordresult_que;
    for(auto &ID:index_IDs){
        wordresult_que.push(WordResult(_dict[ID - 1].first,
                            _dict[ID - 1].second,
                            Distance(query_word,_dict[ID - 1].first)));
    }
    //查询词典,计算查询结果的最小编辑距离,加入优先级队列自动排序
    
    vector<string> wordresult_vec;
    int result_num = 5;
    for(int idx = 0;idx < result_num;++idx){
        string temp = wordresult_que.top().GetWord();
        wordresult_que.pop();
        wordresult_vec.push_back(temp);
        if(wordresult_que.empty()) break;
    }

    LogInfo("KeyRecommender successed");
    return Serialize(wordresult_vec);
    //找到关键字，用json库打包
}

set<int> KeyRecommender::GetIndexIDs(const string &query_word){
    set<int> index_IDs;
    size_t loc = 0;
    while(loc < query_word.size())
    {
        size_t byte_num = GetByteNum_UTF8(query_word[loc]);
        string subword = query_word.substr(loc,byte_num);
        index_IDs.insert(_index[subword].begin(),_index[subword].end());
        loc += byte_num;
    }
    return index_IDs;
}

size_t KeyRecommender::GetByteNum_UTF8(const char byte)                                              
{
    if ((byte & 0b10000000) == 0b00000000) return 1;
    if ((byte & 0b11100000) == 0b11000000) return 2;
    if ((byte & 0b11110000) == 0b11100000) return 3;
    if ((byte & 0b11111000) == 0b11110000) return 4;
    return 1;
}

size_t KeyRecommender::GetLength(const string &str){
    size_t len = 0;
    for(size_t idx = 0;idx < str.size();++idx){
        size_t nBytes = GetByteNum_UTF8(str[idx]);
        idx += (nBytes - 1);
        ++len;
    }
    return len;
}

inline
int triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int KeyRecommender::Distance(const string &lhs,const string &rhs)
{
    if(lhs == rhs) return 0; 
    else if(lhs.size() == 0) return rhs.size();
    else if(rhs.size() == 0) return lhs.size();
	size_t lhs_len = GetLength(lhs);
	size_t rhs_len = GetLength(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];
	for(size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}

	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
	string sublhs, subrhs;
    size_t dist_i = 1,dist_j = 1;
    size_t lhs_idx = 0,rhs_idx = 0;
	while(lhs_idx < lhs.size())
	{
		size_t nBytes = GetByteNum_UTF8(lhs[lhs_idx]);
		sublhs = lhs.substr(lhs_idx, nBytes);
		lhs_idx += nBytes;
        dist_j = 1;
        rhs_idx = 0;
		while(rhs_idx < rhs.size())
		{
			nBytes = GetByteNum_UTF8(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += nBytes;
			if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
			    editDist[dist_i][dist_j] = triple_min(
				    editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);
			}
            ++dist_j;
		}
        ++dist_i;
	}
	return editDist[lhs_len][rhs_len];
}

