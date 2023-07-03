#ifndef LINUX_EMULATOR_COMMANDVALIDATOR_H
#define LINUX_EMULATOR_COMMANDVALIDATOR_H

#include "parsing.h"
#include <map>
#include <algorithm>
#include <regex>

namespace LinuxEmulator{


class CommandValidator {
public:
    CommandValidator();
    CommandValidator(const Parsing&);
    bool isValidCommand(const Parsing&);
    Parsing getCommand() const;
    std::map<std::string, std::vector<std::string>> getValidOptions() const;
private:
    Parsing command;
    std::vector<std::string> validCommands = {
        "date",
        "cal",
        "df",
        "free",
        "mkdir",
        "ls",
        "pwd",
        "cd",
        "touch",
        "cat",
        "vim",
        "file",
        "mv",
        "cp",
        "rm",
        "less",
        "rmdir",
        "ln",
        "wc",
        "head",
        "tail",
        "echo",
        "history",
        "clear",
        "passwd",
        "id",
        "help",
        "ssh",
        "useradd",
        "chmod",
        "ps",
        "top",
        "jobs",
    };
    std::map<std::string, std::vector<std::string>> validOptions = {
        {"cal", {}},
        {"date", {"-d", "-f", "-r"}},
        {"df", {"-a", "-h"}},
        {"free", {"-b", "-k", "-m"}},
        {"mkdir", {"-m", "-p", "-v", "-Z"}},
        {"ls", {"-a", "-A", "-d", "-F", "-h", "-l", "-r", "-S", "-t", "-la", "-lt"}},
        {"pwd", {"-L", "-P"}},
        {"cd", {"-L", "-P", "-e", "-@", "..", ".", "~", "-"}},
        {"touch", {"-a", "-c", "-d", "-f", "-h", "-m", "-r", "-t"}},
        {"cat", {"-A", "-b", "-E", "-e", "-n", "-s", "-t", "-T", "-u", "-v"}},
        {"vim", {"-v", "-e", "-E", "-s", "-d", "-y", "-r"}},
        {"file", {"-v", "-m", "-z", "-Z", "-b", "-c", "-e", "-f"}},
        {"mv", {"-b", "-f", "-i", "-n", "-S", "-t", "-T", "-u", "-v", "-Z"}},
        {"cp", {"-a", "-b", "-d", "-f", "-i", "-H", "-l"}},
        {"rm", {"-f", "-i", "-I", "-r", "-d", "-v"}},
        {"less", {}},
        {"ln", {"-s"}},
        {"wc", {"-l", "-c", "-m"}},
        {"head", {"-n"}},
        {"tail", {"-n"}},
        {"echo", {}},
        {"history", {}},
        {"clear", {}},
        {"id", {}},
        {"passwd", {}},
        {"help", {}},
        {"ssh", {}},
        {"useradd", {}},
        {"chmod", {}},
        {"ps", {}},
        {"top", {}},
        {"jobs", {}}
    };
};

CommandValidator::CommandValidator() = default;

CommandValidator::CommandValidator(const Parsing& com) : command(com) {}

Parsing CommandValidator::getCommand() const {
    return command;
}

std::map<std::string, std::vector<std::string>> CommandValidator::getValidOptions() const {
    return validOptions;
}

bool CommandValidator::isValidCommand(const Parsing& com) {
    std::string commandName = com.getCommand();
    if (std::find(validCommands.begin(), validCommands.end(), commandName) == validCommands.end()) {
        return false;
    }
    const std::map<std::string, std::vector<std::string>>& options = com.getOptions();
    const std::vector<std::string>& validOpts = validOptions[commandName];
    for (const auto& pair : options) {
        const std::vector<std::string>& opts = pair.second;
        for (const std::string& opt : opts) {
            if (std::find(validOpts.begin(), validOpts.end(), opt) == validOpts.end()) {
                return false;
            }
        }
    }
    const std::vector<std::string>& arguments = com.getArguments();
    return true;
}

}  //namespace LinuxEmulator

#endif
