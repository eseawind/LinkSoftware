#pragma once
class CReceiveSocket;
class CPlatformOfObserveAndControlDlg;
// CListenSocket ����Ŀ��

class CListenSocket : public CSocket
{
public:
	CReceiveSocket* m_sRecvr;
	CPlatformOfObserveAndControlDlg* pDlg;
public:
	bool openServer(UINT nHostPort);
public:
	CListenSocket(CPlatformOfObserveAndControlDlg* pDlg);
	virtual ~CListenSocket();

	virtual void OnAccept(int nErrorCode);
};


