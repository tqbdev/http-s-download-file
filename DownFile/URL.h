#pragma once
#include <string>
using namespace std;
class URL
{
public:
	std::string full_path,protocol, host, path, query;
	URL(std::string url_s);
	URL();
	~URL();
private:
	void parse(const std::string& url_s);
};

