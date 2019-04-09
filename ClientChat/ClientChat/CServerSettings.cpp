#include "stdafx.h"
#include "ClientChat.h"
#include "CServerSettings.h"
#include "afxdialogex.h"
#include <afxsock.h>

// CServerSettings dialog

IMPLEMENT_DYNAMIC(CServerSettings, CDialog)

CServerSettings::CServerSettings(CWnd* pParent /*=nullptr*/) : CDialog(IDD_ServerSettings, pParent)
{
}

CServerSettings::~CServerSettings() {
}

void CServerSettings::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, inpServerIP, m_serverIP);
	DDX_Control(pDX, inpServerPort, m_serverPort);
}


BEGIN_MESSAGE_MAP(CServerSettings, CDialog)
	ON_BN_CLICKED(btnCancel, &CServerSettings::OnBtnClickCancel)
	ON_BN_CLICKED(btnConnect, &CServerSettings::OnBtnClickConnect)

END_MESSAGE_MAP()

void CServerSettings::OnBtnClickCancel() {
	EndDialog(0);
}

void CServerSettings::OnBtnClickConnect() {
	CString bufferServerPort;
	m_serverPort.GetWindowText(bufferServerPort);
	serverPort = _ttoi(bufferServerPort);

	DWORD bufferServerIP;
	m_serverIP.GetAddress(bufferServerIP);
	serverIP.Format(L"%d", bufferServerIP);

	EndDialog(btnConnect);
}

UINT CServerSettings::GetServerPort() {
	return serverPort;
}

CString CServerSettings::GetServerIP() {
	return serverIP;
}