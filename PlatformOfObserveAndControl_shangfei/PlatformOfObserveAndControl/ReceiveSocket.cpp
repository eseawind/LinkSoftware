// ReceiveSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "ReceiveSocket.h"
#include "PlatformOfObserveAndControl.h"
#include "PlatformOfObserveAndControlDlg.h"


// CReceiveSocket

CReceiveSocket::CReceiveSocket(CPlatformOfObserveAndControlDlg* pDlg)
{
	this->pDlg=pDlg;
}

CReceiveSocket::~CReceiveSocket()
{

}


// CReceiveSocket 成员函数

void CReceiveSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	char buff[1024];
	Receive(buff,1024);
	pDlg->recvFromClient(buff);

	CSocket::OnReceive(nErrorCode);
}


void CReceiveSocket::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnSend(nErrorCode);
}
