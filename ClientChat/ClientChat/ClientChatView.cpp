#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ClientChat.h"
#endif

#include "ClientChatDoc.h"
#include "ClientChatView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientChatView

IMPLEMENT_DYNCREATE(CClientChatView, CFormView)

BEGIN_MESSAGE_MAP(CClientChatView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CClientChatView construction/destruction

CClientChatView::CClientChatView() noexcept
: CFormView(IDD_CLIENTCHAT_FORM) {
	// TODO: add construction code here

}

CClientChatView::~CClientChatView() {
}

void CClientChatView::DoDataExchange(CDataExchange* pDX) {
	CFormView::DoDataExchange(pDX);
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

	CServerSettings serverSettingsDlg;
	if (serverSettingsDlg.DoModal() == btnConnect) {
		serverIP = serverSettingsDlg.serverIP;
	}
}

void CClientChatView::OnRButtonUp(UINT /* nFlags */, CPoint point) {
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CClientChatView::OnContextMenu(CWnd* /* pWnd */, CPoint point) {
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
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


// CClientChatView message handlers
