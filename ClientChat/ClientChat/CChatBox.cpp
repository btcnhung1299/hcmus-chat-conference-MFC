#include "stdafx.h"
#include "ClientChat.h"
#include "CChatBox.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CChatBox, CDialog)

CChatBox::CChatBox(CWnd* pParent /*=nullptr*/) : CDialog(IDD_ChatBox, pParent) {
	titleCB = L"Empty";
}

CChatBox::~CChatBox() {
}

void CChatBox::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, titleChatBox, titleCB);
	DDX_Control(pDX, titleChatBox, m_titleCB);
}

BEGIN_MESSAGE_MAP(CChatBox, CDialog)
END_MESSAGE_MAP()