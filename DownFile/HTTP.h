#pragma once
#include <WinSock2.h>
#include <Ws2tcpip.h> //inet_pton
#pragma comment(lib,"ws2_32.lib") //WinSock
#include <string>
#include <iostream>
#include <fstream>
#include "URL.h"
#include "Header.h"
using namespace std;

enum HttpVer
{
	HTTP1_0,
	HTTP1_1
};

#define BUFFERSIZE 1024
class HTTP
{
private:
	URL pathURL;
	string host;
	int port;
	SOCKET sock;
	WSADATA wsaData;
	/// lấy địa chỉ ip từ host
	string getHostByName(string name);
	void Connect();
	void InitSocket();
	Header MapHeader;
	string Response;
	int recv_size;
	HttpVer ver;
public:
	HTTP();
	HTTP(string website, HttpVer ver = HTTP1_0);
	HTTP(URL url, HttpVer ver = HTTP1_0);
	//HTTP(string host, int port = 80);
	string GetResponse();
	Header GetHeader();
	void Request();
	void RequestFile(ofstream &out);
	int GetSizeRecv();
	int GetStatus();
	~HTTP();
};

