#include "stdafx.h"
#include "ClientChat.h"
#include "CServerSettings.h"
#include "afxdialogex.h"


// CServerSettings dialog

IMPLEMENT_DYNAMIC(CServerSettings, CDialog)

CServerSettings::CServerSettings(CWnd* pParent /*=nullptr*/) : CDialog(IDD_ServerSettings, pParent) {
}

CServerSettings::~CServerSettings() {
}

void CServerSettings::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_IPAddress(pDX, inpServerIP, serverIP);
}


BEGIN_MESSAGE_MAP(CServerSettings, CDialog)
	ON_BN_CLICKED(btnConnect, &CServerSettings::OnBtnClickConnect)
	ON_BN_CLICKED(btnCancel, &CServerSettings::OnBtnClickCancel)


END_MESSAGE_MAP()


// CServerSettings message handlers
void CServerSettings::OnBtnClickConnect() {

}

void CServerSettings::OnBtnClickCancel() {
	EndDialog(btnConnect);
}