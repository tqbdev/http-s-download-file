#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;
class HtmlNode{
public:
	vector<HtmlNode> ChildNodes;
	string tag = "";
	string InnerHtml = "";
	string InnerText = "";
	map<string, string> Attributes;
	vector<HtmlNode> findNodes(string tag);
};
class HtmlDocument
{
	string data;
	vector<HtmlNode> ChildNodes;
	map<string, string> ParseAttributes(string html);
public:
	HtmlDocument();
	void LoadHtml(string source);
	void Parse();
	vector<HtmlNode> getChildNodes();
	vector<HtmlNode> findNodes(string tag);
	~HtmlDocument();
};

