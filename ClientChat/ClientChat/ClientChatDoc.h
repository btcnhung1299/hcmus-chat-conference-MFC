#pragma once
#include "CServerSettings.h"
#include "CommonLib.h"
#include "CLogin.h"
#include "CNoti.h"
#include <utility>
#include <afxsock.h>

class CClientChatDoc : public CDocument {
	private:
		CSocket clntSock, listenerConv, listenerUser, receiverConv, receiverUser;

		UINT serverPort;
		CString serverIP;
		UINT contactPort;
		UINT receivePort;

		CString username;
		CString password;

	protected: 						// create from serialization only
		CClientChatDoc() noexcept;
		DECLARE_DYNCREATE(CClientChatDoc)

	// Attributes
	public:

	// Operations
	public:
		void InitListenerConv();
		void InitListenerUser();

	// Overrides
	public:
		virtual BOOL OnNewDocument();
		virtual void Serialize(CArchive& ar);
	#ifdef SHARED_HANDLERS
		virtual void InitializeSearchContent();
		virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
	#endif // SHARED_HANDLERS

	// Implementation
	public:
		virtual ~CClientChatDoc();
		
		BOOL Send(CommonData&, CommonData&);
		void ReceiveConv(std::pair<CString, CString>&);
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
