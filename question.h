#ifndef LINUX_EMULATOR_QUESTION_H
#define LINUX_EMULATOR_QUESTION_H

#include <iostream> 
#include <string>
#include <fstream> 
#include <vector>


namespace LinuxEmulator{

class Question {
public:
	Question();
	std::vector<std::string> getQuestions() const;
	void setQuestions(const std::vector<std::string>&);
private:
	std::vector<std::string> questions;
};

Question::Question() 
{
	std::ifstream questionFile("questions1.txt"); 
    if(questionFile.is_open()) {
    	std::string str;
    	while(std::getline(questionFile, str)) {
    			questions.push_back(str);
    		}
    	questionFile.close();
    } else {
    	std::cout << "Failed to open the question file." << std::endl;
    }
}

std::vector<std::string> Question::getQuestions() const {
	return questions;
}

void Question::setQuestions(const std::vector<std::string>& q) {
	questions = q;
}

}  //namespace LinuxEmulator

#endif //LINUX_EMULATOR_QUESTION_H
