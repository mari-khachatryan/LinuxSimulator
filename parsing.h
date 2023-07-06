#ifndef LINUX_EMULATOR_COMMAND_H
#define LINUX_EMULATOR_COMMAND_H

#include <string>
#include <vector>
#include <sstream>
#include <map>

namespace LinuxEmulator{


class Parsing {
public:
	Parsing();
	Parsing(const std::string&);
	std::string getAllCommand() const;
	std::string getCommand() const;
	std::vector<std::string> getArguments() const;
	std::map<std::string, std::vector<std::string>> getOptions() const;

	void setAllCommand(const std::string& c);
	std::vector<std::string> split(const std::string&, char);
private:
	std::string allCommond;
	std::string commond;
	std::vector<std::string> arguments;
	std::map<std::string, std::vector<std::string>> options;
};

Parsing::Parsing() = default;

Parsing::Parsing(const std::string& a) : allCommond{a} {
	std::vector<std::string> newV = split(a, ' ');
	commond = newV[0];
	for(int i = 1; i < newV.size(); ++i) {
		if(newV[i][0] == '-') {
			std::string optionName = newV[i];
			std::vector<std::string> optionArgs;
			while(++i < newV.size() && newV[i][0] != '-' && newV[i][0] != '/') {
				optionArgs.push_back(newV[i]);
			}
			--i;

			options[optionName] = optionArgs;
		} else {
			arguments.push_back(newV[i]);
		}
	}
}

std::string Parsing::getAllCommand() const {
	return allCommond;
}

std::string Parsing::getCommand() const {
	return commond;
}

std::vector<std::string> Parsing::getArguments() const {
	return arguments;
}

std::map<std::string, std::vector<std::string>> Parsing::getOptions() const {
	return options;
}

void Parsing::setAllCommand(const std::string& c) {
	allCommond = c;
}

std::vector<std::string> Parsing::split(const std::string& str, char ch) {
	std::vector<std::string> newV;
	std::stringstream ss(str);
	std::string part;
	while(std::getline(ss, part, ch)) {
		newV.push_back(part);
	}
	return newV;
}

}  //namespace LinuxEmulator


#endif    //LINUX_EMULATOR_COMMAND_H
