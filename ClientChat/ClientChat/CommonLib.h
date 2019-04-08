#pragma once

#include <string>
#include <afxsock.h>

#define MAX_FILE_SIZE 5242880


//SERVER WILL NOT READ THIS VALUE FROM CLIENT.
struct CommonTime {
	int second, minute, hour, day, month, year;
};

//Data for exchaging between server and client
struct CommonData {
	std::string from;	//senderId
	std::string to;		//receiverId
	std::string type;	//mu: U2U message, mg: U2G message, fu: U2U file sharing, fg: U2G file sharing 
	std::string message;
	CommonTime timeStamp;	//SERVER WILL NOT READ THIS VALUE. CLIENT SCHOULDN'T CHANGE THIS VALUE.
	int fileSize;		// from 1 to MAX_FILE_SIZE Bytes
};

//Convert string to wstring
void convertStr2WStr(std::string& str, std::wstring& wstr);

//Send data through a socket
void SendCommonData(CSocket& sock, CommonData& data);

//Receive data through a socket
void ReceiveCommonData(CSocket& sock, CommonData& data);