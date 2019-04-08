#include "SupportLib.h"

#include "stdafx.h"
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