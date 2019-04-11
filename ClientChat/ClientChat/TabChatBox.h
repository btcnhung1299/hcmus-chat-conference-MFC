#pragma once
#include <afxcmn.h>
#include <vector>
#include "CCreateGroup.h"

class TabChatBox : public CTabCtrl {
	public:
		std::vector<int> m_chatBoxID;
		std::vector<CDialog> m_chatBoxDlg;

	public:
		TabChatBox();
		~TabChatBox();
		BOOL CreateNewGroup(CommonData& newGroupInfo);
		void InitChatBoxDlg();
		void ActivateChatBoxDlg();
};

