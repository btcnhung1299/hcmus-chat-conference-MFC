// CNoti.cpp : implementation file
//

#include "stdafx.h"
#include "ClientChat.h"
#include "CNoti.h"
#include "afxdialogex.h"


// CNoti dialog

IMPLEMENT_DYNAMIC(CNoti, CDialog)

CNoti::CNoti(CWnd* pParent /*=nullptr*/) : CDialog(IDD_Noti, pParent), notiContent(_T("Default noti")) {
}

CNoti::CNoti(NotiType type, CWnd* pParent) : CDialog(IDD_Noti, pParent) {
	//CWnd *btn = GetDlgItem(btnAcceptNoti);

	switch (type) {
		case (NotiType::SUCCESS_REGISTER): {
			notiContent = _T("Registration successful.");
			break;
		}
		case (NotiType::DUPLICATE_REGISTER): {
			notiContent = _T("Username already exists.");
			break;
		}
		case (NotiType::SUCCESS_LOGIN): {
			notiContent = _T("Login successful.");
			break;
		}
		case (NotiType::NOTEXIST_LOGIN): {
			notiContent = _T("Wrong username/password.");
			break;
		}
		case (NotiType::SUCCESS_GROUP): {
			notiContent = _T("Create group successfully.");
			break;
		}
		case (NotiType::SUCCESS_SEND): {
			notiContent = _T("Upload successful.");
			break;
		}
		case (NotiType::FAIL_SEND): {
			notiContent = _T("Unable to upload your file.");
			break;
		}
		default: notiContent = _T("Not recognized"); break;
	}
}

CNoti::~CNoti() {
}

void CNoti::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, titleNoti, notiContent);
}


BEGIN_MESSAGE_MAP(CNoti, CDialog)
	ON_BN_CLICKED(btnAcceptNoti, &CNoti::OnBntClickAcceptNoti)
END_MESSAGE_MAP()


// CNoti message handlers

void CNoti::OnBntClickAcceptNoti() {
	EndDialog(btnAcceptNoti);
}
