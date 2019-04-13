#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "ClientChat.h"
#endif

#include "ClientChatDoc.h"
#include <propkey.h>
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CClientChatDoc, CDocument)
BEGIN_MESSAGE_MAP(CClientChatDoc, CDocument)
END_MESSAGE_MAP()


// ---------------------------- CONSTRUCTOR / DESTRUCTOR -----------------------
CClientChatDoc::CClientChatDoc() noexcept {
	AfxSocketInit(NULL);

	// Connect to server to get contact port
	CSocket clntSock;
	clntSock.Create();
	CServerSettings serverSettingsDlg;
	BOOL connected = false;

	while (!connected && serverSettingsDlg.DoModal() == btnConnect) {
		serverPort = serverSettingsDlg.GetServerPort();
		serverIP = serverSettingsDlg.GetServerIP();
		if (clntSock.Connect(serverIP, serverPort)) {
			connected = true;
			clntSock.Receive(&contactPort, 4, 0);
		}
	}
	clntSock.Close();
}

CClientChatDoc::~CClientChatDoc() {
	CDocument::~CDocument();
	clntSock.Close();
	mainClntSock.Close();
	receiver.Close();
}

BOOL CClientChatDoc::OnNewDocument() {
	if (!CDocument::OnNewDocument()) {
		return FALSE;
	}
	return TRUE;
}

// --------------------------- SEND REQUESTS ------------------------
BOOL CClientChatDoc::Send(CommonData& dataSend, CommonData& dataResponse) {
	if (!clntSock.Create()) {
		AfxMessageBox(L"Can't create socket");
	}

	if (!clntSock.Connect(serverIP, serverPort)) {
		AfxMessageBox(L"Can't connect to server");
	}

	if (!clntSock.Receive(&contactPort, 4, 0)) {
		AfxMessageBox(L"Didn't receive contact port from server");
	}

	clntSock.Close();
	
	if (!clntSock.Create()) {
		AfxMessageBox(L"Can't create socket");
	}

	if (!clntSock.Connect(serverIP, contactPort)) {
		AfxMessageBox(L"Can't connect to contace port provided");
	}

	// Add username to every packet sent
	CT2CA bufferUsername(username, CP_UTF8);
	dataSend.from = std::string(bufferUsername);

	// User specified path for sending / receiving file
	std::string filePath;
	if (dataSend.type == "fu" || dataSend.type == "fg") {
		filePath = dataSend.message;
		dataSend.message = filePath.substr(filePath.find_last_of("/\\") + 1);
	}
	else if (dataSend.type == "uf" || dataSend.type == "gf") {
		filePath = dataResponse.message;
	}
	
	// Send and receive response from server after each request
	SendCommonData(clntSock, dataSend);
	ReceiveCommonData(clntSock, dataResponse);

	// Handle server responses
	BOOL sucRequest = false;
	if (dataSend.type == "cg") {
		sucRequest = (dataResponse.type == "cg" ? true : false);
	}
	else if (dataSend.type == "fu" || dataSend.type == "fg") {
		sucRequest = ((dataResponse.type == "fu" || dataResponse.type == "fg") ? true : false);

		// If metadata received successfully by server
		if (sucRequest) {
			FILE *fp = fopen(filePath.data(), "rb");
			long long buffer;

			int byteRead = 0;

			if (fp == NULL) {
				AfxMessageBox(L"File not found!");
				sucRequest = false;
				goto Out;
			}
			
			do {
				byteRead = fread(&buffer, 1, sizeof(long long), fp);
				clntSock.Send(&byteRead, sizeof(long long), 0);
				if (clntSock.Send(&buffer, byteRead) != byteRead) {
					AfxMessageBox(L"Data loss occured!");
				}

			} while (byteRead == sizeof(long long));

			// Sending file terminator
			char terminator = 0;
			clntSock.Send(&terminator, sizeof(char), 0);
			fclose(fp);

			// Waiting until server received file successfully
			int fullReceive = 0;
			do {
				clntSock.Receive(&fullReceive, sizeof(int), 0);
			} while (fullReceive != 1);
		}
	}
	else if (dataSend.type == "uf" || dataSend.type == "gf") {
		sucRequest = ((dataResponse.type == "uf" || dataResponse.type == "gf") ? true : false);

		Sleep(200);

		// If having received metadata, start receiving actual file from server
		if (sucRequest) {
			std::ofstream fo;
			fo.open(filePath.data(), std::ios::binary);
			if (!fo) {
				AfxMessageBox(L"Unable to save file to specified path.");
				sucRequest = false;
				goto Out;
			}

			char buffer[8];
			int toWriteSize = 0;
			int temp = 0;

			do {
				clntSock.Receive(&temp, sizeof(long long), 0);
				toWriteSize = temp;
				if (toWriteSize <= 0) break;
				clntSock.Receive(buffer, toWriteSize, 0);
				fo.write(buffer, toWriteSize);
				temp = 0;
			} while (toWriteSize > 0 && toWriteSize <= sizeof(long long));

			int check = 1;
			clntSock.Send(&check, sizeof(int), 0);
			fo.close();
		}
	}

	Out:
	clntSock.Close();
	return sucRequest;
}


// -------------------- THREAD - RECEIVE REQUESTS -------------------
void CClientChatDoc::InitListener() {
	if (!mainClntSock.Create(myPort)) {
		AfxMessageBox(L"Can't create listener");
	}
}

void CClientChatDoc::Receive(CommonData& receiveData) {
	if (!mainClntSock.Listen()) {
		AfxMessageBox(L"Can't listen");
	}

	mainClntSock.Accept(receiver);
	ReceiveCommonData(receiver, receiveData);
	receiver.Close();
}


// ------------------------ SERIALIZATION ----------------------------
void CClientChatDoc::Serialize(CArchive& ar) {
	if (ar.IsStoring()) {
	}
	else {
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CClientChatDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) {
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CClientChatDoc::InitializeSearchContent() {
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CClientChatDoc::SetSearchContent(const CString& value) {
	if (value.IsEmpty()) {
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else {
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr) {
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif

#ifdef _DEBUG
void CClientChatDoc::AssertValid() const {
	CDocument::AssertValid();
}

void CClientChatDoc::Dump(CDumpContext& dc) const {
	CDocument::Dump(dc);
}
#endif