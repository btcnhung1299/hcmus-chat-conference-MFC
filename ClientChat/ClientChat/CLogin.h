#pragma once


// CLogin dialog

class CLogin : public CDialog {
	DECLARE_DYNAMIC(CLogin)

	public:
		CLogin(CWnd* pParent = nullptr);   // standard constructor
		virtual ~CLogin();

	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_Login };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		DECLARE_MESSAGE_MAP()

	public:
		afx_msg void OnBntClickChooseLogin();
		afx_msg void OnBntClickChooseRegister();
		int m_loginOption;
};
