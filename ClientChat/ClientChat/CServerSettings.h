#pragma once


class CServerSettings : public CDialog {
	DECLARE_DYNAMIC(CServerSettings)

	public:
		CServerSettings(CWnd* pParent = nullptr);   // standard constructor
		virtual ~CServerSettings();

	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ServerSettings };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		afx_msg void OnBtnClickConnect();
		afx_msg void OnBtnClickCancel();
		DECLARE_MESSAGE_MAP()

	public:
		DWORD serverIP;
		UINT serverPort;
};
