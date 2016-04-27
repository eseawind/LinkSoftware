// ListenSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ListenSocket.h"
#include "ReceiveSocket.h"
#include "PlatformOfObserveAndControl.h"
#include "PlatformOfObserveAndControlDlg.h"


// CListenSocket

CListenSocket::CListenSocket(CPlatformOfObserveAndControlDlg* pDlg)
{
	this->pDlg=pDlg;
}

CListenSocket::~CListenSocket()
{

}

// CListenSocket ��Ա����
bool CListenSocket::openServer(UINT nHostPort)
{
	if(Create(nHostPort))
	{
		Listen();
		return true;
	}
	else
	{
		return false;
	}
}

void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_sRecvr=new CReceiveSocket(pDlg);
	Accept(*m_sRecvr);

	CSocket::OnAccept(nErrorCode);
}
