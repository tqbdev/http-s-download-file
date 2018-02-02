#include "HTTP.h"
string HTTP::getHostByName(string name){
	try {
		hostent *h = gethostbyname(name.c_str());
		unsigned char *addr = reinterpret_cast<unsigned char *>(h->h_addr_list[0]);
		in_addr * address = (in_addr *)h->h_addr;
		string ip_address = inet_ntoa(*address);
		return ip_address;
	}
	catch (std::exception const &exc) {
		std::cerr << exc.what() << "\n";
		return "";
	}
	return "";
}

void HTTP::InitSocket(){
	//init winsock
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		cout << "WSAStartup() failed" << endl;
	//open socket
	if ((this->sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		cout << "socket() failed" << endl;
}

HTTP::HTTP()
{
	this->port = 80;
	this->host = "";
	this->recv_size = 0;
	this->InitSocket();
}

HTTP::HTTP(URL url, HttpVer ver /*= HTTP1_0*/){
	this->InitSocket();
	this->pathURL = url;
	this->port = 80;
	this->recv_size = 0;
	this->host = this->getHostByName(url.host);
	//this->Header = "";
	this->Response = "";
	this->ver = ver;
}


HTTP::HTTP(string website, HttpVer ver /*= HTTP1_0*/){
	this->InitSocket();
	this->pathURL = URL(website);
	this->port = 80;
	this->recv_size = 0;
	this->host = this->getHostByName(pathURL.host);
	//this->Header = "";
	this->Response = "";
	this->ver = ver;
}

HTTP::~HTTP()
{
	closesocket(this->sock);
	WSACleanup();
}

void HTTP::Connect(){
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(this->host.c_str());
	serveraddr.sin_port = htons((unsigned short)port);
	if (connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
		cout << "connect() failed" << endl;
}
void HTTP::Request(){
	//Connect
	this->Connect();

	//Make Request
	string request = "";
	request += "GET " + this->pathURL.path + " HTTP/1.";

	if (this->ver == HTTP1_0) request += "0";
	if (this->ver == HTTP1_1) request += "1";

	request += "\r\n";
	request += "Host: " + this->pathURL.host + "\r\n";
	request += "User-Agent: runscope/0.1\r\n";
	request += "Accept: */*\r\n";
	request += "Accept-Encoding: *\r\n";
	request += "Connection: close\r\n";
	request += "\r\n";

	//send request
	if (send(sock, request.c_str(), request.length(), 0) != request.length())
		cout << "send() sent a different number of bytes than expected" << endl;

	bool done = false;
	bool EndHeader;
	int nByteRevc;
	char HeaderResponse;
	string dataHeader = "";
	//Lấy Header
	while (!done){
		//Lấy 1 byte dữ liệu
		nByteRevc = recv(this->sock, &HeaderResponse, 1, 0);

		if (nByteRevc < 0)    // Loi
			done = true;
		switch (HeaderResponse)    {
		case '\r':
			break;
		case '\n': // Nếu gặp "\n\n" hay "\n\r\n" thì kết thúc Header
			if (EndHeader == true){
				done = true;
			}
			EndHeader = true;
			break;
		default:
			EndHeader = false;
			break;
		}
		dataHeader += HeaderResponse;	
	}
	//Tạo Header
	this->MapHeader = Header(dataHeader);

	//Lấy Response
	int resp_leng = BUFFERSIZE;
	char buffer[BUFFERSIZE];
	while (true)
	{
		resp_leng = recv(sock, (char*)&buffer, BUFFERSIZE, 0);
		if (resp_leng <= 0) {
			break;
		}
		this->Response += string(buffer, resp_leng);
		this->recv_size += resp_leng; // Tổng byte đã lấy
	}
}

void HTTP::RequestFile(ofstream &out)
{
	//Connect
	this->Connect();

	//Make Request
	string request = "";
	request += "GET " + this->pathURL.path + " HTTP/1.";

	if (this->ver == HTTP1_0) request += "0";
	if (this->ver == HTTP1_1) request += "1";

	request += "\r\n";
	request += "Host: " + this->pathURL.host + "\r\n";
	request += "User-Agent: runscope/0.1\r\n";
	request += "Accept: */*\r\n";
	request += "Accept-Encoding: *\r\n";
	request += "Connection: close\r\n";
	request += "\r\n";

	//send request
	if (send(sock, request.c_str(), request.length(), 0) != request.length())
		cout << "send() sent a different number of bytes than expected" << endl;

	bool done = false;
	bool EndHeader;
	int nByteRevc;
	char HeaderResponse;
	string dataHeader = "";
	//Lấy Header
	while (!done) {
		//Lấy 1 byte dữ liệu
		nByteRevc = recv(this->sock, &HeaderResponse, 1, 0);

		if (nByteRevc < 0)    // Loi
			done = true;
		switch (HeaderResponse) {
		case '\r':
			break;
		case '\n': // Nếu gặp "\n\n" hay "\n\r\n" thì kết thúc Header
			if (EndHeader == true) {
				done = true;
			}
			EndHeader = true;
			break;
		default:
			EndHeader = false;
			break;
		}
		dataHeader += HeaderResponse;
	}
	//Tạo Header
	this->MapHeader = Header(dataHeader);

	//Lấy Response
	int resp_leng = BUFFERSIZE;
	char buffer[BUFFERSIZE];
	while (true)
	{
		resp_leng = recv(sock, (char*)&buffer, BUFFERSIZE, 0);
		if (resp_leng <= 0) {
			break;
		}
		//this->Response += string(buffer, resp_leng);
		out.write(buffer, resp_leng);
		this->recv_size += resp_leng; // Tổng byte đã lấy
	}
}

string HTTP::GetResponse(){
	return this->Response;
}

Header HTTP::GetHeader(){
	return this->MapHeader;
}

int HTTP::GetSizeRecv(){
	return this->recv_size;
}

int HTTP::GetStatus(){
	return this->MapHeader.getStatus();
}