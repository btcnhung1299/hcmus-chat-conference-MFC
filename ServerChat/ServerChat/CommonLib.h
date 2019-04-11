#pragma once

#include <string>
#include <afxsock.h>

struct CommonTime {
	int second, minute, hour, date, month, year;
};

struct CommonData {
	std::string from;	//source
	std::string to;		//destination
	std::string type;	//mu: U2U message, mg: U2G message, fu: U2U file sharing, fg: U2G file sharing 
	std::string message;
	CommonTime timeStampt;
	int fileSize;	// from 1 to 5 * 1024 * 1024 Bytes
};

void convertStr2WStr(std::string& str, std::wstring& wstr);
void ReceiveCommonData(CSocket& sock, CommonData& data);
void SendCommonData(CSocket& sock, CommonData& data);