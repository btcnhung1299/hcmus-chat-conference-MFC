#include "stdafx.h"
#include "ClientChat.h"
#include "CChatBox.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CChatBox, CDialog)

CChatBox::CChatBox(CClientChatDoc* doc, CWnd* pParent) : CDialog(IDD_ChatBox, pParent) {
	titleCB = L"Empty";
	type = BoxType::CHAT_DIRECT;
	p_Document = doc;
}

CChatBox::~CChatBox() {
}

void CChatBox::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, titleChatBox, titleCB);
	DDX_Control(pDX, titleChatBox, m_titleCB);
	DDX_Control(pDX, inputMsg, m_inputMsg);
	DDX_Control(pDX, outputConversation, m_outputConversation);
}

BEGIN_MESSAGE_MAP(CChatBox, CDialog)
	ON_BN_CLICKED(btnSendMsg, &CChatBox::OnBtnClickSendMsg)
	ON_BN_CLICKED(btnUpload, &CChatBox::OnBtnClickUpload)
END_MESSAGE_MAP()

void CChatBox::OnBtnClickSendMsg() {
	CString bufferMsg;
	m_inputMsg.GetWindowText(bufferMsg);

	CT2CA msg(bufferMsg, CP_UTF8);
	CT2CA to(titleCB, CP_UTF8);

	sendMsg.message = std::string(msg);
	sendMsg.to = std::string(to);
	sendMsg.type = (type == BoxType::CHAT_GROUP ? "mg" : "mu");

	CommonData response;
	p_Document->Send(sendMsg, response);

	m_inputMsg.SetWindowText(_T(""));
}

void CChatBox::DisplayNewMsg(CommonData& newMsg) {
	CString userFrom = CString(newMsg.from.c_str());
	CString content = CString(newMsg.message.c_str());

	int curPos = m_outputConversation.GetTextLengthEx(GTL_NUMCHARS);
	m_outputConversation.SetSel(curPos, curPos + 1);
	m_outputConversation.ReplaceSel(userFrom + _T(": ") + content + _T("\r\n"));

	CHARFORMAT2 userFormat;
	userFormat.cbSize = sizeof(CHARFORMAT2);
	userFormat.dwMask = CFM_BOLD;
	userFormat.dwEffects = CFE_BOLD;
	m_outputConversation.SetSel(curPos, curPos + userFrom.GetLength() + 1);
	m_outputConversation.SetSelectionCharFormat(userFormat);
}

void CChatBox::OnBtnClickUpload() {
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this);
	fileDlg.m_pOFN->lpstrTitle = _T("Choose file...");
	
	CommonData fileInfo, response;
	CString pathName;

	if (fileDlg.DoModal() == IDOK) {
		pathName = fileDlg.GetPathName();
		fileInfo.type = (type == BoxType::CHAT_DIRECT ? "fu" : "fg");
		fileInfo.message = std::string(CT2CA(pathName, CP_UTF8));
		fileInfo.to = std::string(CT2CA(titleCB, CP_UTF8));
		
		// If file metadata received by server, send actual file
		if (p_Document->Send(fileInfo, response)) {
			AfxMessageBox(L"Send!!");
		}
	}
}