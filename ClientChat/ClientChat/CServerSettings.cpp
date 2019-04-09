#include "stdafx.h"
#include "ClientChat.h"
#include "CServerSettings.h"
#include "afxdialogex.h"
#include <afxsock.h>

// CServerSettings dialog

IMPLEMENT_DYNAMIC(CServerSettings, CDialog)

CServerSettings::CServerSettings(CWnd* pParent /*=nullptr*/) : CDialog(IDD_ServerSettings, pParent) {
}

CServerSettings::~CServerSettings() {
}

void CServerSettings::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, inpServerIP, m_serverIP);
	DDX_Control(pDX, inpServerPort, m_serverPort);
}


BEGIN_MESSAGE_MAP(CServerSettings, CDialog)
	ON_BN_CLICKED(btnConnect, &CServerSettings::OnBtnClickConnect)
	ON_BN_CLICKED(btnCancel, &CServerSettings::OnBtnClickCancel)


END_MESSAGE_MAP()


// CServerSettings message handlers
void CServerSettings::OnBtnClickConnect() {
	AfxSocketInit(NULL);
	CSocket clnt;

	if (!clnt.Create()) {
		AfxMessageBox(L"Can't create socket");
	}

	// Get server IP and port
	UINT serverPort = GetServerPort();
	CString serverIP = GetServerIP();

	if (!clnt.Connect(serverIP, serverPort)) {
		AfxMessageBox(L"Can't Connect");
	}
	else {
		EndDialog(1);
	}
}

void CServerSettings::OnBtnClickCancel() {
	EndDialog(0);
}

CString CServerSettings::GetServerIP() {
	DWORD bufferServerIP;
	m_serverIP.GetAddress(bufferServerIP);
	CString serverIP;
	serverIP.Format(L"%d", bufferServerIP);
	return serverIP;
}

UINT CServerSettings::GetServerPort() {
	CString bufferServerPort;
	m_serverPort.GetWindowText(bufferServerPort);
	UINT serverPort = _ttoi(bufferServerPort);
	return serverPort;
}