
// ControllerOnServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "ControllerOnServerDlg.h"
#include "afxdialogex.h"

#include "AddLinksDlg.h"
#include "Globe.h"
#include "InitialConfigDlg.h"
#include "Node.h"
#include "PlatformOfObserveAndControlDlg.h"
#include "SelectNetDeviceDlg.h"
#include "WinpCapHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CControllerOnServerDlg 对话框




CControllerOnServerDlg::CControllerOnServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControllerOnServerDlg::IDD, pParent)
	, numNodes(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	caper=new WinpCapHelper();
}

void CControllerOnServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NODES, m_ListNodes);
}

BEGIN_MESSAGE_MAP(CControllerOnServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_NodeAdd, &CControllerOnServerDlg::OnBnClickedNodeAdd)
	ON_MESSAGE(WM_MY_RefreshNodeInfoList, &CControllerOnServerDlg::OnMyRefreshnodeinfolist)
//	ON_BN_CLICKED(IDC_BUTTON_Run, &CControllerOnServerDlg::OnBnClickedButtonRun)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CControllerOnServerDlg::OnItemdblclickListNodes)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_NODES, &CControllerOnServerDlg::OnDblclkListNodes)
	ON_BN_CLICKED(IDC_BUTTON_Run, &CControllerOnServerDlg::OnBnClickedButtonRun)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_SELECTEDNETDEVICE, &CControllerOnServerDlg::OnUserSelectednetdevice)
	ON_BN_CLICKED(IDC_BUTTON_LinkAdd, &CControllerOnServerDlg::OnBnClickedButtonLinkadd)
	ON_BN_CLICKED(IDC_BUTTON_SceneConfig, &CControllerOnServerDlg::OnBnClickedButtonSceneconfig)
END_MESSAGE_MAP()


// CControllerOnServerDlg 消息处理程序

BOOL CControllerOnServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CSelectNetDeviceDlg* pDlg=new CSelectNetDeviceDlg();
	pDlg->devID=&devID;
	pDlg->Create(IDD_SelectNetDevice_DIALOG,this);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->CenterWindow(this);

	initListCtrls();
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CControllerOnServerDlg::initListCtrls()
{
	m_ListNodes.SetExtendedStyle(m_ListNodes.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	m_ListNodes.DeleteAllItems();

	if(m_ListNodes.GetHeaderCtrl()->GetItemCount()==0)
	{
		m_ListNodes.InsertColumn(0,_T("节点ID"),LVCFMT_LEFT,50);
		m_ListNodes.InsertColumn(1,_T("节点类型"),LVCFMT_LEFT,80);
		m_ListNodes.InsertColumn(2,_T("链路类型"),LVCFMT_LEFT,80);
		m_ListNodes.InsertColumn(3,_T("目的节点"),LVCFMT_LEFT,80);
	}

	const CString notFound=_T("");
	const CString sectionNode=_T("NodeInfo");
	CFileFind finder;
	CString configPath;
	CString valueStr;
	int count=0;

	
	//for(configPath.Format(_T(".\\config[%d].ini"),count);
	//	finder.FindFile(configPath);
	//	)
	bool isFound=finder.FindFile(_T(".\\*.ini"));
	while(isFound)
	{
		isFound=finder.FindNextFile();
		configPath=_T(".\\")+finder.GetFileName();
		if(configPath.Find(_T("config["))<0)
		{
			continue;
		}
		::GetPrivateProfileString(sectionNode,_T("NodeID"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		valueStr.ReleaseBuffer();
		m_ListNodes.InsertItem(count,valueStr);

		::GetPrivateProfileString(sectionNode,_T("NodeType"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		valueStr.ReleaseBuffer();
		m_ListNodes.SetItemText(count,1,valueStr);

		::GetPrivateProfileString(sectionNode,_T("LinkType"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		valueStr.ReleaseBuffer();
		m_ListNodes.SetItemText(count,2,valueStr);

		::GetPrivateProfileString(sectionNode,_T("DstNode"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		valueStr.ReleaseBuffer();
		m_ListNodes.SetItemText(count,3,valueStr);

		//int intfMACHex=GetPrivateProfileInt(sectionNode,_T("IntfMAC"),0,configPath);
		//valueStr.Format(_T("%012X"),intfMACHex);
		////::GetPrivateProfileString(sectionNode,_T("IntfMac"),notFound,valueStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
		////valueStr.ReleaseBuffer();
		//m_ListNodes.SetItemText(count,2,valueStr);
		count++;
	}
	numNodes=count;
}

void CControllerOnServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CControllerOnServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CControllerOnServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CControllerOnServerDlg::OnBnClickedNodeAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CInitialConfigDlg* pDlg=new CInitialConfigDlg;
	pDlg->numNodes=numNodes;
	pDlg->Create(IDD_NodeConfig_DIALOG,this);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->CenterWindow(this);
}


afx_msg LRESULT CControllerOnServerDlg::OnMyRefreshnodeinfolist(WPARAM wParam, LPARAM lParam)
{
	initListCtrls();
	return 0;
}

void CControllerOnServerDlg::OnItemdblclickListNodes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CControllerOnServerDlg::OnDblclkListNodes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int rowIndex=pNMItemActivate->iItem;
	CString nodeIDStr=m_ListNodes.GetItemText(rowIndex,0);

	CPlatformOfObserveAndControlDlg* pDlg=new CPlatformOfObserveAndControlDlg;
	pDlg->node->nodeID=_ttoi(nodeIDStr);
	pDlg->devID=devID;
	pDlg->Create(IDD_PLATFORMOFOBSERVEANDCONTROL_DIALOG,this);
	pDlg->ShowWindow(SW_SHOW);

	*pResult = 0;
}


void CControllerOnServerDlg::OnBnClickedButtonRun()
{
	// TODO: 在此添加控件通知处理程序代码
	const int sysMetricsX=GetSystemMetrics(SM_CXFULLSCREEN);
	const int sysMetricsY=GetSystemMetrics(SM_CYFULLSCREEN);
	for (int i=0;i<m_ListNodes.GetItemCount();i++)
	{
		CString nodeIDStr=m_ListNodes.GetItemText(i,0);

		CPlatformOfObserveAndControlDlg* pDlg=new CPlatformOfObserveAndControlDlg;
		pDlg->node->nodeID=_ttoi(nodeIDStr);
		pDlg->devID=devID;
		pDlg->Create(IDD_PLATFORMOFOBSERVEANDCONTROL_DIALOG,NULL);
		pDlg->SetWindowPos(NULL,(i%2)*(sysMetricsX/2),(i/2)*(sysMetricsY/2),0,0,SWP_NOSIZE|SWP_NOZORDER);
		pDlg->ShowWindow(SW_SHOW);

	}
	//SendMessage(SW_HIDE);
}


void CControllerOnServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	xdelete(caper);
	CDialogEx::OnClose();
}


afx_msg LRESULT CControllerOnServerDlg::OnUserSelectednetdevice(WPARAM wParam, LPARAM lParam)
{
	int netDevID=*(int*)wParam;
	caper->setMyAddr(0);
	caper->openAdapter(netDevID);
	caper->beginCatching();
	return 0;
}


void CControllerOnServerDlg::OnBnClickedButtonLinkadd()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddLinksDlg* pDlg=new CAddLinksDlg;
	pDlg->Create(IDD_DIALOG_AddLinks,this);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->CenterWindow(this);

}


void CControllerOnServerDlg::OnBnClickedButtonSceneconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("环境参数保存成功！"));
}
