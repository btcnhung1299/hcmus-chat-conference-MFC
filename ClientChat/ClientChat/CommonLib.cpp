#include "stdafx.h"
#include "CommonLib.h"

#include <afxsock.h>

using namespace std;


// ==== PRIVATE FUNCTIONS -- SUPPORT THIS CPP ONLY

void SendString(CSocket& sock, const char * charArr) {
	int charArrLen = strlen(charArr);
	sock.Send(&charArrLen, sizeof(int), 0);
	sock.Send(charArr, charArrLen, 0);
}

void SendString(CSocket& sock, string& str) {
	SendString(sock, str.data());
}

void ReceiveString(CSocket& sock, char *& charArr) {
	int charArrLen = 0;
	sock.Receive(&charArrLen, sizeof(int), 0);
	charArr = new char[charArrLen + 1];
	sock.Receive(charArr, charArrLen, 0);
	charArr[charArrLen] = '\0';
}

void ReceiveString(CSocket& sock, string& str) {
	char * tmpCharArr;
	ReceiveString(sock, tmpCharArr);
	str = tmpCharArr;
	delete[] tmpCharArr;
}



// ==== PUBLIC FUNCTIONS -- USE IN OTHER CPP


void convertStr2WStr(string& str, wstring& wstr) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	wstr.clear();
	wstr.resize(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
}

void SendCommonData(CSocket& sock, CommonData& data) {
	//Send Type
	SendString(sock, data.type);
	//Send From
	SendString(sock, data.from);
	//Send To
	SendString(sock, data.to);
	//Send message
	SendString(sock, data.message);
	//Send timeStampt
	sock.Send(&data.timeStamp, sizeof(CommonTime), 0);
	//Send fileSize
	sock.Send(&data.fileSize, sizeof(int), 0);
}

void ReceiveCommonData(CSocket& sock, CommonData& data) {
	//Receive Type
	ReceiveString(sock, data.type);
	//Receive From
	ReceiveString(sock, data.from);
	//Receive To
	ReceiveString(sock, data.to);
	//Receive message
	ReceiveString(sock, data.message);
	//Receive timeStampt
	sock.Receive(&data.timeStamp, sizeof(CommonTime), 0);
	//Receive fileSize
	sock.Receive(&data.fileSize, sizeof(int), 0);
}