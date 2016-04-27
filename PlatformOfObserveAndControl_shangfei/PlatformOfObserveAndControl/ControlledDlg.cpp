// ControlledDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "ControlledDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "Node.h"
#include "SoftwareCommend.h"
#include "PhyHelper.h"
#include "WinpCapHelper.h"

// CControlledDlg 对话框

IMPLEMENT_DYNAMIC(CControlledDlg, CDialogEx)

CControlledDlg::CControlledDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlledDlg::IDD, pParent)
	, isAddedInNetwork(FALSE)
	, isSendingRecogInfo(FALSE)
	, isSendingMonitoringInfo(FALSE)
	, m_EditGrdPosStr(_T(""))
	, m_EditPlanePosStr(_T(""))
	, m_EditNaviQualStr(_T(""))
	, count(0)
{

}

CControlledDlg::~CControlledDlg()
{
}

void CControlledDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Controlled_AddIn_SelNetwork, mCombo_selNetworkID);
	DDX_Control(pDX, IDC_PROGRESS_Controlled_AddIn_AddInNetwork, mProgress_AddInNetwork);
	DDX_Control(pDX, IDC_BUTTON_Controlled_Recognition_Send, mBtn_SendRecogInfo);
	DDX_Control(pDX, IDC_BUTTON_Controlled_Monitoring_Send, mBtn_sendMonitoringInfo);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AddIn_AddInNetwork, mBtn_AddInNetwork);
	DDX_Control(pDX, IDC_COMBO_Controlled_AirCtrl_MasterCtrl_NodeSel, m_ComboSelTerminal);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_MasterCtrl_Start, m_BtnStartControl);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_Controlled_Preparing, m_BtnTaskMsg1);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_Controlled_Done, m_BtnTaskMsg2);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_Controlled_Refuse, m_BtnTaskMsg3);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_Controlled_Unable, m_BtnTaskMsg4);
	DDX_Control(pDX, IDC_BUTTON_Controlled_AirCtrl_MasterCtrl_Quit, m_BtnStopControl);
	DDX_Text(pDX, IDC_EDIT_Controlled_Navigation_StatePos, m_EditGrdPosStr);
	DDX_Text(pDX, IDC_EDIT_Controlled_Navigation_PlanePos, m_EditPlanePosStr);
	DDX_Text(pDX, IDC_EDIT_Controlled_Navigation_Quality, m_EditNaviQualStr);
}


BEGIN_MESSAGE_MAP(CControlledDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AddIn_AddInNetwork, &CControlledDlg::OnBnClickedButtonControlledAddinAddinnetwork)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Recognition_Send, &CControlledDlg::OnBnClickedButtonControlledRecognitionSend)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Monitoring_Send, &CControlledDlg::OnBnClickedButtonControlledMonitoringSend)
	ON_MESSAGE(WM_USER_RECVFROMPCIE, &CControlledDlg::OnUserRecvfrompcie)
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CControlledDlg::OnUserRecvfromwinpcap)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Accpet, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondAccpet)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Refuse, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondRefuse)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Command1, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand1)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Command2, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand2)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Command3, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand3)
	ON_BN_CLICKED(IDC_RADIO_Controlled_AirCtrl_MasterCtrl, &CControlledDlg::OnClickedRadioControlledAirctrlMasterctrl)
	ON_BN_CLICKED(IDC_RADIO_Controlled_AirCtrl_Controlled, &CControlledDlg::OnBnClickedRadioControlledAirctrlControlled)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_MasterCtrl_Start, &CControlledDlg::OnBnClickedButtonControlledAirctrlMasterctrlStart)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_Controlled_Preparing, &CControlledDlg::OnBnClickedButtonControlledAirctrlControlledPreparing)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_Controlled_Done, &CControlledDlg::OnBnClickedButtonControlledAirctrlControlledDone)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_Controlled_Refuse, &CControlledDlg::OnBnClickedButtonControlledAirctrlControlledRefuse)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_Controlled_Unable, &CControlledDlg::OnBnClickedButtonControlledAirctrlControlledUnable)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AirCtrl_MasterCtrl_Quit, &CControlledDlg::OnBnClickedButtonControlledAirctrlMasterctrlQuit)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Navigation_Stop, &CControlledDlg::OnBnClickedButtonControlledNavigationStop)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Navigation_Start, &CControlledDlg::OnBnClickedButtonControlledNavigationStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CControlledDlg 消息处理程序


BOOL CControlledDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initNodeInfo();
	mProgress_AddInNetwork.SetRange32(0,4);
	mCombo_selNetworkID.SetCurSel(0);
	m_ComboSelTerminal.SetCurSel(1);
	OnBnClickedButtonControlledNavigationStop();
	serverCaper->setMyHWnd(GetSafeHwnd(),HWND_Controlled);
	pcieSender->setHwnd(GetSafeHwnd(),HWND_Controlled);
	pcieReceiver->setHwnd(GetSafeHwnd(),HWND_Controlled);
	CheckRadioButton(IDC_RADIO_Controlled_AirCtrl_MasterCtrl,IDC_RADIO_Controlled_AirCtrl_Controlled,IDC_RADIO_Controlled_AirCtrl_Controlled);
	CheckRadioButton(IDC_RADIO_Controlled_AirCtrl_MasterCtrl_Return,IDC_RADIO_Controlled_AirCtrl_MasterCtrl_Dodge,IDC_RADIO_Controlled_AirCtrl_MasterCtrl_Return);
	CheckRadioButton(IDC_RADIO_Controlled_AirCtrl_Controlled_Preparing,IDC_RADIO_Controlled_AirCtrl_Controlled_Unable,IDC_RADIO_Controlled_AirCtrl_Controlled_Preparing);
	OnBnClickedRadioControlledAirctrlControlled();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CControlledDlg::initNodeInfo()
{
	CString tempStr;

	//节点配置信息
	//节点类型
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Type,node->nodeType);

	//节点ID
	tempStr.Format(_T("%d"),node->nodeID);
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_ID,tempStr);

	//燃油状态
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_FuelState,_T("正常"));

	//装备状态
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_EquipState,_T("正常"));

	//工作状态
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_WorkState,_T("正常"));

	//工作频段
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_FreqRange,_T("正常"));

	//航迹信息
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_Type,node->trackInfo.trackType);

	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_StartPos,node->trackInfo.startPosStr);

	tempStr.Format(_T("%.2f"),node->trackInfo.velocity);
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_Velocity,tempStr);

	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_DstPos,node->trackInfo.dstPosStr);

	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_VRule,node->trackInfo.VRule);
}

void CControlledDlg::OnBnClickedButtonControlledAddinAddinnetwork()
{
	// TODO: 在此添加控件通知处理程序代码
	CString btnStr;
	Busi_AddIn* cmd=new Busi_AddIn;
	if(!isAddedInNetwork)
	{
		btnStr=_T("停止接入");
		isAddedInNetwork=true;
		mProgress_AddInNetwork.SetPos(4);
		SetBusi_AddIn(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,mCombo_selNetworkID.GetCurSel(),node->nodeID,Reaction_AddIn);
	}
	else
	{
		isAddedInNetwork=false;
		btnStr=_T("接入网络");
		mProgress_AddInNetwork.SetPos(0);
		SetBusi_AddIn(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,mCombo_selNetworkID.GetCurSel(),node->nodeID,Reaction_Exit);
		
	}
	pcieSender->sendPacket(HWND_Master,(unsigned char*)cmd,sizeof(Busi_AddIn));
	mBtn_AddInNetwork.SetWindowText(btnStr);
	

}


void CControlledDlg::OnBnClickedButtonControlledRecognitionSend()
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


void CControlledDlg::OnBnClickedButtonControlledMonitoringSend()
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

afx_msg LRESULT CControlledDlg::OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam)
{
	PACKET packet=*(PACKET*)lParam;
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)(packet.buf+1);
	switch(header->function)
	{
	case Func_Cmd:
	{

	}
		break;
	case Func_ProtocolBusiness:
	{
		Cmd_ProtoclBusinessHeader* businessHeader=(Cmd_ProtoclBusinessHeader*)header;
		switch(businessHeader->type)
		{
		case Business_AddIn:
		{
			Busi_AddIn* cmd=(Busi_AddIn*)businessHeader;
			switch(cmd->action)
			{
			case Action_StartGuide:
			{
				
			}
				break;
			case Action_Start:
			{
				
			}
				break;
			case Action_Clear:
			{
				node->mNetworkID=-1;
			}
				break;
			default:
				break;
			}
		}
			break;
		case Business_Command:
			{
				Busi_Command* cmd=(Busi_Command*) businessHeader;
				CString tempStr;
				tempStr.Format(_T("%d"),cmd->nodeID);
				_CEditSetText(IDC_EDIT_Controlled_ArmyCommand_SrcID,tempStr);
				tempStr.Format(_T("%08X"),cmd->commandType);
				_CEditSetText(IDC_EDIT_Controlled_ArmyCommand_Code,tempStr);
				_CEditSetText(IDC_EDIT_Controlled_ArmyCommand_CodeExplanation,RECOMMANDS[cmd->commandType]);

				serverCaper->sendPacket(0,HWND_Controlled,(u_char*)cmd,sizeof(Busi_Command));
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
	//caper->sendPacket(0,HWND_Controlled,(u_char*)lParam,packet.len);
	return 0;
}


afx_msg LRESULT CControlledDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
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
			OnBnClickedButtonControlledAddinAddinnetwork();
		}
			break;
		case BTN_ReCommandYes:
			{
				OnBnClickedButtonControlledArmycommandRespondAccpet();
			}
			break;
		case BTN_ReCommandNo:
			{
				OnBnClickedButtonControlledArmycommandRespondRefuse();
			}
			break;
		case BTN_ReCommand1:
			{
				OnBnClickedButtonControlledArmycommandRespondCommand1();
			}
			break;
		case BTN_ReCommand2:
			{
				OnBnClickedButtonControlledArmycommandRespondCommand2();
			}
			break;
		case BTN_ReCommand3:
			{
				OnBnClickedButtonControlledArmycommandRespondCommand3();
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


void CControlledDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	serverCaper->setMyHWnd(NULL,HWND_Controlled);
	pcieSender->setHwnd(NULL,HWND_Controlled);
	pcieReceiver->setHwnd(NULL,HWND_Controlled);
	CDialogEx::OnClose();
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondAccpet()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,ReCommandYes);
	pcieSender->sendPacket(HWND_Master,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondRefuse()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,ReCommandNo);
	pcieSender->sendPacket(HWND_Master,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand1()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,ReCommand1);
	pcieSender->sendPacket(HWND_Master,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand2()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,ReCommand2);
	pcieSender->sendPacket(HWND_Master,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand3()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,ReCommand3);
	pcieSender->sendPacket(HWND_Master,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CControlledDlg::OnClickedRadioControlledAirctrlMasterctrl()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BtnStartControl.EnableWindow(TRUE);
	m_BtnStopControl.EnableWindow(TRUE);
	m_BtnTaskMsg1.EnableWindow(FALSE);
	m_BtnTaskMsg2.EnableWindow(FALSE);
	m_BtnTaskMsg3.EnableWindow(FALSE);
	m_BtnTaskMsg4.EnableWindow(FALSE);
}


void CControlledDlg::OnBnClickedRadioControlledAirctrlControlled()
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


void CControlledDlg::OnBnClickedButtonControlledAirctrlMasterctrlStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString msgStr;
	m_ComboSelTerminal.GetWindowTextW(msgStr);
	msgStr=_T("正在控制：节点")+msgStr;
	AfxMessageBox(msgStr);
}


void CControlledDlg::OnBnClickedButtonControlledAirctrlControlledPreparing()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("已回复"));
}


void CControlledDlg::OnBnClickedButtonControlledAirctrlControlledDone()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("已回复"));
}


void CControlledDlg::OnBnClickedButtonControlledAirctrlControlledRefuse()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("已回复"));
}


void CControlledDlg::OnBnClickedButtonControlledAirctrlControlledUnable()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("已回复"));
}


void CControlledDlg::OnBnClickedButtonControlledAirctrlMasterctrlQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("停止控制"));
}


void CControlledDlg::OnBnClickedButtonControlledNavigationStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditGrdPosStr=_T("--");
	m_EditPlanePosStr=_T("--");
	m_EditNaviQualStr=_T("--");
	UpdateData(FALSE);
	KillTimer(0);
}


void CControlledDlg::OnBnClickedButtonControlledNavigationStart()
{
	// TODO: 在此添加控件通知处理程序代码

	m_EditGrdPosStr=node->trackInfo.startPosStr;
	m_EditPlanePosStr.Format(_T("(%d,%d,%d)"),237*(count++),43*(count++),880);
	m_EditNaviQualStr=_T("良好");
	UpdateData(FALSE);
	SetTimer(0,10000,NULL);
}


void CControlledDlg::OnTimer(UINT_PTR nIDEvent)
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
