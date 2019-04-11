#pragma once

enum LoginType { LOGIN, REGISTER };

class CLogin : public CDialog {
	DECLARE_DYNAMIC(CLogin)
	public:
		// Control variables
		CEdit m_username;
		CEdit m_password;

		// Value variables
		int loginOption;
		CString username;
		CString password;

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
		afx_msg void OnBtnClickChooseLogin();
		afx_msg void OnBtnClickChooseRegister();
};
