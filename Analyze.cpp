#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <mecab.h>
#include "Analyze.h"

#define CHECK(eval) if (! eval) { \
   const char *e = tagger ? tagger->what() : MeCab::getTaggerError(); \
   std::cerr << "Exception:" << e << std::endl; \
   return -1; }


Analyze::Analyze():tagger(MeCab::createTagger("-d Mecab\\dic\\ipadic")){
    //CHECK(tagger);
}

Analyze::~Analyze(){
    delete tagger;
}

void Analyze::AnalyzeString(const std::string& input){
    BOSnode = tagger->parseToNode(input.c_str());
    //CHECK(BOSnode);
}

std::vector<std::string> Analyze::extractNoun()const{
    std::vector<std::string> res;
	for(const MeCab::Node * node = BOSnode; node; node=node->next){
		if(isNoun(node))res.push_back(std::string(node->surface).substr(0,node->length));
	}
	return res;
}

std::vector<std::string> Analyze::extractIndependent()const{
    std::vector<std::string> res;
	for(const MeCab::Node * node = BOSnode; node; node=node->next){
		if(isIndependent(node))res.push_back(std::string(node->surface).substr(0,node->length));
	}
	return res;
}

bool Analyze::isIndependent(const MeCab::Node * node)const{
	if((10<=node->posid&&node->posid<=12)||(31<=node->posid))return true;
	else return false;
}




bool Analyze::isNoun(const MeCab::Node * node)const{
	if(36<=node->posid&&node->posid<=67)return true;
	else return false;
}

#if 0
int main(int argc, char **argv) {
	std::string input;
	std::getline(std::cin, input);
	MeCab::Tagger* tagger=MeCab::createTagger("-d F:\\Programming\\lib\\Mecab\\dic\\ipadic");
	CHECK(tagger);

	const MeCab::Node* node = tagger->parseToNode(input.c_str());
	CHECK(node);

	for(; node; node=node->next){
		/*if(isIndependent(node->posid))std::cout<<"["<<node->posid<<"]";
		else std::cout<<std::string(node->surface).substr(0,node->length);*/
		if(isNoun(node->posid))std::cout<<std::string(node->surface).substr(0,node->length)<<std::endl;
/*		
    std::cout 
		<<std::string(node->surface).substr(0,node->length)
		//<< ' ' << node->feature
		//<< '\t' << (int)(node->surface - input.c_str())
	      //<< '\t' << (int)(node->surface - input.c_str() + node->length)
	      << '\t' << node->rcAttr	//âEï∂ñ¨ID
	      << '\t' << node->lcAttr	//ç∂ï∂ñ¨ID
	      << '\t' << node->posid	//ïiéåID
	      //<< '\t' << (int)node->char_type
	      //<< '\t' << (int)node->stat
	      //<< '\t' << (int)node->isbest
	      //<< '\t' << node->alpha
	      //<< '\t' << node->beta
	      //<< '\t' << node->prob
	      //<< '\t' << node->cost
		  << std::endl;*/
	}

	delete tagger;

	return 0;
}

#endif