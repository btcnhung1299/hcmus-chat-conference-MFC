#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ClientChat.h"
#endif

#include "ClientChatDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CClientChatDoc, CDocument)

BEGIN_MESSAGE_MAP(CClientChatDoc, CDocument)
END_MESSAGE_MAP()


// CClientChatDoc construction/destruction

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
}

BOOL CClientChatDoc::OnNewDocument() {
	if (!CDocument::OnNewDocument()) {
		return FALSE;
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

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

	std::string tmpFilePath;
	CT2CA bufferUsername(username, CP_UTF8);
	dataSend.from = std::string(bufferUsername);

	if (dataSend.type == "fu" || dataSend.type == "fg") {
		tmpFilePath = dataSend.message;
		int deli = tmpFilePath.find_last_of("/\\");
		dataSend.message = tmpFilePath.substr(deli + 1);
	}
		
	SendCommonData(clntSock, dataSend);
	ReceiveCommonData(clntSock, dataResponse);

	BOOL res = false;
	if (dataSend.type == "cg") {
		res = (dataResponse.type == "cg" ? true : false);
	}
	else if (dataSend.type == "fu" || dataSend.type == "fg") {
		res = ((dataResponse.type == "fu" || dataResponse.type == "fg") ? true : false);
		if (res) {
			FILE *fp = fopen(tmpFilePath.data(), "rb");
			char buffer[FILE_BUFFER_SIZE];
			int byteRead = 0;

			if (fp == NULL) {
				AfxMessageBox(L"File not found!");
				res = false;
			}
			else {
				do {
					byteRead = fread(buffer, 1, FILE_BUFFER_SIZE, fp);
					clntSock.Send(&byteRead, sizeof(int), 0);
					if (clntSock.Send(buffer, byteRead) != byteRead) {
						AfxMessageBox(L"loss");
					}
				} while (byteRead == FILE_BUFFER_SIZE);

				fclose(fp);
			}
		}
	}

	clntSock.Close();
	return res;
}

void CClientChatDoc::InitListener() {
	if (!mainClntSock.Create(myPort)) {
		AfxMessageBox(L"Can't create listener");
	}
}

void CClientChatDoc::Receive(CommonData& receiveData) {
	if (!mainClntSock.Listen()) {
		AfxMessageBox(L"Can't listen");
	}

	mainClntSock.Accept(receiverConv);
	ReceiveCommonData(receiverConv, receiveData);
	receiverConv.Close();
}


// CClientChatDoc serialization

void CClientChatDoc::Serialize(CArchive& ar) {
	if (ar.IsStoring()) {
		// TODO: add storing code here
	}
	else {
		// TODO: add loading code here
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

#endif // SHARED_HANDLERS

// CClientChatDoc diagnostics

#ifdef _DEBUG
void CClientChatDoc::AssertValid() const {
	CDocument::AssertValid();
}

void CClientChatDoc::Dump(CDumpContext& dc) const {
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CClientChatDoc commands
