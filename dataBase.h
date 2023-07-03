#ifndef LINUX_EMULATOR_DATABASE_H
#define LINUX_EMULATOR_DATABASE_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

namespace LinuxEmulator{

class DataBase {
public:
    DataBase(const std::vector<std::string>&, const std::vector<std::string>&);
    std::string getQuestion(const std::string& answer) const;
    std::string getAnswer(const std::string&) const;
    void printDataBase() const;
    std::map<std::string, std::string> getDataMap() const;
private:
    std::map<std::string, std::string> dataMap;
};


DataBase::DataBase(const std::vector<std::string>& questions, const std::vector<std::string>& answers) {
    if (questions.size() != answers.size()) {
        std::cerr << "Error: Questions and Answers vectors must have the same size." << std::endl;
        return;
    }

    for (size_t i = 0; i < questions.size(); ++i) {
        dataMap[questions[i]] = answers[i];
    }
}

std::string DataBase::getQuestion(const std::string& answer) const {
    for (const auto& pair : dataMap) {
        if (pair.second == answer) {
            return pair.first;
        }
    }
    return "Unknown Question";
}

std::string DataBase::getAnswer(const std::string& question) const {
    auto it = dataMap.find(question);
    if (it != dataMap.end()) {
        return it->second;
    }
    return "Unknown Answer";
}

void DataBase::printDataBase() const {
    for (const auto& pair : dataMap) {
        std::cout << "Question: " << pair.first << ", Answer: " << pair.second << std::endl;
    }
}

std::map<std::string, std::string> DataBase::getDataMap() const {
	return dataMap;
}


}   //namespace LinuxEmulator

#endif   //DATABASE_H