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


// CClientChatView

IMPLEMENT_DYNCREATE(CClientChatView, CFormView)

BEGIN_MESSAGE_MAP(CClientChatView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(btnCreateNewGroup, &CClientChatView::OnBtnClickCreateGroup)
END_MESSAGE_MAP()

// CClientChatView construction/destruction

CClientChatView::CClientChatView()
noexcept
: CFormView(IDD_MainWnd) {
}

CClientChatView::~CClientChatView() {
}


void CClientChatView::DoDataExchange(CDataExchange* pDX) {
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, lstOnlineUsers, m_lstOnlineUsers);
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

	m_lstOnlineUsers.AddString(_T("jason"));
	m_lstOnlineUsers.AddString(_T("emma"));
	m_lstOnlineUsers.AddString(_T("henry"));
	m_lstOnlineUsers.AddString(_T("damien"));
	m_lstOnlineUsers.AddString(_T("lara"));

	/*if (!AfxBeginThread(ThreadUpdateOnlineUsers, reinterpret_cast<LPVOID>(this), THREAD_PRIORITY_NORMAL, 0, 0, NULL)) {
		AfxMessageBox(L"Failed creating thread to update online users");
	}
	
	if (!AfxBeginThread(ThreadUpdateConversation, reinterpret_cast<LPVOID>(this), THREAD_PRIORITY_NORMAL, 0, 0, NULL)) {
		AfxMessageBox(L"Failed creating thread to update conversation");
	}*/
}


// ------------------------- THREADS --------------------------------
UINT CClientChatView::ThreadUpdateOnlineUsers(LPVOID Param) {
	CClientChatView *pThis = reinterpret_cast<CClientChatView *>(Param);
//	pThis->GetDocument()->InitListenerUser();
	BOOL end = false;

	while (!end) {

	}
	return 0;
}

UINT CClientChatView::UpdateOnlineUsersOnView() {
	//std::pair<CString, CString> msg;
	//GetDocument()->Receive(msg);
	//AfxMessageBox(L"abc");
	return 0;
}

UINT CClientChatView::ThreadUpdateConversation(LPVOID Param) {
	CClientChatView *pThis = reinterpret_cast<CClientChatView *>(Param);
//	pThis->GetDocument()->InitListenerConv();
	BOOL end = false;
	
	while (!end) {
		//pThis->UpdateConversation();
	}
	return 0;
}

UINT CClientChatView::UpdateConversationOnView() {
	//std::pair<CString, CString> msg;
	//GetDocument()->Receive(msg);
	//AfxMessageBox(L"abc");
	return 0;
}

// --------------------- EVENT HANDLERS ---------------------------
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
	CString newGroupID;
	if (GetDocument()->Send(newGroupInfo, serverResponse)) {
		AfxMessageBox(L"Create group successfully");
		newGroupID = CString(serverResponse.message.c_str());
	}
	else {
		AfxMessageBox(L"Fail creating new group");
	}
}


// CClientChatView diagnostics

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


