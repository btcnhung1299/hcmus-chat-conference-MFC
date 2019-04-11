#pragma once


class CClientChatView : public CFormView {
	public:
		CListBox m_lstOnlineUsers;

	protected: 									// create from serialization only
		CClientChatView() noexcept;
		DECLARE_DYNCREATE(CClientChatView)

	public:
	#ifdef AFX_DESIGN_TIME
		enum{ IDD = IDD_MainWnd };
	#endif

	// Attributes
	public:
		CClientChatDoc* GetDocument() const;

	// Operations
	public:
		UINT UpdateOnlineUsersOnView();
		UINT UpdateConversationOnView();
		static UINT ThreadUpdateOnlineUsers(LPVOID Param);
		static UINT ThreadUpdateConversation(LPVOID Param);

	// Overrides
	public:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual void OnInitialUpdate(); 		// called first time after construct

	// Implementation
	public:
		virtual ~CClientChatView();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

	protected:

	// Generated message map functions
	protected:
		afx_msg void OnBtnClickCreateGroup();
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		DECLARE_MESSAGE_MAP()
	
public:
	
};

#ifndef _DEBUG  // debug version in ClientChatView.cpp
inline CClientChatDoc* CClientChatView::GetDocument() const
   { return reinterpret_cast<CClientChatDoc*>(m_pDocument); }
#endif

