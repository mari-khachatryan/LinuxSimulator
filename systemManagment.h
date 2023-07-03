#ifndef LINUX_EMULATOR_SYSTEMMANAGEMENT_H
#define LINUX_EMULATOR_SYSTEMMANAGEMENT_H

#include "gTree.h"
#include "file.h"
#include "parsing.h"
#include "commandValidator.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sys/statvfs.h>
#include <fstream>

namespace LinuxEmulator {

class SystemManagement {
public:
    SystemManagement();
    SystemManagement(const std::string& command, GeneralTree& gt);
    Parsing getPars() const;
    bool getIsValid() const;
    void setPars(const Parsing&);
    void setIsValid(bool);

    bool createDirectory(const std::string& name);
    void removeDirectory(const std::string& name);
    bool directoryExists(const std::string& name) const;
    void removeFile(const std::string& name);
    bool fileExists(const std::string& name) const;

    std::string getCurrentDirectory() const;
    GeneralTree& getGTree();
    void setGTree(const GeneralTree&);
    void setCurrentDirectory(const std::string&);
    Node* findNode(Node* currentNode, const File& targetValue) const;
    Node* findNode(const File& targetValue);
    void addChild(Node* parentNode, Node* childNode);
    void commandExecute();

    void ls();
    void listDirectory(const std::string&, std::ostream&);
    void listDirectory(const std::string&);
    void date();
    void cal();
    void df();
    void free();
    void help();
    void cd(const std::string&);
    void createFile(const std::string&);
    Node* dfsFindNode(Node* currentNode, const File& targetDir);
    void pwd();
    void mkdir(const std::string&);

private:
    std::string currentDirectory;
    std::string previousDirectory;
    GeneralTree gTree;
    Parsing pars;
    bool isValid;
};

SystemManagement::SystemManagement() : currentDirectory("/"), gTree() {}

SystemManagement::SystemManagement(const std::string& command, GeneralTree& gt)
    : currentDirectory("/")
    , gTree(gt)
    , pars(command)
{
    CommandValidator comValidator(pars);
    isValid = comValidator.isValidCommand(pars);
    File rootDir("/", true);
    Node* rootNode = new Node(rootDir);
    rootNode->parent = nullptr;
    gTree.setRootData(rootDir);
    gTree.setRoot(rootNode);
    currentDirectory = "/";
    previousDirectory = "/";
}

Parsing SystemManagement::getPars() const {
    return pars;
}

bool SystemManagement::getIsValid() const {
    return isValid;
}

void SystemManagement::setPars(const Parsing& p) {
    pars = p;
}

void SystemManagement::setIsValid(bool is) {
    isValid = is;
}

bool SystemManagement::createDirectory(const std::string& name) {
    File newDirectory(name, true);
    File parentDirectory(currentDirectory, true);

    if (gTree.insert(newDirectory, parentDirectory)) {
        return true; // Directory created successfully
    } else {
        return false; // Failed to create directory
    }
}

void SystemManagement::removeDirectory(const std::string& name) {
    File directory(name, true);
    gTree.remove(directory);
}

bool SystemManagement::directoryExists(const std::string& name) const {
    File directory(name, true);
    Node* node = findNode(gTree.getRoot(), directory);
    return (node != nullptr);
}

void SystemManagement::removeFile(const std::string& name) {
    File file(name, false);
    gTree.remove(file);
}

bool SystemManagement::fileExists(const std::string& name) const {
    File file(name, false);
    Node* node = findNode(gTree.getRoot(), file);
    return (node != nullptr);
}

std::string SystemManagement::getCurrentDirectory() const {
    return currentDirectory;
}

GeneralTree& SystemManagement::getGTree() {
    return gTree;
}

void SystemManagement::setGTree(const GeneralTree& tree) {
    gTree = tree;
}

void SystemManagement::setCurrentDirectory(const std::string& dir) {
    previousDirectory = currentDirectory;
    currentDirectory = dir;
}

Node* SystemManagement::findNode(Node* currentNode, const File& targetValue) const {
    if (currentNode == nullptr)
        return nullptr;
    if (currentNode->getData() == targetValue)
        return currentNode;
    for (int i = 0; i < currentNode->getChildCount(); i++) {
        Node* resultNode = findNode(currentNode->getChild(i), targetValue);
        if (resultNode != nullptr)
            return resultNode;
    }
    return nullptr;
}

Node* SystemManagement::findNode(const File& targetValue) {
    return findNode(gTree.getRoot(), targetValue);
}

void SystemManagement::addChild(Node* parentNode, Node* childNode) {
    if (parentNode != nullptr && childNode != nullptr)
        parentNode->addChild(childNode);
}

void SystemManagement::commandExecute() {
    std::string command = pars.getCommand();
    std::vector<std::string> arg = pars.getArguments();
    if (command == "ls") {
        ls();
    } else if (command == "date") {
        date();
    } else if (command == "cal") {
        cal();
    } else if (command == "df") {
        df();
    } else if (command == "free") {
        free();
    } else if (command == "help") {
        help();
    } else if (command == "cd") {
        //std::string arg = pars.getArguments();
        cd(arg[0]);
    } else if (command == "touch") {
        //std::string arg = pars.getArguments();
        createFile(arg[0]);
    } else if (command == "mkdir") {
        // for(int i = 0; i < arg.size(); ++i) {
        //     mkdir(arg[i]);
        // }
        mkdir(arg[0]);
    } else if (command == "pwd") {
        pwd();
    } else if (command == "rm") {
        //std::string arg = pars.getArguments();
        if (fileExists(arg[0]))
            removeFile(arg[0]);
        else if (directoryExists(arg[0]))
            removeDirectory(arg[0]);
        else
            std::cout << "File or directory not found." << std::endl;
    } else {
        std::cout << "Invalid command." << std::endl;
    }
}

void SystemManagement::ls() {
    listDirectory(currentDirectory);
}

void SystemManagement::listDirectory(const std::string& directoryPath, std::ostream& outputStream) {
    File directory(directoryPath, true);
    Node* node = findNode(directory);
    if (node != nullptr) {
        for (int i = 0; i < node->getChildCount(); i++) {
            File child = node->getChild(i)->getData();
            outputStream << child.getName() << std::endl;
        }
    }
}

void SystemManagement::listDirectory(const std::string& directoryPath) {
    listDirectory(directoryPath, std::cout);
}

void SystemManagement::date() {
    std::time_t t = std::time(nullptr);
    std::tm* timeInfo = std::localtime(&t);
    std::cout << std::put_time(timeInfo, "%c") << std::endl;
}

void SystemManagement::cal() {
    std::time_t t = std::time(nullptr);
    std::tm* timeInfo = std::localtime(&t);
    int year = timeInfo->tm_year + 1900;
    int month = timeInfo->tm_mon + 1;
    std::string command = "cal " + std::to_string(month) + " " + std::to_string(year);
    system(command.c_str());
}

void SystemManagement::df() {
    struct statvfs buf;
    statvfs("/", &buf);
    unsigned long total = buf.f_blocks * buf.f_frsize;
    unsigned long available = buf.f_bavail * buf.f_frsize;
    unsigned long used = total - available;
    std::cout << "Total disk space: " << total << " bytes" << std::endl;
    std::cout << "Used disk space: " << used << " bytes" << std::endl;
    std::cout << "Available disk space: " << available << " bytes" << std::endl;
}

void SystemManagement::free() {
    std::ifstream file("/proc/meminfo");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("MemTotal") != std::string::npos) {
                std::cout << line << std::endl;
            } else if (line.find("MemFree") != std::string::npos) {
                std::cout << line << std::endl;
            } else if (line.find("MemAvailable") != std::string::npos) {
                std::cout << line << std::endl;
            }
        }
        file.close();
    } else {
        std::cout << "Failed to open /proc/meminfo" << std::endl;
    }
}

void SystemManagement::help() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "ls                     - List files and directories in the current directory" << std::endl;
    std::cout << "date                   - Display the current date and time" << std::endl;
    std::cout << "cal                    - Display the calendar of the current month" << std::endl;
    std::cout << "df                     - Display disk space usage" << std::endl;
    std::cout << "free                   - Display memory usage" << std::endl;
    std::cout << "help                   - Display this help message" << std::endl;
    std::cout << "cd [directory]         - Change the current directory" << std::endl;
    std::cout << "touch [file]           - Create a new file" << std::endl;
    std::cout << "mkdir [directory]      - Create a new directory" << std::endl;
    std::cout << "pwd                    - Print the current working directory" << std::endl;
    std::cout << "rm [file or directory] - Remove a file or directory" << std::endl;
}

void SystemManagement::cd(const std::string& directoryPath) {
    if (directoryExists(directoryPath)) {
        if (directoryPath == "..") {
            if (currentDirectory != "/") {
                size_t pos = currentDirectory.find_last_of('/');
                currentDirectory = currentDirectory.substr(0, pos);
                if (currentDirectory.empty())
                    currentDirectory = "/";
            }
        } else if (directoryPath == ".") {
            // Do nothing, current directory remains the same
        } else {
            currentDirectory = directoryPath;
        }
    } else {
        std::cout << "Directory not found." << std::endl;
    }
}

void SystemManagement::createFile(const std::string& filename) {
    File file(filename, false);
    File parentDirectory(currentDirectory, true);

    if (gTree.insert(file, parentDirectory)) {
        std::cout << "File created successfully." << std::endl;
    } else {
        std::cout << "Failed to create file." << std::endl;
    }
}

Node* SystemManagement::dfsFindNode(Node* currentNode, const File& targetDir) {
    if (currentNode == nullptr)
        return nullptr;
    if (currentNode->getData() == targetDir)
        return currentNode;
    for (int i = 0; i < currentNode->getChildCount(); i++) {
        Node* resultNode = dfsFindNode(currentNode->getChild(i), targetDir);
        if (resultNode != nullptr)
            return resultNode;
    }
    return nullptr;
}

void SystemManagement::pwd() {
    std::cout << currentDirectory << std::endl;
}

void SystemManagement::mkdir(const std::string& dirname) {
    if (createDirectory(dirname)) {
        std::cout << "Directory created successfully." << std::endl;
    } else {
        std::cout << "Failed to create directory." << std::endl;
    }
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_SYSTEMMANAGEMENT_H
