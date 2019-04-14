#pragma once

enum NotiType { SUCCESS_REGISTER, DUPLICATE_REGISTER, SUCCESS_LOGIN, NOTEXIST_LOGIN, SUCCESS_GROUP, FAIL_GROUP, SUCCESS_SEND, FAIL_SEND, SUCCESS_RECEIVE, FAIL_RECEIVE };

class CNoti : public CDialog {
	DECLARE_DYNAMIC(CNoti)
	public:
		CString notiContent;

	public:
		CNoti(CWnd* pParent = nullptr);   // standard constructor
		CNoti(NotiType type, CWnd* pParent = nullptr);
		virtual ~CNoti();

	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_Noti };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBntClickAcceptNoti();
};
