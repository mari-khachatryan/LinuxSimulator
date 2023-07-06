#ifndef LINUX_EMULATOR_DISPLAY_H
#define LINUX_EMULATOR_DISPLAY_H

#include "systemManagment.h"
#include "parsing.h"
#include "user.h"
#include "question.h"
#include "answer.h"
#include "dataBase.h"
#include "gTree.h"
#include <map>
#include <random>
#include <cstdio>
#include <limits>

namespace LinuxEmulator{

class Display {
public:
	Display();
	void display();
	std::map<std::string, std::string> getRandomPairs(const std::map<std::string, std::string>&, int);
	std::vector<std::string> split(const std::string&, char);
	void printColoredText(const std::string& text, int colorCode);
	std::string takePoint(const std::string& str);
	void hideOutput();
private:
	GeneralTree gtree;
	GeneralTree gtree1;
	GeneralTree gtree2;
    SystemManagement systemManagement;
	SystemManagement smMy;
	SystemManagement smOther;
	int points;
	std::map<std::string, std::vector<std::string>> finalView;
};

Display::Display() : systemManagement(gtree), smMy(gtree1), smOther(gtree2) 
{
	points = 0; 
}

void Display::display() {
	std::cout << "               This Linux simulator was created by Mariam Khachatryan,\n"
	             "                      a student of Picsart Academy's Wave3 group \n";
	std::cout << "                              Please choose the workplace. " << std::endl;
	std::string numStr;
	std::string message = "1. Help";
    int color = 31; // Red color code

    printColoredText(message, color);
    std::cout << std::endl;
	std::cout << "2. CommandLine" << std::endl;
	std::cout << "3. Exam portal" << std::endl;
	std::cout << "Print 'exit' for log out" << std::endl;
	while(true){
		std::cout << "> ";
		std::cin >> numStr;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if(numStr == "exit") {
			break;
		} else if(numStr.empty()) {
            std::cout << "> ";
            continue;
		} else if(numStr == "1") {
			std::cout << "There are 2 mods. 2-CommandLine mod is created to get familiar with the system.\n" 
						  "There are no wrong or right answers when using it. If you think you are ready for\n"
						  "testing, choose 3-Exam portal. It does not differ from -2- From the CommandLine,\n"
						  "but a point will be calculated for each action. The terminal will prompt you with questions.\n"
						  "       2. CommandLine\n" 
						  "       3. Exam portal\n";
						  while(numStr != "1" && numStr != "2") {
						  	std::cout << "> ";
						  	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						  }
			continue;
		} else if(numStr == "2") {
			std::string username;
	    	std::cout << "Input username: ";
	    	std::getline(std::cin, username);
	    	std::string password;
	    	std::cout << "Input password: ";
	    	std::getline(std::cin, password);
	    	User user(username, password);
			while(true) {
					std::string command;
					printColoredText(username, 32);
        			printColoredText("@hostname> ", 32);
					std::getline(std::cin, command);
					if(command == "exit") {
						break;
					} else if (command.empty()) {
						continue;
					} else {
						Parsing com(command);
						systemManagement.setPars(com);
						if(!systemManagement.getIsValid()) {
							std::cout << "Sory but command not valid!" << std::endl;
							continue;
						} else {
							systemManagement.commandExecute();
						}
					}
				}
			
		} else if (numStr == "3"){
			GeneralTree myGtree;
			GeneralTree OtherGtree;
			std::cout << "You have selected examMode, you are sure you want to proceed without testing the commands "<< std::endl;
			char ch;
			std::cout << "Are you sure y/n" << std::endl;
			std::cin >> ch;
			if(ch == 'n') {
				continue;
			}
			std::cout << "!! If you want to log out from examMod please enter 'exit' !!" << std::endl;
			Question q;
			Answer a;
			std::vector<std::string> questions = q.getQuestions();
			std::vector<std::string> answers = a.getAnswers();
			DataBase db(questions, answers);
			const int questionsNum = 10;
			std::map<std::string, std::string> randmap = getRandomPairs(db.getDataMap(), questionsNum);
			int i = 0;
			for (const auto& pair : randmap) {
				std::string question = pair.first; 
		        std::string answer = pair.second;
				++i;
				question = takePoint(question);
				answer = takePoint(answer);
	
				std::cout << i << ". " << question << std::endl;
				std::vector<std::string> trueAnswers = split(answer, ',');
				while(true) {
					bool flagTrue = false;
					std::string command;
					std::cout << "ExamPortal > ";
					std::getline(std::cin, command);
					if(command == "exit") {
						break;
					} else if (command.empty()) {
						continue;
					} else {
						for(const auto ans : trueAnswers) {
							if(command == ans) {
								flagTrue = true;
							}
						}

						Parsing comMy(trueAnswers[0]);
						smMy.setPars(comMy);
						if(flagTrue) {
							Parsing comOther(command);
							smOther.setPars(comOther);
						}

						if(smMy.getIsValid()) {
							hideOutput();
							smMy.commandExecute();
						} 
						if(smOther.getIsValid()) {
							hideOutput();
							smOther.commandExecute();
						}
						if(!(gtree1 == gtree2)) {
							flagTrue = false;
						}
						std::vector<std::string> finalAns;
						finalAns.push_back(command);
						finalAns.push_back(trueAnswers[0]);
						finalAns.push_back(std::to_string(flagTrue));  
						finalView[question] = finalAns;

						if(flagTrue) {
							points += 10;
						}
					}
					break;
				}
				if(i == 10) break;
			}
				std::cout << "YOU FINISHED YOUR EXAMINE !!" << std::endl;
				if(points >= 90) {
					std::cout << "Congratulations you passed the exam!" << std::endl;
				} else {
					std::cout << "Sorry, but you failed exam!" << std::endl;
				}
				std::cout << "Your total points is: " << points << std::endl;

				for (const auto& pair : finalView) {
			        std::cout << pair.first << std::endl;
			        std::vector<std::string> v = pair.second;
			        if(v[2] == "true") {
			        	std::string message = pair.second[0];
			        	int color = 32;
			        	printColoredText(message, color);
			        	std::string message2 = pair.second[1];
			        	color = 32;
			        	message2 = "     - " + message2;
 			        	printColoredText(message2, color);
			        	std::cout << std::endl;
			        } else {
			        	std::string message = pair.second[0];
			        	int color = 31;   //red
			        	printColoredText(message, color);
			        	std::string message2 = pair.second[1];
			        	color = 32;
			        	message2 = "     - " + message2;
 			        	printColoredText(message2, color);
			        	std::cout << std::endl;
			        }
			    }
		} else if(numStr == "") {
			std::cout << "> " << std::endl;
			std::cin >> numStr;
		} else {
			std::cout << "the selected command is not correct" << std::endl;
		}
	}
}

	

std::map<std::string, std::string> Display::getRandomPairs(const std::map<std::string, std::string>& inputMap, int num) {
    std::map<std::string, std::string> randomPairs;
    std::vector<std::pair<std::string, std::string>> mapPairs(inputMap.begin(), inputMap.end());
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(mapPairs.begin(), mapPairs.end(), generator);
    for (int i = 0; i < num && i < mapPairs.size(); ++i) {
        randomPairs.insert(mapPairs[i]);
    }
    
    return randomPairs;
}

std::vector<std::string> Display::split(const std::string& str, char ch) {
	std::vector<std::string> newV;
	std::stringstream ss(str);
	std::string part;
	while(std::getline(ss, part, ch)) {
		newV.push_back(part);
	}
	return newV;
}

void  Display::printColoredText(const std::string& text, int colorCode) {
    std::cout << "\033[" << colorCode << "m" << text << "\033[0m";
    // Red: 31
	// Green: 32
	// Yellow: 33
	// Blue: 34
	// Magenta: 35
	// Cyan: 36
	// White: 37
}

std::string Display::takePoint(const std::string& str) {
	size_t posOfPoint = str.find('.');
	std::string s = "";
    if(posOfPoint != std::string::npos) {
		s = str.substr(posOfPoint + 2);
	}
	return s;
}

void Display::hideOutput() {
    std::streambuf* originalOutputBuffer = std::cout.rdbuf();
    std::ofstream nullDevice("/dev/null");
    std::cout.rdbuf(nullDevice.rdbuf());

    std::cout.rdbuf(originalOutputBuffer);

}


}  //namespace LinuxEmulator


#endif //DISPLAY_H
