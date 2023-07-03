#ifndef LINUX_EMULATOR_ANSWER_H
#define LINUX_EMULATOR_ANSWER_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace LinuxEmulator{

class Answer {
public:
	Answer();
	std::vector<std::string> getAnswers() const;
	void setAnswers(const std::vector<std::string>&);
private:
	std::vector<std::string> answers;
};


Answer::Answer() 
{
	std::string trueAnswer;
	std::ifstream answerFile("answers1.txt");
    if(answerFile.is_open()) {
    	std::string str;
    	while(std::getline(answerFile, str)) {
    		answers.push_back(str);
    	}
    	answerFile.close();
    } else {
    	std::cout << "Failed to open the question file." << std::endl;
    }
}

std::vector<std::string> Answer::getAnswers() const {
	return answers;
}

void Answer::setAnswers(const std::vector<std::string>& a) {
	answers = a;
}

}  // namespace LinuxEmulator

#endif //LINUX_EMULATOR_ANSWER_H