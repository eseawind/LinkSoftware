// BroadPlaneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "BroadPlaneDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "SoftwareCommend.h"
#include "WinpCapHelper.h"
#include "Node.h"

// CBroadPlaneDlg �Ի���

IMPLEMENT_DYNAMIC(CBroadPlaneDlg, CDialogEx)

	CBroadPlaneDlg::CBroadPlaneDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBroadPlaneDlg::IDD, pParent)
{

}

CBroadPlaneDlg::~CBroadPlaneDlg()
{
}

void CBroadPlaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BroadPlane_Report_Receive, m_ListRecv);
	DDX_Control(pDX, IDC_LIST_BroadPlane_Report_Send, m_ListSend);
	DDX_Control(pDX, IDC_EDIT_BroadPlane_Report_Send, m_EditSending);
	DDX_Control(pDX, IDC_STATIC_BroadPlane_Show_RecvPic, m_StaticPic);
	DDX_Control(pDX, IDC_BUTTON_BroadPlane_Show_Path, m_EditPath);
	DDX_Control(pDX, IDC_PROGRESS_BroadPlane_Show_RecvPic, m_ProgressSending);
}


BEGIN_MESSAGE_MAP(CBroadPlaneDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_BroadPlane_Show_Browse, &CBroadPlaneDlg::OnBnClickedButtonBroadplaneShowBrowse)
	ON_BN_CLICKED(IDC_BUTTON_BroadPlane_Show_Send, &CBroadPlaneDlg::OnBnClickedButtonBroadplaneShowSend)
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CBroadPlaneDlg::OnUserRecvfromwinpcap)
END_MESSAGE_MAP()


// CBroadPlaneDlg ��Ϣ�������


void CBroadPlaneDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


BOOL CBroadPlaneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CBroadPlaneDlg::OnBnClickedButtonBroadplaneShowBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CBroadPlaneDlg::OnBnClickedButtonBroadplaneShowSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

afx_msg LRESULT CBroadPlaneDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
{
	unsigned char* data=(unsigned char*)lParam;
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)lParam;
	switch(header->function)
	{
	
	default:
		break;
	}
	xdeletes(data);
	return 0;
}
