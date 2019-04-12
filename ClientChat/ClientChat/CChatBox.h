#pragma once

class CChatBox : public CDialog {
	DECLARE_DYNAMIC(CChatBox)

	public:
		CStatic m_titleCB;
		CString titleCB;

	public:
		CChatBox(CWnd* pParent = nullptr);   // standard constructor
		virtual ~CChatBox();

	public:
		CString GetTitle() { return titleCB; }
		void SetTitle(CString newTitleCB) { titleCB = newTitleCB; m_titleCB.SetWindowText(titleCB);  }

	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ChatBox };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		DECLARE_MESSAGE_MAP()
};
