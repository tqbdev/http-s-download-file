#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;
class Header
{
private:
	std::map<std::string, std::string> MapHeader;
	string data;
	int Status;
public:
	string toString();
	Header();
	Header(string msg);
	string& operator[] (string x) {
		return MapHeader[x];
	}
	int getStatus();
	~Header();
};

