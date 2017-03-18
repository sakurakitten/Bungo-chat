#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <random>
#include <regex>
#include "RuleBase.h"

void widen(const std::string &src, std::wstring &dest) {
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs(wcs, src.c_str(), src.length() + 1);
	dest = wcs;
	delete [] wcs;
}

void narrow(const std::wstring &src, std::string &dest) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	dest = mbs;
	delete [] mbs;
}


RuleBase::RuleBase(void)
{
}

RuleBase::RuleBase(const std::string &regexpath, const std::string& serifpath):serifs(52)
{
	prevword[0]=prevword[1]=-1;
	LoadRegex(regexpath);
	LoadSerif(serifpath);
	
}

bool RuleBase::nextIsMarkov(){
	if(prevword[0]=='j'-'a'){
		prevword[0]=-1;
		prevword[1]=-1;
		return true;
	}else return false;
}


inline int sigtonum(char c){
	return islower(c)?c-'a':26+c-'A';
}

void RuleBase::LoadRegex(const std::string& regexpath){
	std::ifstream ifs(regexpath.c_str());
	std::string line;
	std::string sig, type;
	std::wstring wline,regsrc;
	std::tr1::wregex re(L"^(.+)\\t(.+)\\t(.+)$");
	std::tr1::wsmatch matches;
	while(/*ifs>>sig>>type,ifs.ignore(),*/std::getline(ifs,line)){
		if(line.empty())continue;
		std::wstring wline;
		widen(line,wline);
		if(!std::tr1::regex_match(wline,matches,re))continue;
		narrow(matches.str(1),sig);
		narrow(matches.str(2),type);
		regsrc=matches.str(3);
		regexset tmp={sigtonum(sig[0]), type[0], std::tr1::wregex(regsrc)};
		this->re.push_back(tmp); 
    }
}


void RuleBase::LoadSerif(const std::string& serifpath){
	std::ifstream ifs(serifpath.c_str());
	std::string sig, serif,line;
	std::wstring wline;
	std::tr1::wregex re(L"^(.*)\\t(.+)$");
	std::tr1::wsmatch matches;
	while(/*ifs>>sig,ifs.ignore(),*/std::getline(ifs,line)){
		widen(line,wline);
		std::tr1::regex_match(wline, matches, re);
		narrow(matches.str(1), sig);
		narrow(matches.str(2), serif);
		for(std::string::iterator it=sig.begin();it!=sig.end(); ++it){
			serifs[sigtonum(*it)].push_back(serif);
		}
    }
}

RuleBase::~RuleBase(void)
{
}

std::string RuleBase::respondRuleBase(const std::string& input, const std::vector<std::string>& words){
	/*return randomChoice();*/
	std::string res=replyByWord(input,words);
	if(!res.empty())return res;
	res=randomChoice();
	if((rand()%6==0)&&!res.empty())return res;//確率変更
	prevword[0]=-1;
	prevword[1]=-1;
	return "";
}
std::string RuleBase::respondRuleBaseF(const std::string& input, const std::vector<std::string>& words){
	/*return randomChoice();*/
	std::string res=replyByWord(input,words);
	if(!res.empty())return res;
	res=randomChoice();
	if(!res.empty())return res;
	prevword[0]=-1;
	prevword[1]=-1;
	return "";
}

std::string RuleBase::randomChoice(){
	if(serifs[0].empty())return "";
	int t = rand()%serifs[0].size();
	if(prevword[0]==0&&prevword[1]==t)return "";
		prevword[0]=0;
		prevword[1]=t;
	return serifs[0][t];
}



std::wstring NumtoCHNwstr(int i){
	//60未満
	std::wstring res;
	switch(i/10){
		case 5:
			res+=L"五十";
			break;
		case 4:
			res+=L"四十";
			break;
		case 3:
			res+=L"三十";
			break;
		case 2:
			res+=L"二十";
			break;
		case 1:
			res+=L"十";
			break;
	}
	switch(i%10){
		case 9:
			res+=L"九";
			break;
		case 8:
			res+=L"八";
			break;
		case 7:
			res+=L"七";
			break;
		case 6:
			res+=L"六";
			break;
		case 5:
			res+=L"五";
			break;
		case 4:
			res+=L"四";
			break;
		case 3:
			res+=L"三";
			break;
		case 2:
			res+=L"二";
			break;
		case 1:
			res+=L"一";
			break;
	}
	if(res.empty())res=L"零";
	return res;
}

std::string RuleBase::replyQuery(long long querybit, std::wstring whentype){
	std::tr1::wregex tre(L"^(.*)\\[when\\](.*)$");

	for(int i = 2; i<9; ++i){
		if((querybit&(long long(1)<<i))&&!serifs[i].empty()){
		
			int len=serifs[i].size();
			int limit=int(RAND_MAX*atan(float(len)/20)/acos(0.0));
			if(rand()<limit){
				if(i==4){
					int serifidx=rand()%len;
					if(prevword[0]==i&&prevword[1]==serifidx)return "";
					prevword[0]=i;
					prevword[1]=serifidx;
					std::string serif=serifs[i][serifidx];
					std::string::size_type pos=serif.find("[timeunit]");
					if(pos!=std::string::npos&&!whentype.empty()){
						std::string swhentype;
						narrow(whentype.substr(0,swhentype.size()-1), swhentype);
						return serif.replace(pos, strlen("[timeunit]"),swhentype);
					}
					std::wstring wserif;
					widen(serif,wserif);
					if(std::tr1::regex_match(wserif, tre)){
						//[when]の置換
						if(whentype.find(L"時")!=std::wstring::npos){
							int time=(rand()%36)%24;
							std::wstring wstrtime=NumtoCHNwstr(time);
							std::wstring wres = std::tr1::regex_replace(wserif, tre, L"$1"+wstrtime+whentype+((rand()%16)?L"半":L"")+L"$2");
							std::string sres;
							narrow(wres,sres);
							return sres;
						}
						if(whentype.find(L"分")!=std::wstring::npos||whentype.find(L"秒")!=std::wstring::npos){
							int time=rand()%60;
							std::wstring wstrtime=NumtoCHNwstr(time);
							std::wstring wres = std::tr1::regex_replace(wserif, tre, L"$1"+wstrtime+whentype+L"$2");
							std::string sres;
							narrow(wres,sres);
							return sres;
						}
						if(whentype.find(L"日")!=std::wstring::npos){
							int time=rand()%31;
							std::wstring wstrtime=NumtoCHNwstr(time);
							std::wstring wres = std::tr1::regex_replace(wserif, tre, L"$1"+wstrtime+whentype+L"$2");
							std::string sres;
							narrow(wres,sres);
							return sres;
						}
						if(whentype.find(L"月")!=std::wstring::npos){
							int time=rand()%12;
							std::wstring wstrtime=NumtoCHNwstr(time);
							std::wstring wres = std::tr1::regex_replace(wserif, tre, L"$1"+wstrtime+whentype+L"$2");
							std::string sres;
							narrow(wres,sres);
							return sres;
						}
					}else{
						return serifs[i][serifidx];
					}
				}else{
					return serifs[i][rand()%len];
				}
			}
		}
	}
	if(!serifs[1].empty()){
		int len=serifs[1].size();
		int t=rand()%len;
		if(prevword[0]==1&&prevword[1]==t)return "";
		prevword[0]=1;
		prevword[1]=t;
		if(!(rand()%6))return serifs[1][t];
	}
	return "";
}

std::string RuleBase::replySilence(){
	while(true){
		for(int i = 9; i<12; i++){
			if(!serifs[i].empty()){
				//無言
				int len=serifs[i].size();
				int limit=int(RAND_MAX*atan(float(len)/20)/acos(0.0));
				if(rand()<limit||len!=0){
					int t=rand()%len;
					if(prevword[0]==i&&prevword[1]==t)continue;
					prevword[0]=i;
					prevword[1]=t;
					return serifs[i][t];
				}
			}
		}
		if(rand()%20==0){
			std::string res=randomChoice();
			if(!res.empty())return res;
		}
	}
}

std::string RuleBase::replyByWord(const std::string &input, const std::vector<std::string> &words){
	

	//std::regex re("(.+)(とは|って)(何)?(だ|です)?(か)?(//?|？)?");*/
	//std::tr1::wregex re(L"((あなた|貴方|アナタ|君|きみ|キミ|お前|おまえ|てめえ|てめー|テメー|手前|おめえ|貴様|きさま|あんた|アンタ)(の)?)?(お|御)?(名前|なまえ|名)(((は|って)?(何(なん)?(だ|((です|でしょう|である|であります|でありましょう)(か?)))?)?)|((を)?(教え|おしえ)((て|ろ)(ください|くれ)?(さ|ね|よ|よね)?|よ)))(\\?|？|。)?");
	std::wstring winput;
	widen(input,winput);
	/*if(std::tr1::regex_match(winput,re)){
		return"名前はまだ無い。";
	}*/
	long long int res=0;
	std::wstring whentype;
	for(std::vector<regexset>::iterator it=re.begin(); it!=re.end(); ++it){
		if(it->type=='m'){
			res|=int(std::tr1::regex_match(winput,it->re))<<it->signum;
		}
		if(it->type=='s'){
			res|=int(std::tr1::regex_search(winput,it->re))<<it->signum;
		}
		if(it->type=='t'){
			std::tr1::wsmatch matches;
			res|=int(std::tr1::regex_search(winput,matches,it->re))<<it->signum;
			whentype=matches.str(1);
			if(whentype.empty()||whentype==L"いつ")whentype=L"";
			else whentype = whentype.substr(1);
		}
	}
	/*if(res&(1<<('i'-'a'))){
		
	}*/
	
	if(res&0x1fc){
	//質問された
		std::string sres=replyQuery(res, whentype);
		if(!sres.empty())return sres;
	}

	if(res&(1<<11)){
		return replySilence();
	}

	for(int i=26; i<52; i++){
		if((res&(long long(1)<<i))&&!serifs[i].empty()){
			int len=serifs[i].size();
			int t=rand()%len;
			if(prevword[0]==i&&prevword[1]==t)return "";

			prevword[0]=i;
			prevword[1]=t;
			return serifs[i][t];
		}
	}

	return "";
}