#pragma once
#include <direct.h>
#include <fstream>
#include "HTTP.h"
#include "HtmlDocument.h"
#include <thread>
#include <iomanip>
#include "Script.h"
using namespace std;

#define NAME_TAG "1512003_1512004_1512029_"

void DownloadFile(string url,HttpVer ver) {
	URL Link = URL(url);
	vector<string> listPath = Split(Link.path, "/");
	ofstream myfile;
	string fileName = NAME_TAG + listPath[listPath.size() - 1];
	myfile.open(fileName, ios::out | ios::binary);

	HTTP httpDown(Link, ver);

	std::thread t1(&HTTP::RequestFile, std::ref(httpDown), std::ref(myfile));
	t1.detach();

	int sizeFile = -1;

	while (true){
		sizeFile = atoi(httpDown.GetHeader()["Content-Length"].c_str());
		if (sizeFile != 0){
			for (int i = 0; i < httpDown.GetSizeRecv() / (sizeFile / 100); ++i){
				cout << "-";
			}

			if (httpDown.GetSizeRecv() >= sizeFile) {
				cout << ">100%       ";
				break;
			}
			else{
				cout << ">" << std::setprecision(2) << httpDown.GetSizeRecv() * 100.0 / sizeFile << "%       ";
				cout << "\r";
			}
		}
	}

	if (httpDown.GetStatus() != 200) {
		cout << "Error Web " << httpDown.GetStatus() << endl;
		myfile.close();
		remove(fileName.c_str());
		return;
	}

	cout << "\nDownloaded: " << listPath[listPath.size() - 1] << " - Size: " << httpDown.GetHeader()["Content-Length"] << endl;
	string data = httpDown.GetResponse();
	myfile.close();
}
void DownloadFolder(string path, HttpVer ver){
	string rootFolderName = NAME_TAG + GetFolderName(path);
	rootFolderName += '/';

	_mkdir(rootFolderName.c_str());

	HTTP http(path, ver);
	http.Request();
	if (http.GetStatus() != 200){
		cout << "\nError Web " << http.GetStatus() << endl;
		return;
	}
	HtmlDocument html;
	html.LoadHtml(http.GetResponse());
	html.Parse();
	string response = http.GetResponse();

	vector<HtmlNode> list = html.findNodes("a");

	for (int i = 0; i < list.size(); ++i){
		if (list[i].Attributes["href"] != ""){

			int find = path.find(list[i].Attributes["href"]);
			if (isFile(list[i].Attributes["href"]) == false)
			{
				string subFolderPath = NAME_TAG + GetFolderName(list[i].Attributes["href"]);
				_mkdir(subFolderPath.c_str());
				continue;
			}

			ofstream myfile;
			string fileName = rootFolderName + list[i].InnerText;
			myfile.open(fileName, ios::out | ios::binary);

			HTTP httpDown(URL(path + list[i].Attributes["href"]), ver);
			std::thread t1(&HTTP::RequestFile, std::ref(httpDown), std::ref(myfile));
			t1.detach();

			int sizeFile = -1;
			while (true){
				sizeFile = atoi(httpDown.GetHeader()["Content-Length"].c_str());
				if (sizeFile != 0){
					for (int i = 0; i < httpDown.GetSizeRecv() / (sizeFile / 100); ++i){
						cout << "-";
					}

					if (httpDown.GetSizeRecv() >= sizeFile) {
						cout << ">100%       ";
						break;
					}
					else{
						cout << ">" << std::setprecision(2) << httpDown.GetSizeRecv() * 100.0 / sizeFile << "%       ";
						cout << "\r";
					}
				}
			}
			if (httpDown.GetStatus() != 200){
				cout << "\nError Web " << http.GetStatus() << endl;
				myfile.close();
				remove(fileName.c_str());
				continue;
			}
			cout << "\nDownloaded: " << list[i].InnerText << " - Size: " << httpDown.GetHeader()["Content-Length"] << endl;
			myfile.close();
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc == 3){
		HttpVer ver = HTTP1_0;
		if (strcmp(argv[2], "--http1.0") == 0){
			ver = HTTP1_0;
		}
		else if (strcmp(argv[2], "--http1.1") == 0){
			ver = HTTP1_1;
		}
		else{
			cout << "Error Version HTTP, use defaut http1.0" << endl;
		}

		string path = string(argv[1]);

		if (isFile(path)){
			DownloadFile(path, ver);
		}
		else{
			DownloadFolder(path, ver);
		}
	}

	system("pause");
	return 0;
}
