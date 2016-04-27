#pragma once
// CReceiveSocket ÃüÁîÄ¿±ê

class CPlatformOfObserveAndControlDlg;

class CReceiveSocket : public CSocket
{
public:
	CPlatformOfObserveAndControlDlg* pDlg;
public:
	CReceiveSocket(CPlatformOfObserveAndControlDlg* pDlg);
	virtual ~CReceiveSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};


