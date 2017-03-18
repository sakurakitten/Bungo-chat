/*requires:
#include <map>
#include <string>
#include <vector>
*/

class MTable{
private:
    std::map<std::pair<std::string,std::string>,std::vector<std::string>> forwardTable;
    std::map<std::pair<std::string,std::string>,std::vector<std::string>> reverseTable;
    std::map<std::string,std::vector<std::pair<std::string,std::string>>> middleTable;
    std::vector<std::pair<std::string,std::string>> beginnings;
    
    

    
    //void loadBeginnings(const std::string& line);
    void loadTableRow(const std::string& line);
    
    std::string extendSentence(const std::string& s1, const std::string& s2)const;
    std::string extendSentenceBack(const std::string& s1, const std::string& s2)const;
    
public:
    MTable();
    MTable(const std::string& input);
    ~MTable();
    std::string startSentence(const std::string& str)const;
    std::string makeSentenceRand()const;
    std::string makeSentence(const std::string& str)const;
	std::string respondMarkov(const std::vector<std::string>& words)const;
};