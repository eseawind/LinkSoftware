// BroadGrdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "BroadGrdDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "SoftwareCommend.h"
#include "WinpCapHelper.h"
#include "Node.h"


// CBroadGrdDlg �Ի���

IMPLEMENT_DYNAMIC(CBroadGrdDlg, CDialogEx)

	CBroadGrdDlg::CBroadGrdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBroadGrdDlg::IDD, pParent)
{

}

CBroadGrdDlg::~CBroadGrdDlg()
{
}

void CBroadGrdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BroadGround_Report_Receive, m_ListRecv);
	DDX_Control(pDX, IDC_LIST_BroadGround_Report_Send, m_ListSend);
	DDX_Control(pDX, IDC_EDIT_BroadGround_Report_Send, m_EditSending);
	DDX_Control(pDX, IDC_STATIC_BroadGround_Show_RecvPic, m_StaticPic);
	DDX_Control(pDX, IDC_PROGRESS_BroadGround_Show_RecvPic, m_ProgressRecv);
}


BEGIN_MESSAGE_MAP(CBroadGrdDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_BroadGround_Show_Open, &CBroadGrdDlg::OnBnClickedButtonBroadgroundShowOpen)
END_MESSAGE_MAP()


// CBroadGrdDlg ��Ϣ�������


BOOL CBroadGrdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CBroadGrdDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


void CBroadGrdDlg::OnBnClickedButtonBroadgroundShowOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}
