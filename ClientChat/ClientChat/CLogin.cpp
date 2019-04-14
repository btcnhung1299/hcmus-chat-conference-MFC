#include "stdafx.h"
#include "ClientChat.h"
#include "CLogin.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CLogin, CDialog)

CLogin::CLogin(CWnd* pParent /*=nullptr*/) : CDialog(IDD_Login, pParent) {
	OnBtnClickChooseLogin();
}

CLogin::~CLogin() {
}

void CLogin::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, chooseLogin, loginOption);
	DDX_Control(pDX, inpUsername, m_username);
	DDX_Control(pDX, inpPassword, m_password);
	DDX_Text(pDX, inpUsername, username);
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	ON_BN_CLICKED(btnOKLogin, &CLogin::OnBntClickOKLogin)
	ON_BN_CLICKED(btnCancelLogin, &CLogin::OnBntClickCancelLogin)

	ON_BN_CLICKED(chooseLogin, &CLogin::OnBtnClickChooseLogin)
	ON_BN_CLICKED(chooseRegister, &CLogin::OnBtnClickChooseRegister)
END_MESSAGE_MAP()


// CLogin message handlers
void CLogin::OnBntClickOKLogin() {
	m_username.GetWindowText(username);
	m_password.GetWindowText(password);
	EndDialog(btnOKLogin);
}

void CLogin::OnBntClickCancelLogin() {
	EndDialog(btnCancelLogin);
}

void CLogin::OnBtnClickChooseLogin() {
	loginOption = LoginType::LOGIN;
}

void CLogin::OnBtnClickChooseRegister() {
	loginOption = LoginType::REGISTER;
}
