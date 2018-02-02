#include "Script.h"


vector<string> Split(string str, string delimiter){
	vector<string> result;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		result.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	if (str.size() > 0){
		result.push_back(str);
	}
	return result;
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

bool isFile(string url){
	vector<string> listURL = Split(url, "/");
	string test = listURL[listURL.size() - 1];
	int pos = test.find(".");
	if (pos > -1){
		return true;
	}
	return false;
}

string GetFolderName(string url) {
	string res = "";
	
	bool start = false;

	for (int i = url.length() - 1; i >= 0; i--) {
		if (start == true)
		{
			if (url[i] != '/')
			{
				res += url[i];
			}
			else break;
		}
		if (url[i] == '/') start = true;
	}

	reverse(res.begin(), res.end());
	return res;
}