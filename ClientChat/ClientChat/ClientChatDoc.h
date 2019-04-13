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
		CSocket mainClntSock, receiver;
		CSocket clntSock;

		UINT serverPort, contactPort, myPort;
		CString serverIP;

		CString username, password;

	protected:			// Constructor and Destructor
		CClientChatDoc() noexcept;
		DECLARE_DYNCREATE(CClientChatDoc)
		DECLARE_MESSAGE_MAP()
		virtual ~CClientChatDoc();

	public:
		virtual BOOL OnNewDocument();
		virtual void Serialize(CArchive& ar);

	public:				// Thread
		void InitListener();
		void Receive(CommonData& receiveData);

	public:				// Send requests
		BOOL Send(CommonData&, CommonData&);


	#ifdef SHARED_HANDLERS
		virtual void InitializeSearchContent();
		virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
	#endif


	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

	#ifdef SHARED_HANDLERS
		// Helper function that sets search content for a Search Handler
		void SetSearchContent(const CString& value);
	#endif
};

