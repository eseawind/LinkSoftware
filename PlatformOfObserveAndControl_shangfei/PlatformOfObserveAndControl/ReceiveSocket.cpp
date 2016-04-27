// ReceiveSocket.cpp : ʵ���ļ�
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


// CReceiveSocket ��Ա����

void CReceiveSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	char buff[1024];
	Receive(buff,1024);
	pDlg->recvFromClient(buff);

	CSocket::OnReceive(nErrorCode);
}


void CReceiveSocket::OnSend(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CSocket::OnSend(nErrorCode);
}
