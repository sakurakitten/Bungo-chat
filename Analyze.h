#pragma once
/*requires:
#include <mecab.h>
#include <string>
#include <vector>
*/

class Analyze{
private:
	bool isNoun(const MeCab::Node * node)const;
	bool isIndependent(const MeCab::Node * node)const;
    
    std::string str;
    const MeCab::Node *BOSnode;
    MeCab::Tagger* tagger;
    
public:
    Analyze();
    ~Analyze();
    void AnalyzeString(const std::string& input);
    std::vector<std::string> extractNoun()const;
    std::vector<std::string> extractIndependent()const;

    
};