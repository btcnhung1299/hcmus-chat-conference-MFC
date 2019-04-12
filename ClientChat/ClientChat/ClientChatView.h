#pragma once

#include "CChatBox.h"
#define MAX_CB 5

enum ChatBoxType { TAB_EXISTED, NEW_TAB };

class CClientChatView : public CFormView {
	public:
		// Control variables
		CListBox m_lstOnlineUsers;
		CTabCtrl m_tabChatBox;

		// Tab variables
		CChatBox *chatBox[MAX_CB];		// Tab dialog
		bool chatBoxOccupied[MAX_CB];	// Tab used
		TCITEM tabItem;					// Tab bar
		CRect subTabRect;				// Tab size

	protected: 
		CClientChatView() noexcept;
		virtual ~CClientChatView();
		DECLARE_DYNCREATE(CClientChatView)

	public:
	#ifdef AFX_DESIGN_TIME
		enum{ IDD = IDD_MainWnd };
	#endif

	public:
		CClientChatDoc* GetDocument() const;

	public:
		void OpenChatBox(CString chatBoxID, BoxType type);
		void UpdateChatBox(CommonData);
		void ShowTabNumber(int count);
		void UpdateOnlineUsersOnView();
		void UpdateConversationOnView();
		static UINT ThreadUpdateOnlineUsers(LPVOID Param);
		static UINT ThreadUpdateConversation(LPVOID Param);

	public:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual void OnInitialUpdate();
		

	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

	protected:

	// Generated message map functions
	protected:
		afx_msg void OnBtnClickCreateGroup();
		afx_msg void OnDBClickUser();
		afx_msg void OnSelChangeTabChatBox(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG
inline CClientChatDoc* CClientChatView::GetDocument() const
   { return reinterpret_cast<CClientChatDoc*>(m_pDocument); }
#endif
