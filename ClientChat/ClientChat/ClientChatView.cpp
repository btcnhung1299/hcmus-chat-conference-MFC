#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ClientChat.h"
#endif

#include "ClientChatDoc.h"
#include "ClientChatView.h"
#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CClientChatView, CFormView)

// ------------------------- CONSTRUCTOR / DESTRUCTOR --------------------------
CClientChatView::CClientChatView()
noexcept
: CFormView(IDD_MainWnd) {
}

CClientChatView::~CClientChatView() {
}

BOOL CClientChatView::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}


// ------------------------- INITIAL SET UP ------------------------------------
void CClientChatView::OnInitialUpdate() {
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	CClientChatDoc *thisDoc = GetDocument();

	// Create main client socket
	if (!thisDoc->mainClntSock.Create()) {
		AfxMessageBox(L"Can't create main socket");
	}
	CString tmpAddr;
	thisDoc->mainClntSock.GetSockNameEx(tmpAddr, thisDoc->myPort);
	thisDoc->mainClntSock.Close();

	// Open new thread for update
	if (!AfxBeginThread(ThreadUpdate, reinterpret_cast<LPVOID>(this), THREAD_PRIORITY_NORMAL, 0, 0, NULL)) {
		AfxMessageBox(L"Failed creating thread to update conversation");
	}
	
	// Open login dialog
	CLogin loginDlg;
	CommonData loginInfo, response;
	CSocket clntSock;
	std::string onlineUsers;
	BOOL loginStatus = false;
	int loginOption = 0;

	while (!loginStatus) {
		if (loginDlg.DoModal() == btnCancelLogin) {
			break;
		}

		clntSock.Create();
		clntSock.Connect(thisDoc->serverIP, thisDoc->contactPort);

		// Save username to global variable for later use
		thisDoc->username = loginDlg.GetUsername();

		loginOption = loginDlg.GetLoginOption();
		CT2CA bufferUsername(thisDoc->username, CP_UTF8);
		CT2CA bufferPassword(loginDlg.GetPassword(), CP_UTF8);

		loginInfo.from = std::to_string(thisDoc->myPort);
		loginInfo.type = (loginOption == LoginType::LOGIN ? "li" : "re");
		loginInfo.fileSize = thisDoc->username.GetLength();
		loginInfo.message = std::string(bufferUsername) + std::string(bufferPassword);

		SendCommonData(clntSock, loginInfo);
		ReceiveCommonData(clntSock, response);

		if (loginOption == LoginType::REGISTER) {
			if (response.type == "suc") {
				CNoti notiSuccess(NotiType::SUCCESS_REGISTER);
				notiSuccess.DoModal();
			}
			else if (response.type == "dup") {
				CNoti notiDuplicate(NotiType::DUPLICATE_REGISTER);
				notiDuplicate.DoModal();
			}
			else {
				AfxMessageBox(L"Undefined registration response");
			}
		}
		else {
			if (response.type == "lisuc") {
				onlineUsers = response.message;
				CNoti notiSuccess(NotiType::SUCCESS_LOGIN);
				notiSuccess.DoModal();
				loginStatus = true;
			}
			else if (response.type == "fail") {
				CNoti notiFail(NotiType::NOTEXIST_LOGIN);
				notiFail.DoModal();
			}
			else {
				AfxMessageBox(L"Undefined login response");
			}
		}

		clntSock.Close();
	}

	// Determine size of sub tab
	CRect tabRect;
	m_tabChatBox.GetClientRect(&subTabRect);
	m_tabChatBox.AdjustRect(FALSE, &subTabRect);
	m_tabChatBox.GetWindowRect(&tabRect);
	ScreenToClient(tabRect);
	subTabRect.OffsetRect(tabRect.left, tabRect.top);
	subTabRect.top += 20;

	// Create tabs
	for (int i = 0; i < MAX_CB; i++) {
		chatBox[i] = new CChatBox(GetDocument(), this);
		chatBox[i]->Create(IDD_ChatBox, this);
		chatBoxOccupied[i] = false;
	}

	tabItem.mask = TCIF_TEXT;

	// Update users active in the beginning
	CString username = GetDocument()->username;
	CString bufferOnlineUser;

	int startPos = 0, endPos = 0;
	for (int i = 1; i < onlineUsers.length(); i++) {
		if (onlineUsers[i] == '\n') {
			endPos = i;
			bufferOnlineUser = onlineUsers.substr(startPos, endPos - startPos).c_str();
			if (bufferOnlineUser != username) {
				m_lstOnlineUsers.AddString(bufferOnlineUser);
			}
			startPos = endPos + 1;
		}
	}
}


// ------------------------- MESSAGE MAPPING ------------------------------------
BEGIN_MESSAGE_MAP(CClientChatView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(btnCreateNewGroup, &CClientChatView::OnBtnClickCreateGroup)
	ON_LBN_DBLCLK(lstOnlineUsers, &CClientChatView::OnDBClickUser)
	ON_NOTIFY(TCN_SELCHANGE, tabChatBox, &CClientChatView::OnSelChangeTabChatBox)
END_MESSAGE_MAP()

void CClientChatView::DoDataExchange(CDataExchange* pDX) {
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, lstOnlineUsers, m_lstOnlineUsers);
	DDX_Control(pDX, tabChatBox, m_tabChatBox);
}

// ------------------------- EVENT HANDLERS -------------------------------------
void CClientChatView::OnRButtonUp(UINT /* nFlags */, CPoint point) {
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CClientChatView::OnContextMenu(CWnd* /* pWnd */, CPoint point) {
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CClientChatView::OnBtnClickCreateGroup() {
	// Get list of selected users in active user list
	int nSelUsers = m_lstOnlineUsers.GetSelCount();
	CArray<int, int> idSelUsers;
	idSelUsers.SetSize(nSelUsers);
	m_lstOnlineUsers.GetSelItems(nSelUsers, idSelUsers.GetData());

	// Create group protocol: data.type = "cg", data.message = <username1>\n<username2>\n...<usernamen>\n
	CString bufferSelUsers;
	CommonData newGroupInfo;
	newGroupInfo.type = "cg";

	for (int i = 0; i < nSelUsers; i++) {
		m_lstOnlineUsers.GetText(idSelUsers.GetAt(i), bufferSelUsers);
		CT2CA selUsers(bufferSelUsers, CP_UTF8);
		newGroupInfo.message += std::string(selUsers) + "\n";
	}

	// Send create group message to server and get response
	CommonData serverResponse;
	if (GetDocument()->Send(newGroupInfo, serverResponse)) {
		CNoti notiSuccess(NotiType::SUCCESS_GROUP);
		notiSuccess.DoModal();
	}
	else {
		AfxMessageBox(L"Fail creating new group");
	}
}

void CClientChatView::OnDBClickUser() {
	// Get username selected when user double clicked on it
	int userIndex = m_lstOnlineUsers.GetCurSel();
	CString bufferSelUser;
	m_lstOnlineUsers.GetText(userIndex, bufferSelUser);

	// Open chat box with selected user (direct chat box)
	OpenChatBox(bufferSelUser, BoxType::CHAT_DIRECT);
}


// ------------------------- TAB HELPING ----------------------------------------
void CClientChatView::OpenChatBox(CString chatBoxID, BoxType boxType) {
	ChatBoxType type = NEW_TAB;
	int openID = -1, availableNewTab = -1;

	for (int i = 0; i < MAX_CB; i++) {
		CString temp = chatBox[i]->GetTitle();
		if (chatBoxOccupied[i]) {
			if (chatBox[i]->GetTitle() == chatBoxID) {
				type = TAB_EXISTED;
				openID = i;
				break;
			}
		}
		else {
			if (availableNewTab == -1) {
				availableNewTab = i;
			}
		}
	}

	if (type != TAB_EXISTED) {
		chatBox[availableNewTab]->SetTitle(chatBoxID);
		chatBox[availableNewTab]->SetType(boxType);
		tabItem.pszText = (LPWSTR)(LPCWSTR)(chatBoxID);
		m_tabChatBox.InsertItem(availableNewTab, &tabItem);

		openID = availableNewTab;
	}

	ShowTabNumber(openID);
}

void CClientChatView::ShowTabNumber(int count) {	
	for (int i = 0; i < MAX_CB; i++) {
		if (i != count) {
			chatBox[i]->ShowWindow(SW_HIDE);
			continue;
		}

		chatBox[i]->MoveWindow(&subTabRect);
		chatBox[i]->ShowWindow(SW_SHOW);
		m_tabChatBox.SetCurSel(count);
		chatBoxOccupied[count] = true;
	}
}

void CClientChatView::UpdateChatBox(CommonData newMsg) {
	// Get chatbox ID:
	// - Message to group: chatbox ID = data.to
	// - Direct message: chatbox ID = the another's ID
	CString chatBoxID(newMsg.to.c_str());
	if (newMsg.type == "mu") {
		if (chatBoxID == GetDocument()->username) {
			chatBoxID = newMsg.from.c_str();
		}
	}
	
	int curTab = m_tabChatBox.GetCurSel();
	ChatBoxType type = NEW_TAB;
	int openID = -1, availableNewTab = -1;

	for (int i = 0; i < MAX_CB; i++) {
		CString temp = chatBox[i]->GetTitle();
		if (chatBoxOccupied[i]) {
			if (chatBox[i]->GetTitle() == chatBoxID) {
				type = TAB_EXISTED;
				openID = i;
				break;
			}
		}
		else {
			if (availableNewTab == -1) {
				availableNewTab = i;
			}
		}
	}

	if (type != TAB_EXISTED) {
		chatBox[availableNewTab]->SetTitle(chatBoxID);
		chatBox[availableNewTab]->SetType(BoxType::CHAT_DIRECT);
		tabItem.pszText = (LPWSTR)(LPCWSTR)(chatBoxID);
		m_tabChatBox.InsertItem(availableNewTab, &tabItem);
		openID = availableNewTab;
	}

	if (curTab == -1) {
		ShowTabNumber(openID);
	}

	chatBox[openID]->DisplayNewMsg(newMsg);
}

void CClientChatView::OnSelChangeTabChatBox(NMHDR *pNMHDR, LRESULT *pResult) {
	ShowTabNumber(m_tabChatBox.GetCurFocus());
	*pResult = 0;
}


// ------------------------- THREADS --------------------------------
UINT CClientChatView::ThreadUpdate(LPVOID Param) {
	CClientChatView *pView = reinterpret_cast<CClientChatView *>(Param);
	pView->GetDocument()->InitListener();
	BOOL end = false;
	
	while (!end) {
		pView->UpdateOnView();
	}

	return 0;
}

void CClientChatView::UpdateOnView() {
	CommonData receiveData;
	GetDocument()->Receive(receiveData);
	
	if (receiveData.type == "newUserLogin") {
		CString newUsername(receiveData.message.c_str());
		this->m_lstOnlineUsers.AddString(newUsername);
	}
	else if (receiveData.type == "cg") {
		CString chatBoxID = CString(receiveData.message.c_str());
		OpenChatBox(chatBoxID, BoxType::CHAT_GROUP);
	}
	else if (receiveData.type == "mu" || receiveData.type == "mg") {
		UpdateChatBox(receiveData);
	}
}



#ifdef _DEBUG
void CClientChatView::AssertValid() const {
	CFormView::AssertValid();
}

void CClientChatView::Dump(CDumpContext& dc) const {
	CFormView::Dump(dc);
}

CClientChatDoc* CClientChatView::GetDocument() const {
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientChatDoc)));
	return (CClientChatDoc*)m_pDocument;
}
#endif //_DEBUG

