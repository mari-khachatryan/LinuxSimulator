#ifndef LINUX_EMULATOR_USER_H
#define LINUX_EMULATOR_USER_H

#include <string>

namespace LinuxEmulator{

class User {
public:
	User();
	User(const std::string&, const std::string&);
	void setName(const std::string&);
	std::string getName() const;
	void setPassword(const std::string&);
	std::string getPassword() const;
	int getUid() const;
	int getGid() const;
private:
	std::string name;
	std::string password;
	std::string server;
	int uid;
	int gid;
};

User::User() = default;

User::User(const std::string& u, const std::string& p) : name{u}, password{p}, server{"192.168.1.2"}, 
	uid{100}, gid{100} {}

void User::setName(const std::string& n) {
	name = n;
}

std::string User::getName() const {
	return name;
}

int User::getUid() const {
	return uid;
}

int User::getGid() const {
	return gid;
}

void User::setPassword(const std::string& p) {
	password = p;
}

std::string User::getPassword() const {
	return password;
}

}  //namespace LinuxEmulator

#endif   //LINUX_EMULATOR_USER_H