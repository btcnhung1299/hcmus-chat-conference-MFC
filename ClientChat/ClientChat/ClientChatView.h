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

	protected:		// Constructor & Destructor
		CClientChatView() noexcept;
		virtual void OnInitialUpdate();
		virtual ~CClientChatView();
		DECLARE_DYNCREATE(CClientChatView)

	public:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		CClientChatDoc* GetDocument() const;

	protected:		// Event handlers
		virtual void DoDataExchange(CDataExchange* pDX);
		afx_msg void OnBtnClickCreateGroup();
		afx_msg void OnDBClickUser();
		afx_msg void OnSelChangeTabChatBox(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		DECLARE_MESSAGE_MAP()

	public:
	#ifdef AFX_DESIGN_TIME
		enum{ IDD = IDD_MainWnd };
	#endif

	public:			// Tabbing
		int OpenChatBox(CString chatBoxID, BoxType type);
		void UpdateChatBox(CommonData);
		void ShowTabNumber(int count);
		void UpdateActiveUsers(CommonData);
		void UpdateReceivedFiles(CommonData);
		
	public:			// Thread
		void UpdateOnView();
		static UINT ThreadUpdate(LPVOID Param);

	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif
};


#ifndef _DEBUG
inline CClientChatDoc* CClientChatView::GetDocument() const
   { return reinterpret_cast<CClientChatDoc*>(m_pDocument); }
#endif
