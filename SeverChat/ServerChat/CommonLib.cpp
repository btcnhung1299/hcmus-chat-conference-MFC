#include "stdafx.h"
#include "CommonLib.h"

#include <afxsock.h>

using namespace std;

void convertStr2WStr(string& str, wstring& wstr) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	wstr.clear();
	wstr.resize(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
}

//Extract data from a standard data
string GetData(char * fullData, char * dataName) {
	string res;
	return res;
}

void SendCharArr(CSocket& sock, const char * charArr) {
	int charArrLen = strlen(charArr);
	sock.Send(&charArrLen, sizeof(int), 0);
	sock.Send(charArr, charArrLen, 0);

	//for (int i = 0; i < charArrLen; i++)
	//{
	//	sock.Send(&charArr[i], 1, 0);
	//}
}

void SendCharArr(CSocket& sock, string& str) {
	SendCharArr(sock, str.data());
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
}

void SendCommonData(CSocket& sock, CommonData& data) {
	//Send Type
	SendCharArr(sock, data.type);
	//Send From
	SendCharArr(sock, data.from);
	//Send To
	SendCharArr(sock, data.to);
	//Send message
	SendCharArr(sock, data.message);
	//Send timeStampt
	sock.Send(&data.timeStampt, sizeof(CommonTime), 0);
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
	sock.Receive(&data.timeStampt, sizeof(CommonTime), 0);
	//Receive fileSize	
	sock.Receive(&data.fileSize, sizeof(int), 0);
}