#include "stdafx.h"
#include "ClientChat.h"
#include "CServerSettings.h"
#include "afxdialogex.h"
#include <afxsock.h>

// CServerSettings dialog

IMPLEMENT_DYNAMIC(CServerSettings, CDialog)

CServerSettings::CServerSettings(CWnd* pParent /*=nullptr*/) : CDialog(IDD_ServerSettings, pParent), serverPort(0)
{
}

CServerSettings::~CServerSettings() {
}

void CServerSettings::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_IPAddress(pDX, inpServerIP, serverIP);
	DDX_Text(pDX, inpServerPort, serverPort);
}


BEGIN_MESSAGE_MAP(CServerSettings, CDialog)
	ON_BN_CLICKED(btnConnect, &CServerSettings::OnBtnClickConnect)
	ON_BN_CLICKED(btnCancel, &CServerSettings::OnBtnClickCancel)


END_MESSAGE_MAP()


// CServerSettings message handlers
void CServerSettings::OnBtnClickConnect() {
	CSocket clnt;
	clnt.Create();

	CString cstrServerIP;
	cstrServerIP.Format(L"%d", serverIP);

	BOOL connectStatus = clnt.Connect(cstrServerIP, serverPort);
	if (!connectStatus) {
		AfxMessageBox(L"Can't connect");
	}
}

void CServerSettings::OnBtnClickCancel() {
	EndDialog(btnConnect);
}