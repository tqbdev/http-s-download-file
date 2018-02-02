#include "URL.h"
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>
URL::URL()
{
	this->full_path = "";
	this->path = "";
	this->protocol = "";
	this->query = "";
	this->host = "";
}


URL::~URL()
{
}
URL::URL(std::string url_s){
	this->full_path = url_s;
	this->parse(url_s);
}

void URL::parse(const string& url_s)
{
	const string prot_end("://");
	string::const_iterator prot_i = search(url_s.begin(), url_s.end(),
		prot_end.begin(), prot_end.end());
	protocol.reserve(distance(url_s.begin(), prot_i));
	transform(url_s.begin(), prot_i,
		back_inserter(protocol),
		ptr_fun<int, int>(tolower)); // protocol is icase
	if (prot_i == url_s.end())
		return;
	advance(prot_i, prot_end.length());
	string::const_iterator path_i = find(prot_i, url_s.end(), '/');
	host.reserve(distance(prot_i, path_i));
	transform(prot_i, path_i,
		back_inserter(host),
		ptr_fun<int, int>(tolower)); // host is icase
	string::const_iterator query_i = find(path_i, url_s.end(), '?');
	path.assign(path_i, query_i);
	if (query_i != url_s.end())
		++query_i;
	query.assign(query_i, url_s.end());
}