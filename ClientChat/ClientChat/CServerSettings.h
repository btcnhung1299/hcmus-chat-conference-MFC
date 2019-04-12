#pragma once


class CServerSettings : public CDialog {
	DECLARE_DYNAMIC(CServerSettings)
	private:
		CIPAddressCtrl m_serverIP;
		CEdit m_serverPort;
		CEdit m_myPort;

		UINT serverPort;
		UINT myPort;
		CString serverIP;

	public:
		CServerSettings(CWnd* pParent = nullptr);   // standard constructor
		virtual ~CServerSettings();
		
		UINT GetServerPort();
		UINT GetMyPort();
		CString GetServerIP();

	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ServerSettings };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		afx_msg void OnBtnClickCancel();
		afx_msg void OnBtnClickConnect();
		DECLARE_MESSAGE_MAP()
};
