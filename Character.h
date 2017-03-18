#pragma once
/*requires:
#include <map>
#include <vector>
#include <string>
#include "MTable.h"
#include "RuleBase.h"
*/

class Character{
private:
    MTable mtable;
	Analyze analyzer;
	RuleBase rulebase;
public:
    Character();
	~Character();
	Character(const std::string& MTablePath, const std::string& RegexPath, const std::string& SerifPath);
    std::string Respond(const std::string& input);
	std::string RespondF(const std::string& input);
	std::string ForceRespond(const std::string& input);
};