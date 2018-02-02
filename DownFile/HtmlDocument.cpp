#include "HtmlDocument.h"
#include "Script.h"
#include <algorithm>



vector<HtmlNode> HtmlNode::findNodes(string tag){
	vector<HtmlNode> result;
	int len = this->ChildNodes.size();
	for (int i = 0; i < len; ++i){
		if (this->ChildNodes[i].tag == tag) result.push_back(this->ChildNodes[i]);
		else{
			vector<HtmlNode> nodes = this->ChildNodes[i].findNodes(tag);
			for each (HtmlNode n in nodes)
			{
				result.push_back(n);
			}
		}
	}
	return result;
}


HtmlDocument::HtmlDocument()
{
}


HtmlDocument::~HtmlDocument()
{
}


void HtmlDocument::LoadHtml(string source){
	this->data = ReplaceAll(source, ">\n", ">");
	int start = this->data.find("<table>");
	int end = this->data.find("</table>");
	if (start != -1 && end != -1)
		this->data = this->data.substr(start + 7, end - start - 7);
}

vector<HtmlNode> HtmlDocument::getChildNodes(){
	return this->ChildNodes;
}
vector<HtmlNode> HtmlDocument::findNodes(string tag){
	vector<HtmlNode> result;
	int len = this->ChildNodes.size();
	for (int i = 0; i < len; ++i){
		if (this->ChildNodes[i].tag == tag) result.push_back(this->ChildNodes[i]);
		else{
			vector<HtmlNode> nodes = this->ChildNodes[i].findNodes(tag);
			for each (HtmlNode n in nodes)
			{
				result.push_back(n);
			}
		}
	}
	return result;
}

void HtmlDocument::Parse(){
	string source = this->data;
	//this->ChildNode.LoadHtml(source);
	if (source == "") return;
	int pos = 0;
	string temp = "";
	while (pos != std::string::npos){
		if (source[pos] == ' ') { 
			source.erase(0, 1);
			continue;
		}
		if (source[pos] == '<'){
			//Add note
			HtmlNode node = HtmlNode();
			int posEnd = source.find(">");
			//parse tag
			string inTag = source.substr(pos + 1, posEnd - (pos + 1));
			node.tag = inTag.substr(0, source.find(' ') - 1); // Parse tag
			inTag.erase(0, source.find(' '));
			node.Attributes = this->ParseAttributes(inTag); // Parse Attributes
			source.erase(0, posEnd + 1);

			//parese InnerHTML
			string nodeEnd = "</" + node.tag + ">";
			posEnd = source.find(nodeEnd);
			node.InnerHtml = source.substr(pos, posEnd);
			//if (posEnd == -1) continue;
			source.erase(pos, posEnd + nodeEnd.size());
			//parese InnerText
			string InnerHtml = node.InnerHtml;
			int posStart = InnerHtml.find("<");
			node.InnerText = InnerHtml.substr(0, posStart);
			//Parse ChildNodes
			InnerHtml = InnerHtml.erase(0, posStart);
			HtmlDocument doc;
			doc.LoadHtml(InnerHtml);
			doc.Parse();
			node.ChildNodes = doc.getChildNodes();
			this->ChildNodes.push_back(node);
		}
		else{
			break;
		}
	}
}

map<string, string> HtmlDocument::ParseAttributes(string html){
	map<string, string> result;
	string temp;
	string name = "";
	int isRead = 0;
	for (int i = 0; i < html.size(); ++i){
		if (html[i] == '"'){
			isRead++;
		}
		else{
			if (html[i] == '='){
				name = ReplaceAll(temp, " ", "");
				temp = "";
			}
			else
				temp += html[i];
		}
		if (isRead == 2){
			result[name] = temp;
		}
	}
	return result;
}