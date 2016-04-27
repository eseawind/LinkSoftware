// InitNodeIDDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "InitNodeIDDlg.h"
#include "afxdialogex.h"


// CInitNodeIDDlg 对话框

IMPLEMENT_DYNAMIC(CInitNodeIDDlg, CDialogEx)

CInitNodeIDDlg::CInitNodeIDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInitNodeIDDlg::IDD, pParent)
{

}

CInitNodeIDDlg::~CInitNodeIDDlg()
{
}

void CInitNodeIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ConfigNodeID_NodeID, m_EditNodeID);
}


BEGIN_MESSAGE_MAP(CInitNodeIDDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ConfigNodeID_OK, &CInitNodeIDDlg::OnBnClickedButtonConfignodeidOk)
END_MESSAGE_MAP()


// CInitNodeIDDlg 消息处理程序


void CInitNodeIDDlg::OnBnClickedButtonConfignodeidOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString nodeIDStr;
	int nodeID;
	m_EditNodeID.GetWindowText(nodeIDStr);
	if(nodeIDStr!=_T(""))
	{
		nodeID=_ttoi(nodeIDStr);
		::SendMessage(GetParent()->GetSafeHwnd(),WM_USER_CONFIGNODEID,NULL,(LPARAM)&nodeID);
		SendMessage(WM_CLOSE);
	}
	else
	{
		AfxMessageBox(_T("请输入节点号"));
	}
	
}
