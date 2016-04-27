// AddLinksDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "AddLinksDlg.h"
#include "afxdialogex.h"


// CAddLinksDlg 对话框

IMPLEMENT_DYNAMIC(CAddLinksDlg, CDialogEx)

CAddLinksDlg::CAddLinksDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddLinksDlg::IDD, pParent)
{

}

CAddLinksDlg::~CAddLinksDlg()
{
}

void CAddLinksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboNode1);
	DDX_Control(pDX, IDC_COMBO3, m_ComboNode2);
	DDX_Control(pDX, IDC_COMBO2, m_ComboLinkType);
}


BEGIN_MESSAGE_MAP(CAddLinksDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_AddLinks_OK, &CAddLinksDlg::OnBnClickedButtonAddlinksOk)
END_MESSAGE_MAP()


// CAddLinksDlg 消息处理程序


void CAddLinksDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL CAddLinksDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	initNodes();
	m_ComboLinkType.SetCurSel(0);
	m_ComboNode1.SetCurSel(0);
	m_ComboNode2.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAddLinksDlg::initNodes()
{
	const CString notFound=_T("");
	const CString sectionNode=_T("NodeInfo");
	CFileFind finder;
	CString configPath;
	CString valueStr;
	CString comboItemStr=_T("");
	bool isFound=finder.FindFile(_T(".\\*.ini"));
	while(isFound)
	{
		comboItemStr=_T("");
		isFound=finder.FindNextFile();
		configPath=_T(".\\")+finder.GetFileName();
		if(configPath.Find(_T("config["))<0)
		{
			continue;
		}
		::GetPrivateProfileString(sectionNode,_T("LinkType"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		valueStr.ReleaseBuffer();
		if(valueStr!=notFound)
		{
			continue;
		}
		::GetPrivateProfileString(sectionNode,_T("DstNode"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		valueStr.ReleaseBuffer();
		if(valueStr!=notFound)
		{
			continue;
		}
		::GetPrivateProfileString(sectionNode,_T("NodeID"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		valueStr.ReleaseBuffer();
		comboItemStr+=_T("节点 ")+valueStr;
		::GetPrivateProfileString(sectionNode,_T("NodeType"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		valueStr.ReleaseBuffer();
		comboItemStr+=_T("(")+valueStr+_T(")");

		m_ComboNode1.AddString(comboItemStr);
		m_ComboNode2.AddString(comboItemStr);
		//int intfMACHex=GetPrivateProfileInt(sectionNode,_T("IntfMAC"),0,configPath);
		//valueStr.Format(_T("%012X"),intfMACHex);
		////::GetPrivateProfileString(sectionNode,_T("IntfMac"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		////valueStr.ReleaseBuffer();
		//m_ListNodes.SetItemText(count,2,valueStr);
	}
	if (m_ComboNode1.GetCount()==0)
	{
		m_ComboNode1.AddString(_T("无未配置节点"));
	}
	if (m_ComboNode2.GetCount()==0)
	{
		m_ComboNode2.AddString(_T("无未配置节点"));
	}
}

void CAddLinksDlg::OnBnClickedButtonAddlinksOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString node1Str,node2Str,linkType;
	CString nodeID1Str,nodeID2Str;
	m_ComboNode1.GetWindowText(node1Str);
	m_ComboNode2.GetWindowText(node2Str);
	m_ComboLinkType.GetWindowText(linkType);

	if(node1Str==_T("无未配置节点")||node2Str==_T("无未配置节点"))
	{
		SendMessage(WM_CLOSE);
		return;
	}

	int iBegin=node1Str.Find(_T(" "));
	int iEnd=node1Str.Find(_T("("));
	nodeID1Str=node1Str.Mid(iBegin+1,iEnd-iBegin-1);
	int node1=_ttoi(nodeID1Str);
	iBegin=node2Str.Find(_T(" "));
	iEnd=node2Str.Find(_T("("));
	nodeID2Str=node2Str.Mid(iBegin+1,iEnd-iBegin-1);
	int node2=_ttoi(nodeID2Str);

	if(node1==node2)
	{
		AfxMessageBox(_T("链路两端不能为同一节点,请重新设置"));
		return;
	}

	const CString sectionNode=_T("NodeInfo");
	CFileFind finder;
	CString configPath;
	//Node1
	configPath.Format(_T(".\\config[%d].ini"),node1);
	if(finder.FindFile(configPath))
	{
		::WritePrivateProfileString(sectionNode,_T("LinkType"),linkType,configPath);
		::WritePrivateProfileString(sectionNode,_T("DstNode"),nodeID2Str,configPath);
	}
	//Node2
	configPath.Format(_T(".\\config[%d].ini"),node2);
	if(finder.FindFile(configPath))
	{
		::WritePrivateProfileString(sectionNode,_T("LinkType"),linkType,configPath);
		::WritePrivateProfileString(sectionNode,_T("DstNode"),nodeID1Str,configPath);
	}


	::SendMessage(GetParent()->GetSafeHwnd(),WM_MY_RefreshNodeInfoList,NULL,NULL);
	SendMessage(WM_CLOSE);
}
