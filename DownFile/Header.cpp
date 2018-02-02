#include "Header.h"
#include "Script.h"

Header::Header()
{
}


Header::~Header()
{
	MapHeader.clear();
}


Header::Header(string msg){
	this->data = msg;
	vector<string> list = Split(msg, "\r\n");
	for (int i = 0; i < list.size(); ++i){
		vector<string> mapv = Split(list[i], ": ");
		if (mapv.size() >= 2){
			this->MapHeader[mapv[0]] = mapv[1];
		}
		else{
			vector<string> mapv = Split(list[i], " ");
			if (mapv.size() == 3)
				this->Status = atoi(mapv[1].c_str());
		}
	}
}

string Header::toString(){
	return this->data;
}

int Header::getStatus(){
	return this->Status;
}