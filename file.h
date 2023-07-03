#ifndef LINUX_EMULATOR_GENERALFILE_H
#define LINUX_EMULATOR_GENERALFILE_H

#include <iostream>

namespace LinuxEmulator{


class File {
public:
	File(const std::string&, bool);
	File(const std::string&, const std::string&, char*, bool);
	std::string getName() const;
	std::string getAbsolutePath() const;
	char* getContent() const;
	std::string getType() const;
	std::string getPermissions() const;
	bool getIsDirectory() const;
	void setName(const std::string&);
	void setAbsolutePath(const std::string&);
	void getContent(char*);
	std::string getType(const std::string&);
	void setContent(char*); 
    void setType(const std::string& t);

	void setPermissions(const std::string&);
	void setIsDirectory(bool);
	std::string reverse(const std::string&); 
	std::string returnExtension();
	File& operator=(const File& other);

private:
	std::string name;
	bool isDirectory;
	std::string absolutePath;
	std::string permissions;
	char* content;
	std::string type;
};

File::File(const std::string& n, bool is) 
	:name{n}
	,isDirectory{is}
	,permissions{"rw-rw-r–"}
{
	if(returnExtension() == "cpp") {
		type = "c++";
	} else if(returnExtension() == "js") {
		type = "javaScript";
	} else if(returnExtension() == "java") {
		type = "java";
	} else if(returnExtension() == "py") {
		type = "python";
	} else {
		type = "simpleTextFile";
	}
}

File::File(const std::string& n, const std::string& a, char*, bool is) 
	:name{n}
	,absolutePath{a}
	,permissions{"rw-rw-r–"}
	,content{nullptr}
	,isDirectory{is}
{
	if(returnExtension() == "cpp") {
		type = "c++";
	} else if(returnExtension() == "js") {
		type = "javaScript";
	} else if(returnExtension() == "java") {
		type = "java";
	} else if(returnExtension() == "py") {
		type = "python";
	} else {
		type = "simpleTextFile";
	}
}

std::string File::getName() const {
	return name;
}

std::string File::getAbsolutePath() const {
	return absolutePath;
}

char* File::getContent() const {
	return content;
}

std::string File::getType() const {
	return type;
}

std::string File::getPermissions() const {
	return permissions;
}

bool File::getIsDirectory() const {
    return isDirectory;
}

void File::setName(const std::string& n) {
	name = n;
}

void File::setAbsolutePath(const std::string& a) {
	absolutePath = a;
}

void File::setContent(char* c) {
	content = c;
}

void File::setType(const std::string& t) {
	type = t;
}

void File::setPermissions(const std::string& p) {
	permissions = p;
}

void File::setIsDirectory(bool i) {
	isDirectory = i;
}

std::string File::reverse(const std::string& str) {
   std::string nStr = "";
   int n = str.size();
    while(n-- > 0) {
        nStr += str[n];
    }
    return nStr;
}

std::string File::returnExtension() {
	std::string str = reverse(name);
	std::string extension = "";
	int pos = str.find('.');
	extension = str.substr(0, pos);
	extension = reverse(extension);
	return extension;
}


File& File::operator=(const File& other) {
	    if (this != &other) {
	        name = other.name;
	        isDirectory = other.isDirectory;
	        absolutePath = other.absolutePath;
	        permissions = other.permissions;
	        content = other.content;
	        type = other.type;
	    }
	    return *this;
	}

}  //namespace LinuxEmulator

bool operator==(const LinuxEmulator::File& lhs, const LinuxEmulator::File& rhs) {
    return lhs.getName() == rhs.getName() &&
           lhs.getAbsolutePath() == rhs.getAbsolutePath() &&
           lhs.getContent() == rhs.getContent() &&
           lhs.getType() == rhs.getType() &&
           lhs.getPermissions() == rhs.getPermissions() &&
           lhs.getIsDirectory() == rhs.getIsDirectory();
}

bool operator!=(const LinuxEmulator::File& lhs, const LinuxEmulator::File& rhs) {
    return !(lhs == rhs);
}

#endif  //LINUX_EMULATOR_GENERALFILE_H