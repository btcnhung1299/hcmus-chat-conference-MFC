#pragma once

#include "TabChatBox.h"

class CClientChatView : public CFormView {
	private:
		TabChatBox m_tabChatBox;

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
		UINT UpdateConversation();
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
		afx_msg void OnBtnClickNewTab();
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in ClientChatView.cpp
inline CClientChatDoc* CClientChatView::GetDocument() const
   { return reinterpret_cast<CClientChatDoc*>(m_pDocument); }
#endif

