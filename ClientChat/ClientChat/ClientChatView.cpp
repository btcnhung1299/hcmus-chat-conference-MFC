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

void CClientChatView::OnInitialUpdate() {
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//m_lstOnlineUsers.AddString(_T("jason"));
	//m_lstOnlineUsers.AddString(_T("emma"));
	//m_lstOnlineUsers.AddString(_T("henry"));

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

	/*if (!AfxBeginThread(ThreadUpdateOnlineUsers, reinterpret_cast<LPVOID>(this), THREAD_PRIORITY_NORMAL, 0, 0, NULL)) {
		AfxMessageBox(L"Failed creating thread to update online users");
	}
	*/
	if (!AfxBeginThread(ThreadUpdateConversation, reinterpret_cast<LPVOID>(this), THREAD_PRIORITY_NORMAL, 0, 0, NULL)) {
		AfxMessageBox(L"Failed creating thread to update conversation");
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
	int nSelUsers = m_lstOnlineUsers.GetSelCount();
	CArray<int, int> idSelUsers;
	idSelUsers.SetSize(nSelUsers);
	m_lstOnlineUsers.GetSelItems(nSelUsers, idSelUsers.GetData());

	CString bufferSelUsers;
	CommonData newGroupInfo;
	newGroupInfo.type = "cg";

	for (int i = 0; i < nSelUsers; i++) {
		m_lstOnlineUsers.GetText(idSelUsers.GetAt(i), bufferSelUsers);
		CT2CA selUsers(bufferSelUsers, CP_UTF8);
		newGroupInfo.message += std::string(selUsers) + "\n";
	}

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
	int userIndex = m_lstOnlineUsers.GetCurSel();
	CString bufferSelUser;
	m_lstOnlineUsers.GetText(userIndex, bufferSelUser);

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
	/*CChatBox *m_subChatBox[MAX_CB];
	for (int i = 0; i < MAX_CB; i++) {
		m_subChatBox[i] = &chatBox[i];

	}*/
	
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
	CString chatBoxID = CString(newMsg.to.c_str());
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

	chatBox[openID]->DisplayNewMsg(newMsg);
}

void CClientChatView::OnSelChangeTabChatBox(NMHDR *pNMHDR, LRESULT *pResult) {
	ShowTabNumber(m_tabChatBox.GetCurFocus());
	*pResult = 0;
}

// ------------------------- THREADS --------------------------------
UINT CClientChatView::ThreadUpdateOnlineUsers(LPVOID Param) {
	CClientChatView *pThis = reinterpret_cast<CClientChatView *>(Param);
	pThis->GetDocument()->InitListenerUser();
	BOOL end = false;

	while (!end) {

	}
	return 0;
}

void CClientChatView::UpdateOnlineUsersOnView() {
	//std::pair<CString, CString> msg;
	//GetDocument()->Receive(msg);
	//AfxMessageBox(L"abc");
}

UINT CClientChatView::ThreadUpdateConversation(LPVOID Param) {
	CClientChatView *pThis = reinterpret_cast<CClientChatView *>(Param);
	pThis->GetDocument()->InitListenerConv();
	BOOL end = false;
	
	while (!end) {
		pThis->UpdateConversationOnView();
	}

	return 0;
}

void CClientChatView::UpdateConversationOnView() {
	CommonData response;
	GetDocument()->ReceiveConv(response);
	
	if (response.type == "cg") {
		CString chatBoxID = CString(response.message.c_str());
		OpenChatBox(chatBoxID, BoxType::CHAT_GROUP);
	}
	else if (response.type == "mu" || response.type == "mg") {
		UpdateChatBox(response);
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

