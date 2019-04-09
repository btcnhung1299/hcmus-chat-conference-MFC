#pragma once


// CLogin dialog

class CLogin : public CDialog {
	DECLARE_DYNAMIC(CLogin)

	public:
		CLogin(CWnd* pParent = nullptr);   // standard constructor
		virtual ~CLogin();

		CString GetUsername() { return username; }
		CString GetPassword() { return password; }
		int GetLoginOption() { return loginOption; }

	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_Login };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		DECLARE_MESSAGE_MAP()

		

	public:
		/*afx_msg void OnBntClickChooseLogin();
		afx_msg void OnBntClickChooseRegister();*/
		afx_msg void OnBntClickOKLogin();
		afx_msg void OnBntClickCancelLogin();

		int loginOption;
		CString username;
		CString password;

		CEdit m_username;
		CEdit m_password;
		afx_msg void OnBtnClickChooseLogin();
		afx_msg void OnBtnClickChooseRegister();
};
