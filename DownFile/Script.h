#pragma once
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
vector<string> Split(string str, string delimiter);

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);

bool isFile(string url);

string GetFolderName(string url);