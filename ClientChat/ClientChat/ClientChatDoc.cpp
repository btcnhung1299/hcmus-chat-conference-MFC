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

// CClientChatDoc

IMPLEMENT_DYNCREATE(CClientChatDoc, CDocument)

BEGIN_MESSAGE_MAP(CClientChatDoc, CDocument)
END_MESSAGE_MAP()


// CClientChatDoc construction/destruction

CClientChatDoc::CClientChatDoc() noexcept {
	AfxSocketInit(NULL);
	if (!clntSock.Create()) {
		AfxMessageBox(L"Can't create socket");
	}

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

	if (!clntSock.Create()) {
		AfxMessageBox(L"Can't create socket");
	}

	if (!clntSock.Connect(serverIP, contactPort)) {
		AfxMessageBox(L"Can't connect to contact port provided");
	}

	CLogin loginDlg;

	CommonData loginInfo;
	int loginOption = 0;
	
	BOOL loginStatus = false;

	while (!loginStatus && loginDlg.DoModal() == btnOKLogin) {
		// Save username to global variable for later use
		username = loginDlg.GetUsername();
		loginOption = loginDlg.GetLoginOption();
		CT2CA bufferUsername(username, CP_UTF8);
		CT2CA bufferPassword(loginDlg.GetPassword(), CP_UTF8);
		
		loginInfo.from = std::to_string(contactPort);
		loginInfo.type = (loginOption == 0 ? "li" : "re");
		loginInfo.fileSize = username.GetLength();
		loginInfo.message = std::string(bufferUsername) + std::string(bufferPassword);
		
		SendCommonData(clntSock, loginInfo);

		/*clntSock.Send(&commandCode, 1, 0);
		CT2A  bufferUsername(username, CP_UTF8);
		int len = sizeof(bufferUsername);
		clntSock.Send(&len, 4, 0);
		
		if (clntSock.Send(bufferUsername, len, 0) != len) {
			AfxMessageBox(L"Can't send user info");
		}

		clntSock.Send(&receivePort, 4, 0);
		clntSock.Receive(&loggedin, 1, 0);*/
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

void CClientChatDoc::send(CString msg) {
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

	char commandCode = 'c';
	clntSock.Send(&commandCode, 1, 0);
	
	CT2A  buffer(msg, CP_UTF8);
	int len = sizeof(buffer);
	clntSock.Send(&len, 4, 0);

	if (clntSock.Send(buffer, len, 0) != len) {
		AfxMessageBox(L"Can't send msg");
	}

	clntSock.Close();
}

void CClientChatDoc::InitListener() {
	if (!listener.Create()) {
		AfxMessageBox(L"Can't create listener");
	}
}

void CClientChatDoc::receive(std::pair<CString, CString>& msg) {
	int len;
	char bufferFrom[256];
	char bufferContent[256];

	if (!listener.Listen()) {
		AfxMessageBox(L"Can't listen");
	}
	listener.Accept(receiver);

	AfxMessageBox(L"Connect");

	receiver.Receive(&len, 4, 0);
	receiver.Receive(&bufferFrom, len, 0);
	bufferFrom[len] = '\0';

	receiver.Receive(&len, 4, 0);
	receiver.Receive(&bufferContent, len, 0);
	bufferContent[len] = '\0';
		
	msg = std::make_pair(CString(bufferFrom), CString(bufferContent));
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
