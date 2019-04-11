#pragma once
#include "CommonLib.h"

class CCreateGroup : public CDialog {
	DECLARE_DYNAMIC(CCreateGroup)

	public:
		CommonData newGroupInfo;

	public:
		CCreateGroup(CWnd* pParent = nullptr);   // standard constructor
		virtual ~CCreateGroup();
		CommonData GetCreateGroupInfo() { return newGroupInfo; }

	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CreateGroup };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBtnClickCreateGroup();
		CVSListBox m_lstUsers;
};
