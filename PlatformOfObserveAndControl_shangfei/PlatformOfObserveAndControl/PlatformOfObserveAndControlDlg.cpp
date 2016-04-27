
// PlatformOfObserveAndControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "PlatformOfObserveAndControlDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "SoftwareCommend.h"
#include "StatisticsDlg.h"
#include "MasterControlDlg.h"
#include "ControlledDlg.h"
#include "Node.h"
#include "WinpCapHelper.h"
#include "PhyHelper.h"
#include "SelectNetDeviceDlg.h"
#include "InitNodeIDDlg.h"
#include "MasterBroadDlg.h"
#include "ControlledBroadDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ProtocolStatistics::ProtocolStatistics()
{
	curr_time=0;
	last_time=time(NULL);
	init_time=last_time;
	memset(paramValue,0,sizeof(int)*8);
}
void ProtocolStatistics::setNetworkType(int isBroad)
{
	if(isBroad==1)
	{
		pktRate=10000;
	}
	else if(isBroad==0)
	{
		pktRate=1000;
	}
}
void ProtocolStatistics::update()
{
	float rand0;
	float faverage;
	float froot;

	curr_time = time(NULL);
	printf("curr_time =%u \n",curr_time);
	srand(curr_time);
	rand0 = rand();
	froot = rand0/RAND_MAX; // the gen root of this statistics
	if(froot < 0.001)
		paramValue[0] +=curr_time - last_time;
	else 
		paramValue[0] +=froot*pktRate*(curr_time - last_time); // gen pkts num						
						
	paramValue[1] = paramValue[0]; // send pkts num

	rand0 = rand();
	froot = rand0/RAND_MAX; // the gen root of this statistics
	if(froot < 0.001)
		paramValue[2] +=curr_time - last_time;
	else paramValue[2] +=froot*pktRate*(curr_time - last_time); // recv pkts num
						
	faverage = (paramValue[1] + paramValue[2])/(curr_time-init_time);
	paramValue[3] = faverage;  // averrage throught
						
	faverage = paramValue[0]/(curr_time-last_time);
	paramValue[4] = faverage;  // averrage gen pkts rate

	rand0 = rand();
	froot = rand0/RAND_MAX;
	while(froot > 0.01)
		froot -=0.01;      // e2e delay must less than 0.01
						
	paramValue[5] = froot; // e2e delay

	rand0 = rand();
	froot = rand0/RAND_MAX;
	while(froot > 0.001)
		froot -=0.001;      // e2e delay must less than 0.001
						
	paramValue[6] = froot; //delay jitter
						
	faverage = (paramValue[0] - paramValue[1])/paramValue[0];
	paramValue[7] = faverage;   // average pkts loss

	last_time=curr_time;
}
ProtocolStatistics stat;
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
protected:
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


// CPlatformOfObserveAndControlDlg 对话框

CPlatformOfObserveAndControlDlg::CPlatformOfObserveAndControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlatformOfObserveAndControlDlg::IDD, pParent)
{
	isSelfTesting=false;
	pDlg=NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	node=new Node;
	serverCaper=new WinpCapHelper();
	protocolCaper=new WinpCapHelper();
	pcieSender=new PhyHelper(Dir_Down);
	pcieReceiver=new PhyHelper(Dir_Up);
	m_hEventQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
}
CPlatformOfObserveAndControlDlg::~CPlatformOfObserveAndControlDlg()
{
	xdelete(node);
	xdelete(serverCaper);
	xdelete(protocolCaper);
	xdelete(pcieSender);
	xdelete(pcieReceiver);
}
void CPlatformOfObserveAndControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Main_Business_Sel, mCombo_businessSel);
	DDX_Control(pDX, IDC_BUTTON_Main_Business_Start, mBtn_businessStart);
	DDX_Control(pDX, IDC_BUTTON_Main_Business_EditConfig, mBtn_changeConfig);
	DDX_Control(pDX, IDC_COMBO_Main_ObsCtl_LayerSel, mCombo_layerSel);
	DDX_Control(pDX, IDC_COMBO_Main_ObsCtl_SpotSel, mCombo_spotSel);
	DDX_Control(pDX, IDC_COMBO_Main_ObsCtl_FuncSel, mCombo_funcSel);
	DDX_Control(pDX, IDC_EDIT_Main_ObsCtl_DataSrcPath, mEdit_dataSrcPath);
	DDX_Control(pDX, IDC_EDIT_Main_ObsCtl_Mode_Multi_Num, mEdit_numModeMulti);
	DDX_Control(pDX, IDC_BUTTON_Main_ObsCtl_Browse, mBtn_browse);
	DDX_Control(pDX, IDC_BUTTON_Main_ObsCtl_Apply, mBtn_applySettings);
	DDX_Control(pDX, IDC_STATIC_Main_SocketState, mStatic_State);
	DDX_Control(pDX, IDC_COMBO_Main_Baseband_ChannelEncode, m_ComboChannelEncode);
	DDX_Control(pDX, IDC_COMBO_Main_Baseband_SpreadAlgo, m_ComboSpreadAlgo);
	DDX_Control(pDX, IDC_COMBO_Main_Baseband_ModMode, m_ComboModMode);
	DDX_Control(pDX, IDC_COMBO_Main_Baseband_Custom, m_ComboBasebandCustom);
	DDX_Control(pDX, IDC_COMBO_Main_Link_FrameFormat, m_ComboFrameFormat);
	DDX_Control(pDX, IDC_COMBO_Main_Link_LinkManageAlgo, m_ComboLinkManage);
	DDX_Control(pDX, IDC_COMBO_Main_Link_RsrcDist, m_ComboRsrcDist);
	DDX_Control(pDX, IDC_COMBO_Main_Link_Custom, m_ComboLinkCustom);
	DDX_Control(pDX, IDC_COMBO_Main_Intervene_Protocol_Param, m_ComboInterveneProtocolParam);
	DDX_Control(pDX, IDC_COMBO_Main_Intervene_Protocol_Value, m_ComboInterveneProtocolValue);
	DDX_Control(pDX, IDC_COMBO_Main_Intervene_Baseband_Param, m_ComboInterveneBasebandParam);
	DDX_Control(pDX, IDC_COMBO_Main_Intervene_Bandband_Value, m_ComboInterveneBasebandValue);
}

BEGIN_MESSAGE_MAP(CPlatformOfObserveAndControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Main_ObsCtl_Browse, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainObsctlBrowse)
	ON_BN_CLICKED(IDC_BUTTON_Main_Business_EditConfig, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainBusinessEditconfig)
	ON_BN_CLICKED(IDC_BUTTON_Main_NodeInfo_Refresh, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainNodeinfoRefresh)
	ON_BN_CLICKED(IDC_BUTTON__Main_Statistic_OK, &CPlatformOfObserveAndControlDlg::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON_Main_Business_Start, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainBusinessStart)
	ON_BN_CLICKED(IDC_BUTTON_Main_ObsCtl_Apply, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainObsctlApply)
	ON_BN_CLICKED(IDC_BUTTON_Main_Recontruction_Baseband_Run, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainRecontructionBasebandRun)
	ON_BN_CLICKED(IDC_BUTTON_Main_Recontruction_Link_Run, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainRecontructionLinkRun)
	ON_BN_CLICKED(IDC_BUTTON_Main_Intervene_Protocol_Run, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainInterveneProtocolRun)
	ON_BN_CLICKED(IDC_BUTTON_Main_Intervene_Baseband_Run, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainInterveneBasebandRun)
	ON_MESSAGE(WM_USER_HAVESELECTEDDEVICE, &CPlatformOfObserveAndControlDlg::OnUserHaveselecteddevice)
	ON_MESSAGE(WM_USER_CONFIGNODEID, &CPlatformOfObserveAndControlDlg::OnUserConfignodeid)
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CPlatformOfObserveAndControlDlg::OnUserRecvfromwinpcap)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_RECVFROMPCIE, &CPlatformOfObserveAndControlDlg::OnUserRecvfrompcie)
	ON_BN_CLICKED(IDC_BUTTON_Reset, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CPlatformOfObserveAndControlDlg 消息处理程序

BOOL CPlatformOfObserveAndControlDlg::OnInitDialog()
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
	initNodeInfo();
	m_ComboChannelEncode.SetCurSel(0);
	m_ComboSpreadAlgo.SetCurSel(0);
	m_ComboModMode.SetCurSel(0);
	m_ComboBasebandCustom.SetCurSel(0);
	m_ComboFrameFormat.SetCurSel(0);
	m_ComboLinkManage.SetCurSel(0);
	m_ComboRsrcDist.SetCurSel(0);
	m_ComboLinkCustom.SetCurSel(0);
	m_ComboInterveneProtocolParam.SetCurSel(0);
	m_ComboInterveneProtocolValue.SetCurSel(0);
	m_ComboInterveneBasebandParam.SetCurSel(0);
	m_ComboInterveneBasebandValue.SetCurSel(0);
	mCombo_businessSel.SetCurSel(0);
	mCombo_layerSel.SetCurSel(0);
	mCombo_spotSel.SetCurSel(0);
	mCombo_funcSel.SetCurSel(0);
	CheckRadioButton(IDC_RADIO_Main_ObsCtl_Mode_Single,IDC_RADIO_Main_ObsCtl_Mode_Multi,IDC_RADIO_Main_ObsCtl_Mode_Single);
	
	//phy初始化
	pcieSender->setHwnd(GetSafeHwnd(),HWND_Main);
	pcieReceiver->setHwnd(GetSafeHwnd(),HWND_Main);
	pcieSender->PcieOpen();
	pcieReceiver->PcieOpen(); 
	//选择网卡
	serverCaper->setMyHWnd(GetSafeHwnd(),HWND_Main);
	protocolCaper->setMyHWnd(GetSafeHwnd(),HWND_Main);
	CSelectNetDeviceDlg* pDlg=new CSelectNetDeviceDlg();
	pDlg->Create(IDD_DIALOG1,this);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->CenterWindow(this);
	AfxBeginThread(mainRecvFromPcie,this,0);
	//AfxBeginThread(recvFromPcie,this,0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

afx_msg LRESULT CPlatformOfObserveAndControlDlg::OnUserHaveselecteddevice(WPARAM wParam, LPARAM lParam)
{
	int* devID=(int*)lParam;
	//WinpCap初始化
	serverCaper->setMyAddr(node->mAddrHex);
	serverCaper->openAdapter(devID[0]);
	serverCaper->beginCatching();
	protocolCaper->setMyAddr(0x00aaaaaaaaaaaa);
	protocolCaper->openAdapter(devID[1]);
	protocolCaper->beginCatching();

	return 0;
}
//
//void CPlatformOfObserveAndControlDlg::selectNetDevice()
//{
//	USES_CONVERSION;
//	const CString filePath=_T(".\\NetDevSelectionConfig.ini");
//	CFile file;
//	if(file.Open(filePath,CFile::modeReadWrite))
//	{
//		file.Close();
//		devID=GetPrivateProfileInt(_T("DevInfo"),_T("DevID"),-1,filePath);
//		if(devID>0)
//		{
//			caper->setMyAddr(node->mAddrHex);
//			caper->openAdapter(devID);
//		}
//	}
//	else
//	{
//		if(file.Open(filePath,CFile::modeCreate|CFile::modeWrite))
//		{
//			file.SeekToBegin();
//			char temp[]="请在DevID项后填写选择设备号，配置完毕后关闭文件并点击刷新按钮:\r\n";
//			file.Write(temp,strlen(temp)+1);
//			PIP_ADAPTER_INFO pAdapterInfo=WinpCapHelper::getNetAdaptersInfo();
//			for(int i=0;pAdapterInfo!=NULL;pAdapterInfo=pAdapterInfo->Next,i++)
//			{
//				sprintf_s(temp,"%d: %02X:%02X:%02X:%02X:%02X:%02X\r\n",i,
//					pAdapterInfo->Address[0],pAdapterInfo->Address[1],pAdapterInfo->Address[2],
//					pAdapterInfo->Address[3],pAdapterInfo->Address[4],pAdapterInfo->Address[5]);
//				file.Write(temp,strlen(temp)+1);
//			}
//			file.Flush();
//			file.Close();
//			WritePrivateProfileString(_T("DevInfo"),_T("DevID"),_T(""),filePath);
//			ShellExecute(NULL, _T("open"), filePath, NULL, NULL, SW_SHOW);
//		}
//
//	}
//}

void CPlatformOfObserveAndControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlatformOfObserveAndControlDlg::OnPaint()
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
HCURSOR CPlatformOfObserveAndControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPlatformOfObserveAndControlDlg::initNodeInfo()
{
	CFileFind finder;
	CString configPath;
	int index;
	bool isFound=finder.FindFile(_T(".\\*.ini"));
	while(isFound)
	{
		isFound=finder.FindNextFile();
		configPath=_T(".\\")+finder.GetFileName();
		if((index=configPath.Find(_T("config[")))>=0)
		{
			CString tempStr=configPath.Mid(index+7,1);
			node->nodeID=_ttoi(tempStr);
			node->initial(node->nodeID);
			break;
		}
	}
	if(node->nodeID==-1)
	{
		CInitNodeIDDlg* pDlg=new CInitNodeIDDlg();
		pDlg->Create(IDD_ConfigNodeID_DIALOG,this);
		pDlg->ShowWindow(SW_SHOW);
		pDlg->CenterWindow(this);
		return;
	}
	node->initial(node->nodeID);

	CString tempStr;

	//节点配置信息
	//节点类型
	_CEditSetText(IDC_EDIT_Main_NodeInfo_Type,node->nodeType);

	//节点ID
	tempStr.Format(_T("%d"),node->nodeID);
	_CEditSetText(IDC_EDIT_Main_NodeInfo_ID,tempStr);

	//燃油状态
	_CEditSetText(IDC_EDIT_Main_NodeInfo_FuelState,_T("正常"));

	//装备状态
	_CEditSetText(IDC_EDIT_Main_NodeInfo_EquipState,_T("正常"));

	//工作状态
	_CEditSetText(IDC_EDIT_Main_NodeInfo_WorkState,_T("正常"));

	//工作频段
	_CEditSetText(IDC_EDIT_Main_NodeInfo_FreqRange,_T("正常"));

	//航迹信息
	_CEditSetText(IDC_EDIT_Main_NodeInfo_Track_Type,node->trackInfo.trackType);

	_CEditSetText(IDC_EDIT_Main_NodeInfo_Track_StartPos,node->trackInfo.startPosStr);

	tempStr.Format(_T("%.2f"),node->trackInfo.velocity);
	_CEditSetText(IDC_EDIT_Main_NodeInfo_Track_Velocity,tempStr);

	_CEditSetText(IDC_EDIT_Main_NodeInfo_Track_DstPos,node->trackInfo.dstPosStr);

	_CEditSetText(IDC_EDIT_Main_NodeInfo_Track_VRule,node->trackInfo.VRule);

	//selectNetDevice();
}

void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainObsctlBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); 
	CFileDialog fileDlg(TRUE); 
	fileDlg.m_ofn.lpstrTitle=_T("请选择数据源文件"); 
	fileDlg.m_ofn.lpstrFilter=_T("All Files(*.*)\0*.*\0\0");
	fileDlg.m_ofn.lpstrDefExt=_T("*.txt"); 
	if(IDOK==fileDlg.DoModal()) 
	{  
		fileDataSrc.Open(fileDlg.GetFileName(),CFile::modeRead);
		CString filePathStr=fileDlg.GetPathName();//文件名+后缀
		mEdit_dataSrcPath.SetWindowText(filePathStr);
		pcieReceiver->setPath(filePathStr);
		pcieSender->setPath(filePathStr);
	} 
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainBusinessEditconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	//const CString configPath=_T(".\\config.ini"); //配置文件相对路径
	//const CString sectionNode=_T("NodeInfo");
	//const CString sectionTrack=_T("TrackInfo");
	//CFileFind finder;
	//if(!finder.FindFile(configPath))
	//{
	//	CFile fileConfig;
	//	try
	//	{
	//		fileConfig.Open(configPath,CFile::modeCreate|CFile::modeWrite);
	//		fileConfig.SeekToBegin();
	//		char Data[] = ";请参照格式（config格式.txt）编写该配置文件\n";
	//		fileConfig.Write(Data,sizeof(Data));
	//		fileConfig.Flush();
	//		fileConfig.Close();
	//	}
	//	catch(CFileException *e)
	//	{
	//		CString errStr;
	//		errStr.Format(_T("文件创建失败:%d"),e->m_cause);
	//		MessageBox(errStr);
	//		fileConfig.Abort();
	//		e->Delete();
	//	}
	//}
	//ShellExecute(NULL, _T("open"), _T(".\\config.ini"), NULL, NULL, SW_SHOW);
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainNodeinfoRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	initNodeInfo();
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButton()
{
	// TODO: 在此添加控件通知处理程序代码
	int selStats=CStatisticsDlg::STAT_None;
	CButton* pCheck;
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_NumGeneratedPkts);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Pro_NumGenMsgs;
	}
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_NumSendedPkts);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Pro_NumSndMsgs;
	}
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_NumReceivedPkts);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Pro_NumRcvMsgs;
	}
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_NumDropedPkts);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Pro_NumDrpMsgs;
	}
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_NumAvgBusiness);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Pro_NumAvgBusiness;
	}
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_Delay);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Pro_Delay;
	}
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_DelayJitter);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Pro_DelayJitter;
	}
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_AvgPER);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Pro_AvgPER;
	}
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_Link_FreqDeviation);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Base_FreqDeviation;
	}
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_Main_Statistics_Link_ChannelEstimate);
	if(pCheck->GetCheck())
	{
		selStats|=CStatisticsDlg::STAT_Base_ChannelEstimate;
	}

	CStatisticsDlg* pDlg=new CStatisticsDlg;
	pDlg->node=node;
	pDlg->mSelStats=selStats;
	stat.setNetworkType(mCombo_businessSel.GetCurSel());
	pDlg->Create(IDD_STATISTICS,this);
	pDlg->ShowWindow(SW_SHOW);
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainBusinessStart()
{
	// TODO: 在此添加控件通知处理程序代码

	if(node->nodeType==_T("地面站"))
	{
		if(mCombo_businessSel.GetCurSel()==0)
		{
			CMasterControlDlg* pDlg=new CMasterControlDlg;
			pDlg->node=new MasterNode(*node);
			pDlg->Create(IDD_MasterControl,this);
			pDlg->ShowWindow(SW_SHOW);
		}
		else if(mCombo_businessSel.GetCurSel()==1)
		{
			CMasterBroadDlg* pDlg=new CMasterBroadDlg;
			this->pDlg=pDlg;
			pDlg->node=new MasterNode(*node);
			pDlg->Create(IDD_MasterControl1,this);
			pDlg->ShowWindow(SW_SHOW);
		}
	}
	else if(node->nodeType==_T("飞机"))
	{
		if(mCombo_businessSel.GetCurSel()==0)
		{
			CControlledDlg* pDlg=new CControlledDlg;
			pDlg->node=new SlaveNode(*node);
			pDlg->Create(IDD_Controlled,this);
			pDlg->ShowWindow(SW_SHOW);
		}
		else if(mCombo_businessSel.GetCurSel()==1)
		{
			CControlledBroadDlg* pDlg=new CControlledBroadDlg;
			pDlg->node=new SlaveNode(*node);
			pDlg->Create(IDD_Controlled1,this);
			pDlg->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		CString errStr=_T("节点类型错误，请修改配置！");
		MessageBox(errStr);
	}
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainObsctlApply()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int* cmd=NULL;
	int cmdLength;

	unsigned int srcAddr=SrcAddr::SrcAddr_LocalDlg;
	unsigned int dstAddr=mCombo_layerSel.GetCurSel()+DELTA_ADDR;
	unsigned int spot=mCombo_spotSel.GetCurSel()+DELTA_SPOT;
	unsigned int function=mCombo_funcSel.GetCurSel()+DELTA_FUNC;

	switch (function)
	{
	case Func_SndCollectOrder:
		{
			cmd=(unsigned int*)new Cmd_SndCollectOrder;
			SetCmd_SndCollectOrder((Cmd_SndCollectOrder*)cmd,node->nodeID,srcAddr,dstAddr,spot,10000,1);
			cmdLength=sizeof(Cmd_SndCollectOrder);
			pcieReceiver->writeCommand(cmd,cmdLength);
			pcieReceiver->PcieStart();
		}
		break;
	case Func_SndReloadOrder:
		{
			cmd=(unsigned int*)new Cmd_SndReloadOrder;
			SetCmd_SndReloadOrder((Cmd_SndReloadOrder*)cmd,node->nodeID,srcAddr,dstAddr,spot,10000,1);
			cmdLength=sizeof(Cmd_SndReloadOrder);
			pcieSender->writeCommand(cmd,cmdLength);
			pcieSender->PcieStart();
		}
		break;
	case Func_IntvNodeParams:
		{
			switch(dstAddr)
			{
			case DstAddr_BasebandBoard:
				{

				}
				break;
			case DstAddr_ProtocolBoard:
				{

				}
				break;
			}
			//if(cmd)
			//{
			//	PCIEHelper::sendToPCIE(cmd,cmdLength);
			//}
		}
		break;
	case Func_RecontructMode:
		{
			//if(cmd)
			//{
			//	PCIEHelper::sendToPCIE(cmd,cmdLength);
			//}
		}
		break;
	default:
		break;
	}
	
}

void CPlatformOfObserveAndControlDlg::recvFromWpCaper(const u_char* data,unsigned int len)
{
	//u_char* recvData=new u_char[len*sizeof(u_char)];
	//memcpy_s(recvData,len,data,len);
	//SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)recvData;

	//mCombo_layerSel.SetCurSel(header->dstAddr-DELTA_ADDR);
	//mCombo_funcSel.SetCurSel(header->function-DELTA_FUNC);
	//switch(header->function)
	//{
	//case Func_DeliverConfig:
	//	{
	//		Cmd_DeliverConfig* order=(Cmd_DeliverConfig*)recvData;
	//		recvFile(order->frameID,order->frameSize,order->endFlag,order->data);
	//	}
	//	break;
	//case Func_Close:
	//	{
	//		::SendMessage(GetSafeHwnd(),WM_CLOSE,NULL,NULL);
	//		return;
	//	}
	//	break;
	//default:
	//	break;
	//}
	////OnBnClickedButtonMainObsctlApply();
	//
	//EnterCriticalSection(&cs_WinpCap);//进入临界区
	//
	//LeaveCriticalSection(&cs_WinpCap);//离开临界区
	//xdelete(recvData);
}

void CPlatformOfObserveAndControlDlg::recvFile(unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char* data)
{
	char* recvData=(char*) data;
	CString stateStr;
	if(frameID==0)
	{
		CString fileNameStr;
		fileNameStr=recvData;
		fileNameStr=_T(".\\")+fileNameStr;
		if(!fileConfig.Open(fileNameStr,CFile::modeCreate|CFile::modeWrite))
		{
			AfxMessageBox(_T("创建配置文件失败！"));
			return;
		}
		stateStr=_T("开始接收文件：")+fileNameStr;
	}
	else if(frameID>0)
	{
		if(fileConfig.m_hFile!=CFile::hFileNull)
		{
			if(endFlag==0)
			{
				fileConfig.Write(recvData,frameSize);
				stateStr=_T("文件接收中：")+fileConfig.GetFileName();
			}
			else
			{
				fileConfig.Write(recvData,frameSize);
				fileConfig.Close();
				stateStr=_T("文件接收完毕：")+fileConfig.GetFileName();
			}
		}
	}
	mStatic_State.SetWindowText(stateStr);
}

void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainRecontructionBasebandRun()
{
	// TODO: 在此添加控件通知处理程序代码
	CString msgStr,str1,str2,str3;
	
	if(0x12345678==pcieSender->readParams(ParamName::Param_isConnected)
		&&0x12345678==pcieReceiver->readParams(ParamName::Param_isConnected))
	{
		m_ComboChannelEncode.GetWindowText(str1);
		m_ComboSpreadAlgo.GetWindowText(str2);
		m_ComboModMode.GetWindowText(str3);
		msgStr.Format(_T("基带板卡重构结果:\n\
						信道编码：\t%10s\n\
						扩频算法：\t%10s\n\
						调制模式：\t%10s\n"),
						str1,str2,str3);

		unsigned int* cmd=NULL;
		int cmdLength;
		unsigned int srcAddr=SrcAddr::SrcAddr_BasebandBoard;
		unsigned int dstAddr=DstAddr::DstAddr_LocalDlg;
		unsigned int spot=0;
		unsigned int function=Function::Func_RecontructMode;
		cmd=(unsigned int*)new Cmd_RecontructMode;
		SetCmd_RecontructMode((Cmd_RecontructMode*)cmd,node->nodeID,srcAddr,dstAddr,spot,1,0,0,NULL);
		cmdLength=sizeof(Cmd_RecontructMode);
		serverCaper->sendPacket(0,HWND_Main,(u_char*)cmd,cmdLength);
	}
	else
	{
		msgStr=_T("基带板卡重构失败\n");
	}
	AfxMessageBox(msgStr);

}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainRecontructionLinkRun()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int* cmd=NULL;
	int cmdLength;
	unsigned int srcAddr=SrcAddr::SrcAddr_LocalDlg;
	unsigned int dstAddr=DstAddr::DstAddr_ProtocolBoard;
	unsigned int spot=0;
	unsigned int function=Function::Func_RecontructMode;
	cmd=(unsigned int*)new Cmd_RecontructMode;
	unsigned char data[1024];
	data[0]=m_ComboFrameFormat.GetCurSel();
	data[1]=m_ComboLinkManage.GetCurSel();
	data[2]=m_ComboRsrcDist.GetCurSel();
	SetCmd_RecontructMode((Cmd_RecontructMode*)cmd,node->nodeID,srcAddr,dstAddr,spot,1,0,3,data);
	cmdLength=sizeof(Cmd_RecontructMode);
	protocolCaper->sendPacket(0x00555555555555,HWND_Main,(u_char*)cmd,cmdLength);
	xdelete(cmd);
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainInterveneProtocolRun()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int* cmd=NULL;
	int cmdLength;
	unsigned int srcAddr=SrcAddr::SrcAddr_LocalDlg;
	unsigned int dstAddr=DstAddr::DstAddr_ProtocolBoard;
	unsigned int spot=0;
	unsigned int function=Function::Func_IntvNodeParams;
	cmd=(unsigned int*)new Cmd_IntvModeParams;
	unsigned int paramName=m_ComboInterveneProtocolParam.GetCurSel();
	unsigned int paramValue=m_ComboInterveneProtocolValue.GetCurSel();
	SetCmd_IntvModeParams((Cmd_IntvModeParams*)cmd,node->nodeID,srcAddr,dstAddr,spot,paramName,paramValue);
	cmdLength=sizeof(Cmd_IntvModeParams);
	protocolCaper->sendPacket(0x00555555555555,HWND_Main,(u_char*)cmd,cmdLength);
	xdelete(cmd);
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainInterveneBasebandRun()
{
	// TODO: 在此添加控件通知处理程序代码
	CString msgStr,paramStr,valueStr;
	if(0x12345678==pcieSender->readParams(ParamName::Param_isConnected)
		&&0x12345678==pcieReceiver->readParams(ParamName::Param_isConnected))
	{
		m_ComboInterveneBasebandParam.GetWindowText(paramStr);
		m_ComboInterveneBasebandValue.GetWindowText(valueStr);
		msgStr.Format(_T("基带板卡 %s 已被置为 %s "),paramStr,valueStr);

		unsigned int* cmd=NULL;
		int cmdLength;
		unsigned int srcAddr=SrcAddr::SrcAddr_BasebandBoard;
		unsigned int dstAddr=DstAddr::DstAddr_LocalDlg;
		unsigned int spot=0;
		unsigned int function=Function::Func_IntvNodeParams;
		cmd=(unsigned int*)new Cmd_IntvModeParams;
		unsigned int paramName=m_ComboInterveneBasebandParam.GetCurSel();
		unsigned int paramValue=m_ComboInterveneBasebandValue.GetCurSel();
		SetCmd_IntvModeParams((Cmd_IntvModeParams*)cmd,node->nodeID,srcAddr,dstAddr,spot,paramName,paramValue);
		cmdLength=sizeof(Cmd_IntvModeParams);
		serverCaper->sendPacket(0,HWND_Main,(u_char*)cmd,cmdLength);

		if(m_ComboInterveneBasebandParam.GetCurSel()==0&&m_ComboInterveneBasebandValue.GetCurSel()==0)
		{
			if(!isSelfTesting)
			{
				unsigned int code=0x11223344;
				isSelfTesting=true;
				pcieSender->sendPacket(HWND_Main,(u_char*)&code,sizeof(unsigned int));
				SetTimer(0,990,NULL);
			}

		}
		else
		{
			isSelfTesting=false;
			KillTimer(0);
			KillTimer(1);
			mStatic_State.SetWindowText(_T(""));
		}
	}
	else
	{
		msgStr=_T("基带板卡干预失败\n");
	}


	AfxMessageBox(msgStr);
}



afx_msg LRESULT CPlatformOfObserveAndControlDlg::OnUserConfignodeid(WPARAM wParam, LPARAM lParam)
{
	int* nodeID=(int*)lParam;
	node->nodeID=*nodeID;
	node->mAddrHex=*nodeID+1;
	serverCaper->setMyAddr(node->mAddrHex);
	protocolCaper->setMyAddr(0x00aaaaaaaaaaaa);
	return 0;
}

afx_msg LRESULT CPlatformOfObserveAndControlDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
{
	unsigned char* data=(unsigned char*)lParam;
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)data;
	switch(header->function)
	{
	case Func_Cmd:
	{
		SoftwareCommend* cmd=(SoftwareCommend*)header;
		switch(cmd->buttonID)
		{
		case BTN_Close:
		{
			SendMessage(WM_CLOSE);
		}
		break;
		case BTN_Recontruct_Baseband_OK:
		{
			m_ComboChannelEncode.SetCurSel(cmd->comboValues[0]);
			m_ComboSpreadAlgo.SetCurSel(cmd->comboValues[1]);
			m_ComboModMode.SetCurSel(cmd->comboValues[2]);
			OnBnClickedButtonMainRecontructionBasebandRun();
		}
			break;
		case BTN_Recontruct_Protocol_OK:
		{
			m_ComboFrameFormat.SetCurSel(cmd->comboValues[0]);
			m_ComboLinkManage.SetCurSel(cmd->comboValues[1]);
			m_ComboRsrcDist.SetCurSel(cmd->comboValues[2]);
			OnBnClickedButtonMainRecontructionLinkRun();
		}
			break;
		case BTN_Intervene_Baseband_OK:
		{
			m_ComboInterveneBasebandParam.SetCurSel(cmd->comboValues[0]);
			m_ComboInterveneBasebandValue.SetCurSel(cmd->comboValues[1]);
			OnBnClickedButtonMainInterveneBasebandRun();
		}
			break;
		case BTN_Intervene_Protocol_OK:
		{
			m_ComboInterveneProtocolParam.SetCurSel(cmd->comboValues[0]);
			m_ComboInterveneProtocolValue.SetCurSel(cmd->comboValues[1]);
			OnBnClickedButtonMainInterveneProtocolRun();
		}
			break;
		case BTN_Business_Start:
		{
			mCombo_businessSel.SetCurSel(cmd->comboValues[0]);
			OnBnClickedButtonMainBusinessStart();
		}
			break;
		case BTN_ObsNCtrl_Apply:
		{
			mCombo_layerSel.SetCurSel(cmd->comboValues[0]);
			mCombo_spotSel.SetCurSel(cmd->comboValues[1]);
			mCombo_funcSel.SetCurSel(cmd->comboValues[2]);
			OnBnClickedButtonMainObsctlApply();
		}
			break;
		case BTN_Statistics:
		{
			CStatisticsDlg* pDlg=new CStatisticsDlg;
			pDlg->node=node;
			pDlg->mSelStats=cmd->comboValues[0];
			stat.setNetworkType(mCombo_businessSel.GetCurSel());
			pDlg->Create(IDD_STATISTICS,this);
			pDlg->ShowWindow(SW_SHOW);
		}
			break;
		default:
			break;
		}
	}
		break;
	case Func_IntvNodeParams:
		{
			Cmd_IntvModeParams* cmd=(Cmd_IntvModeParams*)header;
			serverCaper->sendPacket(0,HWND_Main,(u_char*)cmd,sizeof(Cmd_IntvModeParams));
			if(header->srcAddr==SrcAddr::SrcAddr_ProtocolBoard)
			{
				CString msgStr,paramStr,valueStr;
				m_ComboInterveneProtocolParam.SetCurSel(cmd->paramName);
				m_ComboInterveneProtocolParam.GetWindowText(paramStr);
				m_ComboInterveneProtocolValue.SetCurSel(cmd->paramValue);
				m_ComboInterveneProtocolValue.GetWindowText(valueStr);
				msgStr.Format(_T("协议板卡 %s 已被置为 %s "),paramStr,valueStr);
				AfxMessageBox(msgStr);
			}
			else if(header->srcAddr==SrcAddr::SrcAddr_BasebandBoard)
			{

			}
			else
			{
			}
		}
		break;
	case Func_RecontructMode:
		{
			Cmd_RecontructMode* cmd=(Cmd_RecontructMode*)header;
			serverCaper->sendPacket(0,HWND_Main,(u_char*)cmd,sizeof(Cmd_RecontructMode));
			if(header->srcAddr==SrcAddr::SrcAddr_ProtocolBoard)
			{
				CString msgStr,str1,str2,str3;
				m_ComboFrameFormat.SetCurSel(cmd->data[0]);
				m_ComboLinkManage.SetCurSel(cmd->data[1]);
				m_ComboRsrcDist.SetCurSel(cmd->data[2]);
				m_ComboFrameFormat.GetWindowText(str1);
				m_ComboLinkManage.GetWindowText(str2);
				m_ComboRsrcDist.GetWindowText(str3);
				msgStr.Format(_T("协议板卡重构结果:\n\
								帧格式：\t\t%10s\n\
								链路管理算法：\t%10s\n\
								资源分配算法：\t%10s\n"),
								str1,str2,str3);
				AfxMessageBox(msgStr);
			}
			else if(header->srcAddr==SrcAddr::SrcAddr_BasebandBoard)
			{

			}
			else
			{
			}
		}
		break;
	case Func_DeliverConfig:
		{
			Cmd_DeliverConfig* order=(Cmd_DeliverConfig*)header;
			recvFile(order->frameID,order->frameSize,order->endFlag,order->data);
		}
		break;
	case Func_Close:
		{
			::SendMessage(GetSafeHwnd(),WM_CLOSE,NULL,NULL);
			return 0;
		}
		break;
	default:
		break;
	}
	xdeletes(data);
	return 0;
}

UINT mainRecvFromPcie(LPVOID pParam)
{
	bool loop=true;
	PACKET packet;
	CPlatformOfObserveAndControlDlg* pDlg=(CPlatformOfObserveAndControlDlg*)pParam;
	HANDLE handles[] = {pcieReceiver->sem_packet_rec, pDlg->m_hEventQuit};
	while(loop)
	{
		switch(WaitForMultipleObjects(2, handles, FALSE, INFINITE))
			{
			case WAIT_OBJECT_0:
				{
					//WaitForSingleObject(pcieReceiver->queue_Mutex, INFINITE);
					packet=pcieReceiver->qBuff.front();
					pcieReceiver->qBuff.pop();
					//ReleaseMutex(pcieReceiver->queue_Mutex);
					SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)(packet.buf+1);
					switch(header->function)
					{
					case Func_DeliverConfig:
						{
							Cmd_DeliverConfig* cmd=(Cmd_DeliverConfig*)header;
							if(pDlg!=NULL)
								((CMasterBroadDlg*)(pDlg->pDlg))->recvFile(cmd->frameID,cmd->frameSize,cmd->endFlag,cmd->data);
						}
					break;
					default:
						if(packet.buf[0]>=0&&packet.buf[0]<HWND_Number)
							::SendMessage(pcieReceiver->hWnd[packet.buf[0]],WM_USER_RECVFROMPCIE,NULL,(LPARAM)&packet);
						break;
					}
				}
				break;
			default:
				loop = false;
				break;
			}
	}
	//caper->sendPacket(0,HWND_Master,(u_char*)lParam,packet.len);
	AfxEndThread(0);
	return 0;
}

void CPlatformOfObserveAndControlDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	serverCaper->setMyHWnd(NULL,HWND_Main);
	protocolCaper->setMyHWnd(NULL,HWND_Main);
	pcieSender->setHwnd(NULL,HWND_Main);
	pcieReceiver->setHwnd(NULL,HWND_Main);
	SetEvent(m_hEventQuit);
	//xdelete(serverCaper);
	//xdelete(protocolCaper);
	CDialogEx::OnClose();
}

void CPlatformOfObserveAndControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 0:
		{
			if(isSelfTesting)
			{
				unsigned int code=0x11223344;
				pcieSender->sendPacket(HWND_Main,(u_char*)&code,sizeof(unsigned int));
				SetTimer(1,1500,NULL);
			}
			
		}
		break;
	case 1:
		{
			if(isSelfTesting)
			{
				mStatic_State.SetWindowText(_T("自测模式：故障"));
				KillTimer(1);
			}
		}
		break;
	//case 2:
	//	{
	//		node->trackInfo.startPos.longtitude+=node->trackInfo.velocity;
	//		node->trackInfo.startPos.latitude+=node->trackInfo.velocity;
	//		CString posStr;
	//		posStr.Format(_T("(%.2f,%.2f,%.2f)"),
	//			node->trackInfo.startPos.longtitude,
	//			node->trackInfo.startPos.latitude,
	//			node->trackInfo.startPos.height);
	//		_CEditSetText(IDC_EDIT_Main_NodeInfo_Track_CurPos,posStr);
	//	}
	//	break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CPlatformOfObserveAndControlDlg::OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam)
{
	unsigned int testCode=0x11223344;
	unsigned int reportCode=0x44332211;
	PACKET packet=*(PACKET*)lParam;
	if(*(unsigned int*)(packet.buf+1)==reportCode)
	{
		mStatic_State.SetWindowText(_T("自测模式：工作正常"));
		if(isSelfTesting)
		{
			KillTimer(1);
		}

		serverCaper->sendPacket(0,HWND_Main,(u_char*)(packet.buf+1),sizeof(unsigned int));
		return 0;
	}
	else if(*(unsigned int*)(packet.buf+1)==testCode)
	{
		pcieSender->sendPacket(HWND_Main,(u_char*)&reportCode,sizeof(unsigned int));
		return 0;
	}
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)(packet.buf+1);
	switch(header->function)
	{
	default:
		break;
	}
	return 0;
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	pcieReceiver->PcieReset();
}
