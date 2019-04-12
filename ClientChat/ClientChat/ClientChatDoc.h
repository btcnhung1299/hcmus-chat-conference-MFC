#pragma once

#include "CServerSettings.h"
#include "CommonLib.h"
#include "CLogin.h"
#include "CNoti.h"
#include <utility>
#include <afxsock.h>
#define FILE_BUFFER_SIZE 102400

class CClientChatDoc : public CDocument {
	private:
		CSocket mainClntSock;
		CSocket clntSock, listenerConv, listenerUser, receiverConv, receiverUser;

		UINT serverPort;
		CString serverIP;
		UINT contactPort;
		UINT myPort;

		CString username;
		CString password;

		std::string onlineUsers;

	protected:
		CClientChatDoc() noexcept;
		DECLARE_DYNCREATE(CClientChatDoc)
		virtual ~CClientChatDoc();

	public:
		std::string GetInitOnlineUsers() { return onlineUsers; }
		void InitListenerConv();
		void InitListenerUser();

	public:
		virtual BOOL OnNewDocument();
		virtual void Serialize(CArchive& ar);
	#ifdef SHARED_HANDLERS
		virtual void InitializeSearchContent();
		virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
	#endif // SHARED_HANDLERS

	public:
		CString GetUsername() { return username; }
		BOOL Send(CommonData&, CommonData&);
		void ReceiveConv(CommonData& receiveData);
		void ReceiveUser();

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

