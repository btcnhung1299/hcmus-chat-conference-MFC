#include "stdafx.h"
#include "ClientChat.h"
#include "CLogin.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CLogin, CDialog)

CLogin::CLogin(CWnd* pParent /*=nullptr*/) : CDialog(IDD_Login, pParent), m_loginOption(0)
{
}

CLogin::~CLogin() {
}

void CLogin::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, chooseLogin, m_loginOption);
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)

END_MESSAGE_MAP()


// CLogin message handlers
