#include "stdafx.h"
#include "TabChatBox.h"
#include "Resource.h"

TabChatBox::TabChatBox() {
}


TabChatBox::~TabChatBox()
{
}

BOOL TabChatBox::CreateNewGroup(CommonData& newGroupInfo) {
	CCreateGroup createGroupDlg;
	if (createGroupDlg.DoModal() == btnCreateGroup) {
		newGroupInfo = createGroupDlg.newGroupInfo;
		return true;
	}
	return false;
}

void TabChatBox::InitChatBoxDlg() {

}

void TabChatBox::ActivateChatBoxDlg() {

}