// ControlledBroadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "ControlledBroadDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "SoftwareCommend.h"
#include "Node.h"
#include "WinpCapHelper.h"
#include "PhyHelper.h"


// CControlledBroadDlg 对话框

IMPLEMENT_DYNAMIC(CControlledBroadDlg, CDialogEx)

CControlledBroadDlg::CControlledBroadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlledBroadDlg::IDD, pParent)
	, isAddedInNetwork(false)
	, isSendingRecogInfo(false)
	, isSendingMonitoringInfo(false)
	, count(0)
{

}

CControlledBroadDlg::~CControlledBroadDlg()
{
}

void CControlledBroadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Controlled_FileDeliver_Path, m_EditFilePath);
	DDX_Control(pDX, IDC_BUTTON_Controlled_FileDeliver_Browse, m_BtnBrowse);
	DDX_Control(pDX, IDC_PROGRESS_Controlled_FileDeliver, m_ProgressSending);
	DDX_Control(pDX, IDC_BUTTON_Controlled_FileDeliver_Open, m_BtnSend);
	DDX_Control(pDX, IDC_BUTTON_ControlledBroad_AddIn_AddInNetwork, mBtn_AddInNetwork);
	DDX_Control(pDX, IDC_COMBO_ControlledBroad_AddIn_SelNetwork, mCombo_selNetworkID);
	DDX_Control(pDX, IDC_PROGRESS_ControlledBroad_AddIn_AddInNetwork, m_ProgressAddIn);
	DDX_Control(pDX, IDC_BUTTON_Controlled_Recognition_Send, mBtn_SendRecogInfo);
	DDX_Control(pDX, IDC_BUTTON_Controlled_Monitoring_Send, mBtn_sendMonitoringInfo);

	DDX_Text(pDX, IDC_EDIT_Controlled_Navigation_StatePos, m_EditGrdPosStr);
	DDX_Text(pDX, IDC_EDIT_Controlled_Navigation_PlanePos, m_EditPlanePosStr);
	DDX_Text(pDX, IDC_EDIT_Controlled_Navigation_Quality, m_EditNaviQualStr);

	DDX_Control(pDX, IDC_COMBO_Controlled_AirCtrl_MasterCtrl_NodeSel, m_ComboSelTerminal);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_MasterCtrl_Start, m_BtnStartControl);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_Controlled_Preparing, m_BtnTaskMsg1);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_Controlled_Done, m_BtnTaskMsg2);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_Controlled_Refuse, m_BtnTaskMsg3);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_Controlled_Unable, m_BtnTaskMsg4);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_MasterCtrl_Quit, m_BtnStopControl);
}


BEGIN_MESSAGE_MAP(CControlledBroadDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Controlled_FileDeliver_Browse, &CControlledBroadDlg::OnBnClickedButtonControlledFiledeliverBrowse)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_FileDeliver_Open, &CControlledBroadDlg::OnBnClickedButtonControlledFiledeliverOpen)
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CControlledBroadDlg::OnUserRecvfromwinpcap)
	ON_BN_CLICKED(IDC_BUTTON_ControlledBroad_AddIn_AddInNetwork, &CControlledBroadDlg::OnBnClickedButtonControlledbroadAddinAddinnetwork)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Recognition_Send, &CControlledBroadDlg::OnBnClickedButtonControlledRecognitionSend)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Monitoring_Send, &CControlledBroadDlg::OnBnClickedButtonControlledMonitoringSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Navigation_Start, &CControlledBroadDlg::OnBnClickedButtonControlledNavigationStart)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Navigation_Stop, &CControlledBroadDlg::OnBnClickedButtonControlledNavigationStop)
	ON_BN_CLICKED(IDC_RADIO_Controlled_AirCtrl_MasterCtrl, &CControlledBroadDlg::OnBnClickedRadioControlledAirctrlMasterctrl)
	ON_BN_CLICKED(IDC_RADIO_Controlled_AirCtrl_Controlled, &CControlledBroadDlg::OnBnClickedRadioControlledAirctrlControlled)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_MasterCtrl_Start, &CControlledBroadDlg::OnBnClickedButtonControlledAirctrlMasterctrlStart)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_MasterCtrl_Quit, &CControlledBroadDlg::OnBnClickedButtonControlledAirctrlMasterctrlQuit)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_Controlled_Preparing, &CControlledBroadDlg::OnBnClickedButtonControlledAirctrlControlledPreparing)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_Controlled_Done, &CControlledBroadDlg::OnBnClickedButtonControlledAirctrlControlledDone)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_Controlled_Refuse, &CControlledBroadDlg::OnBnClickedButtonControlledAirctrlControlledRefuse)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_Controlled_Unable, &CControlledBroadDlg::OnBnClickedButtonControlledAirctrlControlledUnable)
END_MESSAGE_MAP()


// CControlledBroadDlg 消息处理程序


BOOL CControlledBroadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ProgressAddIn.SetRange32(0,4);
	initNodeInfo();
	mCombo_selNetworkID.SetCurSel(0);
	m_ComboSelTerminal.SetCurSel(1);
	OnBnClickedButtonControlledNavigationStop();
	serverCaper->setMyHWnd(GetSafeHwnd(),HWND_BroadPlane);
	serverCaper->setMyAddr(node->mAddrHex);
	pcieSender->setHwnd(GetSafeHwnd(),HWND_BroadPlane);
	pcieReceiver->setHwnd(GetSafeHwnd(),HWND_BroadPlane);
	m_EditFilePath.SetWindowText(_T(".\\RMB100.jpg"));
	CheckRadioButton(IDC_RADIO_Controlled_AirCtrl_MasterCtrl,IDC_RADIO_Controlled_AirCtrl_Controlled,IDC_RADIO_Controlled_AirCtrl_Controlled);
	CheckRadioButton(IDC_RADIO_Controlled_AirCtrl_MasterCtrl_Return,IDC_RADIO_Controlled_AirCtrl_MasterCtrl_Dodge,IDC_RADIO_Controlled_AirCtrl_MasterCtrl_Return);
	CheckRadioButton(IDC_RADIO_Controlled_AirCtrl_Controlled_Preparing,IDC_RADIO_Controlled_AirCtrl_Controlled_Unable,IDC_RADIO_Controlled_AirCtrl_Controlled_Preparing);
	OnBnClickedRadioControlledAirctrlControlled();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CControlledBroadDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	serverCaper->setMyHWnd(NULL,HWND_BroadPlane);
	pcieSender->setHwnd(NULL,HWND_BroadPlane);
	pcieReceiver->setHwnd(NULL,HWND_BroadPlane);
	CDialogEx::OnClose();
}

void CControlledBroadDlg::initNodeInfo()
{
	CString tempStr;

	//节点配置信息
	//节点类型
	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_Type,node->nodeType);

	//节点ID
	tempStr.Format(_T("%d"),node->nodeID);
	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_ID,tempStr);

	//燃油状态
	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_FuelState,_T("正常"));

	//装备状态
	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_EquipState,_T("正常"));

	//工作状态
	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_WorkState,_T("正常"));

	//工作频段
	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_FreqRange,_T("正常"));

	//航迹信息
	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_Track_Type,node->trackInfo.trackType);

	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_Track_StartPos,node->trackInfo.startPosStr);

	tempStr.Format(_T("%.2f"),node->trackInfo.velocity);
	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_Track_Velocity,tempStr);

	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_Track_DstPos,node->trackInfo.dstPosStr);

	_CEditSetText(IDC_EDIT_ControlledBroad_NodeInfo_Track_VRule,node->trackInfo.VRule);
}

void CControlledBroadDlg::OnBnClickedButtonControlledFiledeliverBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE); 
	fileDlg.m_ofn.lpstrTitle=_T("请选择数据源文件"); 
	fileDlg.m_ofn.lpstrFilter=_T("Picture Files\0*.bmp;*.jpeg;*.jpg;*.png\0\0");
	//fileDlg.m_ofn.lpstrFilter=_T("All Files(*.*)\0*.*\0\0");
	fileDlg.m_ofn.lpstrDefExt=_T("*.txt"); 
	if(IDOK==fileDlg.DoModal()) 
	{  
		CFile fileDataSrc;
		fileDataSrc.Open(fileDlg.GetFileName(),CFile::modeRead);
		CString filePathStr=fileDlg.GetPathName();//文件名+后缀
		m_EditFilePath.SetWindowText(filePathStr);
	} 
}


void CControlledBroadDlg::OnBnClickedButtonControlledFiledeliverOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filePath;
	m_EditFilePath.GetWindowText(filePath);
	pcieSender->sendFile(this,HWND_BroadGrd,filePath);
}


afx_msg LRESULT CControlledBroadDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
{
	unsigned char* data=(unsigned char*)lParam;
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)lParam;
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
			case BTN_AddIn:
			{
				mCombo_selNetworkID.SetCurSel(cmd->comboValues[0]);
				OnBnClickedButtonControlledbroadAddinAddinnetwork();
			}
			break;
			case BTN_SendFile:
				{
					OnBnClickedButtonControlledFiledeliverOpen();
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	xdeletes(data);
	return 0;
}


void CControlledBroadDlg::OnBnClickedButtonControlledbroadAddinAddinnetwork()
{
	// TODO: 在此添加控件通知处理程序代码
	CString btnStr;
	Busi_AddIn* cmd=new Busi_AddIn;
	if(!isAddedInNetwork)
	{
		btnStr=_T("停止接入");
		isAddedInNetwork=true;
		m_ProgressAddIn.SetPos(4);
		SetBusi_AddIn(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,mCombo_selNetworkID.GetCurSel(),node->nodeID,Reaction_AddIn);
	}
	else
	{
		btnStr=_T("接入网络");
		isAddedInNetwork=false;
		m_ProgressAddIn.SetPos(0);
		SetBusi_AddIn(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,mCombo_selNetworkID.GetCurSel(),node->nodeID,Reaction_Exit);
		
	}
	pcieSender->sendPacket(HWND_BroadGrd,(unsigned char*)cmd,sizeof(Busi_AddIn));
	mBtn_AddInNetwork.SetWindowText(btnStr);
}


void CControlledBroadDlg::OnBnClickedButtonControlledRecognitionSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString btnStr;
	if(isSendingRecogInfo)
	{
		btnStr=_T("发送识别信息");
	}
	else
	{
		btnStr=_T("停止发送");
	}
	isSendingRecogInfo=!isSendingRecogInfo;
	mBtn_SendRecogInfo.SetWindowText(btnStr);
}


void CControlledBroadDlg::OnBnClickedButtonControlledMonitoringSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString btnStr;
	if(isSendingMonitoringInfo)
	{
		btnStr=_T("发送航迹信息");
	}
	else
	{
		btnStr=_T("停止发送");
	}
	isSendingMonitoringInfo=!isSendingMonitoringInfo;
	mBtn_sendMonitoringInfo.SetWindowText(btnStr);
}

void CControlledBroadDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 0:
		{
			m_EditGrdPosStr=node->trackInfo.startPosStr;
			m_EditPlanePosStr.Format(_T("(%d,%d,%d)"),237*(count++),43*(count++),880);
			m_EditNaviQualStr=_T("良好");
			UpdateData(FALSE);
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CControlledBroadDlg::OnBnClickedButtonControlledNavigationStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditGrdPosStr=node->trackInfo.startPosStr;
	m_EditPlanePosStr.Format(_T("(%d,%d,%d)"),237*(count++),43*(count++),880);
	m_EditNaviQualStr=_T("良好");
	UpdateData(FALSE);
	SetTimer(0,10000,NULL);
}


void CControlledBroadDlg::OnBnClickedButtonControlledNavigationStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditGrdPosStr=_T("--");
	m_EditPlanePosStr=_T("--");
	m_EditNaviQualStr=_T("--");
	UpdateData(FALSE);
	KillTimer(0);
}


void CControlledBroadDlg::OnBnClickedRadioControlledAirctrlMasterctrl()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BtnStartControl.EnableWindow(TRUE);
	m_BtnStopControl.EnableWindow(TRUE);
	m_BtnTaskMsg1.EnableWindow(FALSE);
	m_BtnTaskMsg2.EnableWindow(FALSE);
	m_BtnTaskMsg3.EnableWindow(FALSE);
	m_BtnTaskMsg4.EnableWindow(FALSE);

}


void CControlledBroadDlg::OnBnClickedRadioControlledAirctrlControlled()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BtnStartControl.EnableWindow(FALSE);
	m_BtnStopControl.EnableWindow(FALSE);
	m_BtnTaskMsg1.EnableWindow(TRUE);
	m_BtnTaskMsg2.EnableWindow(TRUE);
	m_BtnTaskMsg3.EnableWindow(TRUE);
	m_BtnTaskMsg4.EnableWindow(TRUE);

	_CEditSetText(IDC_EDIT_Controlled_AirCtrl_Controlled_NodeType,_T("地面站"));
	_CEditSetText(IDC_EDIT_Controlled_AirCtrl_Controlled_NodeID,_T("1"));
	_CEditSetText(IDC_EDIT_Controlled_AirCtrl_Controlled_Pos,_T("--"));
	_CEditSetText(IDC_EDIT_Controlled_AirCtrl_Controlled_TaskCodes,_T("0x00000000"));
	_CEditSetText(IDC_EDIT_Controlled_AirCtrl_Controlled_TaskCodesExplanation,_T("就绪"));
}


void CControlledBroadDlg::OnBnClickedButtonControlledAirctrlMasterctrlStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString msgStr;
	m_ComboSelTerminal.GetWindowTextW(msgStr);
	msgStr=_T("正在控制：节点")+msgStr;
	AfxMessageBox(msgStr);
}


void CControlledBroadDlg::OnBnClickedButtonControlledAirctrlMasterctrlQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("停止控制"));
}


void CControlledBroadDlg::OnBnClickedButtonControlledAirctrlControlledPreparing()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("已回复"));
}


void CControlledBroadDlg::OnBnClickedButtonControlledAirctrlControlledDone()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("已回复"));
}


void CControlledBroadDlg::OnBnClickedButtonControlledAirctrlControlledRefuse()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("已回复"));
}


void CControlledBroadDlg::OnBnClickedButtonControlledAirctrlControlledUnable()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("已回复"));
}
