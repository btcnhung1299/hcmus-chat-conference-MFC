#pragma once

#include "CServerSettings.h"
#include "CommonLib.h"
#include "CLogin.h"
#include "CNoti.h"
#include <utility>
#include <afxsock.h>
#define FILE_BUFFER_SIZE 102400

class CClientChatDoc : public CDocument {
	public:
		CSocket mainClntSock;
		CSocket clntSock, receiverConv;

		UINT serverPort;
		CString serverIP;
		UINT contactPort;
		UINT myPort;

		CString username;
		CString password;

	protected:
		CClientChatDoc() noexcept;
		DECLARE_DYNCREATE(CClientChatDoc)
		virtual ~CClientChatDoc();

	public:
		void InitListener();

	public:
		virtual BOOL OnNewDocument();
		virtual void Serialize(CArchive& ar);
	#ifdef SHARED_HANDLERS
		virtual void InitializeSearchContent();
		virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
	#endif // SHARED_HANDLERS

	public:
		BOOL Send(CommonData&, CommonData&);
		void Receive(CommonData& receiveData);

	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

	protected:

	// Generated message map functions
	protected:
		DECLARE_MESSAGE_MAP()

	#ifdef SHARED_HANDLERS
		// Helper function that sets search content for a Search Handler
		void SetSearchContent(const CString& value);
	#endif // SHARED_HANDLERS
};

