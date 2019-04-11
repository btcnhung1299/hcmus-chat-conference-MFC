// CCreateGroup.cpp : implementation file
//

#include "stdafx.h"
#include "ClientChat.h"
#include "CCreateGroup.h"
#include "afxdialogex.h"


// CCreateGroup dialog

IMPLEMENT_DYNAMIC(CCreateGroup, CDialog)

CCreateGroup::CCreateGroup(CWnd* pParent /*=nullptr*/) : CDialog(IDD_CreateGroup, pParent) {

}

CCreateGroup::~CCreateGroup() {
}

void CCreateGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, lstUsers, m_lstUsers);
}


BEGIN_MESSAGE_MAP(CCreateGroup, CDialog)
	ON_BN_CLICKED(btnCreateGroup, &CCreateGroup::OnBtnClickCreateGroup)
END_MESSAGE_MAP()


void CCreateGroup::OnBtnClickCreateGroup() {
	newGroupInfo.type = "cg";
	int nUsers = m_lstUsers.GetCount();
	CString bufferUsername;

	for (int i = 0; i < nUsers; i++) {
		bufferUsername = m_lstUsers.GetItemText(i);
		CT2CA username(bufferUsername, CP_UTF8);
		newGroupInfo.message += std::string(username) + "\n";
	}

	EndDialog(btnCreateGroup);
}
