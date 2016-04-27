// BroadPlaneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "BroadPlaneDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "Node.h"
#include "SoftwareCommend.h"
#include "WinpCapHelper.h"
#include "PhyHelper.h"

// CBroadPlaneDlg 对话框

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
END_MESSAGE_MAP()


// CBroadPlaneDlg 消息处理程序


void CBroadPlaneDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	caper->setMyHWnd(NULL,HWND_BroadPlane);
	pcieSender->setHwnd(NULL,HWND_BroadPlane);
	pcieReceiver->setHwnd(NULL,HWND_BroadPlane);
	CDialogEx::OnClose();
}


BOOL CBroadPlaneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	caper->setMyHWnd(GetSafeHwnd(),HWND_BroadPlane);
	caper->setMyAddr(node->mAddrHex);
	pcieSender->setHwnd(GetSafeHwnd(),HWND_BroadPlane);
	pcieReceiver->setHwnd(GetSafeHwnd(),HWND_BroadPlane);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBroadPlaneDlg::OnBnClickedButtonBroadplaneShowBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE); 
	fileDlg.m_ofn.lpstrTitle=_T("请选择数据源文件"); 
	fileDlg.m_ofn.lpstrFilter=_T("All Files(*.*)\0*.*\0\0");
	fileDlg.m_ofn.lpstrDefExt=_T("*.txt"); 
	if(IDOK==fileDlg.DoModal()) 
	{  
		CFile fileDataSrc;
		fileDataSrc.Open(fileDlg.GetFileName(),CFile::modeRead);
		CString filePathStr=fileDlg.GetPathName();//文件名+后缀
		m_EditPath.SetWindowText(filePathStr);
	} 
}


void CBroadPlaneDlg::OnBnClickedButtonBroadplaneShowSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filePath;
	m_EditPath.GetWindowText(filePath);
	pcieSender->sendFile(this,HWND_BroadGrd,filePath);
}
