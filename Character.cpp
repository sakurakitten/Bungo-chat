#include <map>
#include <vector>
#include <string>
#include <random>
#include <set>
#include <mecab.h>
#include <regex>
#include "MTable.h"
#include "Analyze.h"
#include "RuleBase.h"
#include "Character.h"

Character::Character(void){
}

Character::Character(const std::string& MTablePath, const std::string& RegexPath, const std::string& SerifPath):mtable(MTablePath), rulebase(RegexPath, SerifPath){
}

Character::~Character(void){
}





std::string Character::Respond(const std::string& input){
	std::string res;
	if(rulebase.nextIsMarkov()){
		res=mtable.makeSentenceRand();
	}
	if(!res.empty())return res;
	analyzer.AnalyzeString(input);
	std::vector<std::string> words=analyzer.extractIndependent();//活用形をもっと豊富にしたいよね
	/*if(words.empty())return rulebase.respondRuleBase(input,words);
	res = mtable.respondMarkov(words);
	return res.empty()?rulebase.respondRuleBase(input,words):res;*/
	return rulebase.respondRuleBase(input,words);
}

std::string Character::RespondF(const std::string& input){
	std::string res;
	if(rulebase.nextIsMarkov()){
		res=mtable.makeSentenceRand();
	}
	if(!res.empty())return res;
	analyzer.AnalyzeString(input);
	std::vector<std::string> words=analyzer.extractIndependent();//活用形をもっと豊富にしたいよね
	/*if(words.empty())return rulebase.respondRuleBase(input,words);
	res = mtable.respondMarkov(words);
	return res.empty()?rulebase.respondRuleBase(input,words):res;*/
	return rulebase.respondRuleBaseF(input,words);
}

std::string Character::ForceRespond(const std::string& input){
	std::string res;
	if(rulebase.nextIsMarkov()){
		res=mtable.makeSentenceRand();
	}
	if(!res.empty())return res;
	return rulebase.replySilence();
}
