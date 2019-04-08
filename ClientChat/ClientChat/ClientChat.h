#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"


class CClientChatApp : public CWinAppEx {
	public:
		CClientChatApp() noexcept;

	// Overrides
	public:
		virtual BOOL InitInstance();

	// Implementation
		UINT  m_nAppLook;
		BOOL  m_bHiColorIcons;

		virtual void PreLoadState();
		virtual void LoadCustomState();
		virtual void SaveCustomState();

		afx_msg void OnAppAbout();
		DECLARE_MESSAGE_MAP()
};

extern CClientChatApp theApp;
