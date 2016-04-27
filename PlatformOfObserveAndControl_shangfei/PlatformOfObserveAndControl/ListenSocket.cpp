// ListenSocket.cpp : 实现文件
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

// CListenSocket 成员函数
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
	// TODO: 在此添加专用代码和/或调用基类
	m_sRecvr=new CReceiveSocket(pDlg);
	Accept(*m_sRecvr);

	CSocket::OnAccept(nErrorCode);
}
