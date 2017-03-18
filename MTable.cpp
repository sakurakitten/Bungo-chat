#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <fstream>
#include <set>
#include <cstdlib>
#include "MTable.h"
#include "Main.h"

MTable::MTable(){}

MTable::~MTable(){}

MTable::MTable(const std::string& input){
    std::string line;
#if 1
	std::ifstream ifs(input.c_str());
    //std::getline(ifs,line);
    //loadBeginnings(line);
    while(std::getline(ifs,line)){
        loadTableRow(line);
    }
#else
    std::getline(std::cin,line);
    //loadBeginnings(line);
    while(std::getline(std::cin,line)){
        loadTableRow(line);
    }
#endif
}

#if 0
void MTable::loadBeginnings(const std::string& line){
    std::stringstream sline;
    sline<<line;
    std::string s1,s2;
    while(sline>>s1&&sline>>s2){
        beginnings.push_back(std::make_pair(std::move(s1),std::move(s2)));
    }
}
#endif

void MTable::loadTableRow(const std::string& line){
    std::stringstream sline;
    sline<<line;
    std::string s0,s1,s2;
    sline>>s0>>s1;
    //std::vector<std::string> row;
    while(sline>>s2){
        //row.push_back(std::move(s2));
        forwardTable[std::make_pair(s0,s1)].push_back(s2);
        middleTable[s1].push_back(std::make_pair(s0,s2));
        reverseTable[std::make_pair(s1,s2)].push_back(s0);
        if(s0=="。"){
            beginnings.push_back(std::make_pair(s1,s2));
        }
    }
    //forward[std::make_pair(std::move(s0),std::move(s1))]=std::move(row);
}

std::string MTable::respondMarkov(const std::vector<std::string>& words)const{
	std::string res;
	std::set<int> checked;
	while(true){
		int next = rand()%words.size();
		if (checked.count(next))continue;	//既に調べた単語
		res = makeSentence(words[next]);
		if (!res.empty()) {
			//文生成成功
			break;
		}
		else {
			//nextの単語では文を続けられない
			checked.insert(next);
			if (checked.size() == words.size()) return "";	//与えられた単語での文生成不可能
		}
	}
	return res;
}

std::string MTable::extendSentence(const std::string& s1, const std::string& s2)const{
    if(s2=="。")return s1;	//文の終わり
	std::pair<std::string,std::string> p=std::make_pair(s1,s2);
    if(forwardTable.find(p)==forwardTable.end())return "";	//2引数に続く単語は存在しない
	std::string res;
	std::set<int> checked;
    //std::uniform_int_distribution<int> dist(0, forwardTable.at(p).size()-1);
	while (true){
		int next = /*dist(mtRand)*/rand()%forwardTable.find(p)->second.size();
		if (checked.count(next))continue;	//既に調べた単語
		res = extendSentence(s2, forwardTable.find(p)->second[next]);
		if (!res.empty()) {
			//文生成成功
			break;
		}
		else {
			//nextの単語では文を続けられない
			checked.insert(next);
			if (checked.size() == forwardTable.find(p)->second.size()) return "";	//与えられた単語での文生成不可能
		}
	}
    return s1+res;
}

std::string MTable::extendSentenceBack(const std::string& s1, const std::string& s2)const{
    //std::cout<<"extendSentenceBack()"<<std::endl;
    //std::cout<<s1<<" "<<s2<<std::endl;
    if(s1=="。")return s2;
    std::pair<std::string,std::string> p=std::make_pair(s1,s2);
    if(reverseTable.find(p)==reverseTable.end())return "";
	std::string res;
	std::set<int> checked;
    //std::uniform_int_distribution<int> dist(0, reverseTable.at(p).size()-1);
	while (true){
		int next = /*dist(mtRand)*/rand()%reverseTable.find(p)->second.size();
		if (checked.count(next))continue;	//既に調べた単語
		res = extendSentence(s2, reverseTable.find(p)->second[next]);
		if (!res.empty()) {
			//文生成成功
			break;
		}
		else {
			//nextの単語では文を続けられない
			checked.insert(next);
			if (checked.size() == reverseTable.find(p)->second.size()) return "";	//与えられた単語での文生成不可能
		}
	}
    return res+s2;
}

std::string MTable::makeSentenceRand()const{
	std::string res;
	std::set<int> checked;
    //std::uniform_int_distribution<int> dist(0, beginnings.size()-1);
	while(true){
		int next = /*dist(mtRand)*/rand()%beginnings.size();
		if (checked.count(next))continue;	//既に調べた単語
		std::pair<std::string,std::string> p=beginnings[next];	//直した!!!!!!
		res = extendSentence(p.first, p.second);
		if (!res.empty()) {
			//文生成成功

			break;
		}
		else {
			//nextの単語では文を続けられない
			checked.insert(next);
			if (checked.size() == forwardTable.find(p)->second.size()) return "";	//与えられた単語での文生成不可能
		}
	}
   return res;
}

std::string MTable::startSentence(const std::string& str)const{
	std::pair<std::string,std::string> p=std::make_pair("。",str);
    if(forwardTable.find(p)==forwardTable.end())return "";	//与えられた単語から文を始めることはできない
 	std::string res;
	std::set<int> checked;
	//std::uniform_int_distribution<int> dist(0, forwardTable.at(p).size() - 1);
	while(true){
		int next = /*dist(mtRand)*/rand()%forwardTable.find(p)->second.size();
		if (checked.count(next))continue;	//既に調べた単語
		res = extendSentence(str, forwardTable.find(p)->second[next]);
		if (!res.empty()) {
			//文生成成功
			break;
		}
		else {
			//nextの単語では文を続けられない
			checked.insert(next);
			if (checked.size() == forwardTable.find(p)->second.size()) return "";	//与えられた単語での文生成不可能
		}
	}
    return res;
}

std::string MTable::makeSentence(const std::string& str)const{
	if (middleTable.find(str) == middleTable.end())return "";
 	std::string res1;
	std::string res2;
	std::set<int> checked1;
    //std::uniform_int_distribution<int> dist1(0, middleTable.at(str).size()-1);
	while(true){
		int next1 = /*dist1(mtRand)*/rand()%middleTable.find(str)->second.size();
		if (checked1.count(next1))continue;	//既に調べた単語のペア
		std::pair<std::string,std::string> p=middleTable.find(str)->second[next1];
		std::string s0 = p.first;
		std::string s2 = p.second;
		std::pair<std::string, std::string> p1 = std::make_pair(s0, str);
		if (forwardTable.find(std::make_pair(str, s2)) == forwardTable.end()) {
			//strの次に来る単語はない
			checked1.insert(next1);
			if (checked1.size() == middleTable.find(str)->second.size()) return "";	//与えられた単語での文生成不可能
		};
		res1 = extendSentenceBack(p1.first,str);
		if (!res1.empty()) {
			//strより前の文生成成功
			std::set<int> checked2;
			//std::uniform_int_distribution<int> dist2(0, forwardTable.at(p1).size()-1);
			while (true) {
				std::pair<std::string,std::string> p2=std::make_pair(str, s2);
				int next2 = /*dist2(mtRand)*/rand()%forwardTable.find(p2)->second.size();
				if (checked2.count(next2))continue;
				std::string s3 = forwardTable.find(p2)->second[next2];
				std::pair<std::string,std::string> p3=std::make_pair(s2, s3);
				res2 = extendSentence(s2, s3);
				if (!res2.empty()) {
					//strより後の文生成成功
					break;
				}
				else {
					checked2.insert(next2);
					if (checked2.size() == forwardTable.find(p2)->second.size()) break;	//str,s2の組み合わせ(=next1)が絶望的
					//一つ目のwhileループ内でcontinueしたい

				}
			}
			if (res2.empty()) {
				//next1が絶望的
				checked1.insert(next1);
				if (checked1.size() == middleTable.find(str)->second.size()) return "";	//与えられた単語での文生成不可能
			}else{
				//文生成成功
				return res1+res2;
			}
		}
		else {
			//next1の単語では文を続けられない
			checked1.insert(next1);
			if (checked1.size() == middleTable.find(str)->second.size()) return "";	//与えられた単語での文生成不可能
		}
	}
//    std::pair<std::string,std::string> p=middleTable.at(str)[dist(mtRand)];
    //std::uniform_int_distribution<int> dist2(0, forwardTable.at(std::make_pair(str, p.second)).size()-1);
    //std::string s2=forwardTable.at(std::make_pair(str, p.second))[dist2(mtRand)];
    //std::cout<<"makeSentence()"<<std::endl;
	//extendSentenceBack(p.first,str)+extendSentence(p.second, s2)
}