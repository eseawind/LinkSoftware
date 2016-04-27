// BroadGrdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "BroadGrdDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "SoftwareCommend.h"
#include "WinpCapHelper.h"
#include "Node.h"


// CBroadGrdDlg 对话框

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


// CBroadGrdDlg 消息处理程序


BOOL CBroadGrdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBroadGrdDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


void CBroadGrdDlg::OnBnClickedButtonBroadgroundShowOpen()
{
	// TODO: 在此添加控件通知处理程序代码

}
