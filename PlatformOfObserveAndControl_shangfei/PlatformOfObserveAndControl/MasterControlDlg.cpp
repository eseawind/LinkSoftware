// MasterControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "MasterControlDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "Node.h"
#include "PhyHelper.h"
#include "SoftwareCommend.h"
#include "WinpCapHelper.h"


// CMasterControlDlg 对话框

IMPLEMENT_DYNAMIC(CMasterControlDlg, CDialogEx)

CMasterControlDlg::CMasterControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMasterControlDlg::IDD, pParent)
	, mEditStr_dstNodeID(_T(""))
	, mEditStr_dstNodeType(_T(""))
	, mEditStr_dstNodeVoiceNetID(_T(""))
	, mEditStr_dstNodeAirCtrlNetID(_T(""))
	, mEditStr_dstNodeNetworkState(_T(""))
	, mEditStr_dstNodeNaviQual(_T(""))
	, mEditStr_dstNodeEquipState(_T(""))
	, mEditStr_dstNodeFuelState(_T(""))
	, mEditStr_dstNodeAckState(_T(""))
	, mEditStr_dstNodeCustom1(_T(""))
	, m_EditGrdPosStr(_T(""))
	, m_EditPlanePosStr(_T(""))
	, m_EditNaviQualStr(_T(""))
	, count(0)
{
	m_hEventQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CMasterControlDlg::~CMasterControlDlg()
{
}

void CMasterControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MasterCtrl_AddIn_SelNetwork, mCombo_selNetwork);
	DDX_Control(pDX, IDC_COMBO_MasterCtrl_Recognition_NodeID, mCombo_selRecognitionNodeID);
	DDX_Control(pDX, IDC_COMBO_MasterCtrl_Monitoring_NodeID, mCombo_selMonitoringNodeID);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_ID, mEditStr_dstNodeID);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_Type, mEditStr_dstNodeType);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_VoiceNetID, mEditStr_dstNodeVoiceNetID);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_AirCtrlNetID, mEditStr_dstNodeAirCtrlNetID);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_NetworkState, mEditStr_dstNodeNetworkState);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_NaviQuality, mEditStr_dstNodeNaviQual);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_EquipState, mEditStr_dstNodeEquipState);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_FuelState, mEditStr_dstNodeFuelState);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_AckState, mEditStr_dstNodeAckState);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Recognition_NodeInfo_Custom1, mEditStr_dstNodeCustom1);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Navigation_StatePos, m_EditGrdPosStr);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Navigation_PlanePos, m_EditPlanePosStr);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl_Navigation_Quality, m_EditNaviQualStr);
	DDX_Control(pDX, IDC_COMBO_MasterCtrl_AirCtrl_NodeSel, m_ComboSelTerminal);
}


BEGIN_MESSAGE_MAP(CMasterControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AddIn_StartGuide, &CMasterControlDlg::OnBnClickedButtonMasterctrlAddinStartguide)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AddIn_CloseGuide, &CMasterControlDlg::OnBnClickedButtonMasterctrlAddinCloseguide)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AddIn_Clear, &CMasterControlDlg::OnBnClickedButtonMasterctrlAddinClear)
	ON_MESSAGE(WM_USER_RECVFROMPCIE, &CMasterControlDlg::OnUserRecvfrompcie)
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CMasterControlDlg::OnUserRecvfromwinpcap)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AddIn_AddedUsers, &CMasterControlDlg::OnBnClickedButtonMasterctrlAddinAddedusers)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command1, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand1)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command2, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand2)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command3, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand3)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command4, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand4)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command5, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand5)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command6, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand6)
	ON_CBN_SELCHANGE(IDC_COMBO_MasterCtrl_Recognition_NodeID, &CMasterControlDlg::OnSelchangeComboMasterctrlRecognitionNodeid)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_Navigation_Start, &CMasterControlDlg::OnBnClickedButtonMasterctrlNavigationStart)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_Navigation_Stop, &CMasterControlDlg::OnBnClickedButtonMasterctrlNavigationStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AirCtrl_Start, &CMasterControlDlg::OnBnClickedButtonMasterctrlAirctrlStart)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AirCtrl_Stop, &CMasterControlDlg::OnBnClickedButtonMasterctrlAirctrlStop)
END_MESSAGE_MAP()


// CMasterControlDlg 消息处理程序


BOOL CMasterControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initNodeInfo();
	mCombo_selNetwork.SetCurSel(0);
	mCombo_selRecognitionNodeID.SetCurSel(0);
	OnSelchangeComboMasterctrlRecognitionNodeid();
	mCombo_selMonitoringNodeID.SetCurSel(0);
	OnBnClickedButtonMasterctrlNavigationStop();
	m_ComboSelTerminal.SetCurSel(1);
	serverCaper->setMyHWnd(GetSafeHwnd(),HWND_Master);
	pcieSender->setHwnd(GetSafeHwnd(),HWND_Master);
	pcieReceiver->setHwnd(GetSafeHwnd(),HWND_Master);
	CheckRadioButton(IDC_RADIO_MasterCtrl_AirCtrl_Return,IDC_RADIO_MasterCtrl_AirCtrl_Dodge,IDC_RADIO_MasterCtrl_AirCtrl_Return);

	//AfxBeginThread(masterRecvFromPcie,this,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMasterControlDlg::initNodeInfo()
{
	CString tempStr;

	//节点配置信息
	//节点类型
	_CEditSetText(IDC_EDIT_MasterCtrl_NodeInfo_Type,node->nodeType);

	//节点ID
	tempStr.Format(_T("%d"),node->nodeID);
	_CEditSetText(IDC_EDIT_MasterCtrl_NodeInfo_ID,tempStr);

	//燃油状态
	_CEditSetText(IDC_EDIT_MasterCtrl_NodeInfo_FuelState,_T("正常"));

	//装备状态
	_CEditSetText(IDC_EDIT_MasterCtrl_NodeInfo_EquipState,_T("正常"));

	//工作状态
	_CEditSetText(IDC_EDIT_MasterCtrl_NodeInfo_WorkState,_T("正常"));

	//工作频段
	_CEditSetText(IDC_EDIT_MasterCtrl_NodeInfo_FreqRange,_T("正常"));
}

void CMasterControlDlg::OnBnClickedButtonMasterctrlAddinStartguide()
{
	// TODO: 在此添加控件通知处理程序代码
	int networkID=mCombo_selNetwork.GetCurSel();
	node->networks.open(networkID);
}


void CMasterControlDlg::OnBnClickedButtonMasterctrlAddinCloseguide()
{
	// TODO: 在此添加控件通知处理程序代码
	int networkID=mCombo_selNetwork.GetCurSel();
	node->networks.close(networkID);
}


void CMasterControlDlg::OnBnClickedButtonMasterctrlAddinClear()
{
	// TODO: 在此添加控件通知处理程序代码
	int networkID=mCombo_selNetwork.GetCurSel();
	node->networks.clear(networkID);
}


afx_msg LRESULT CMasterControlDlg::OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam)
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
			serverCaper->sendPacket(0,HWND_Master,(u_char*)cmd,sizeof(Busi_AddIn));
			switch(cmd->action)
			{
			case Reaction_AddIn:
			{
				CString msg;
				node->networks.addIn(cmd->networkID,cmd->nodeID);
				msg.Format(_T("节点%d加入网络%d"),cmd->nodeID,cmd->networkID);
				AfxMessageBox(msg);
			}
				break;
			case Reaction_Exit:
			{
				CString msg;
				node->networks.quit(cmd->networkID,cmd->nodeID);
				msg.Format(_T("节点%d退出网络%d"),cmd->nodeID,cmd->networkID);
				AfxMessageBox(msg);
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
				_CEditSetText(IDC_COMBO_MasterCtrl_ArmyCommand_ReNodeID,tempStr);
				tempStr.Format(_T("%08X"),cmd->commandType);
				_CEditSetText(IDC_COMBO_MasterCtrl_ArmyCommand_Report,tempStr);
				_CEditSetText(IDC_COMBO_MasterCtrl_ArmyCommand_ReportExplanation,RECOMMANDS[cmd->commandType]);

				serverCaper->sendPacket(0,HWND_Master,(u_char*)cmd,sizeof(Busi_Command));
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
	//caper->sendPacket(0,HWND_Controlled,(u_char*)header,packet.len);
	return 0;
}


afx_msg LRESULT CMasterControlDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
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
		case BTN_StartGuide:
		{
			mCombo_selNetwork.SetCurSel(cmd->comboValues[0]);
			OnBnClickedButtonMasterctrlAddinStartguide();
		}
			break;
		case BTN_EndGuide:
		{
			mCombo_selNetwork.SetCurSel(cmd->comboValues[0]);
			OnBnClickedButtonMasterctrlAddinCloseguide();
		}
			break;
		case BTN_ShowUsers:
		{
			mCombo_selNetwork.SetCurSel(cmd->comboValues[0]);
			OnBnClickedButtonMasterctrlAddinAddedusers();
		}
			break;
		case BTN_ClearList:
		{
			mCombo_selNetwork.SetCurSel(cmd->comboValues[0]);
			OnBnClickedButtonMasterctrlAddinClear();
		}
			break;
		case BTN_Command1:
			{
				OnBnClickedComboMasterctrlArmycommandCommand1();
			}
			break;
		case BTN_Command2:
			{
				OnBnClickedComboMasterctrlArmycommandCommand2();
			}
			break;
		case BTN_Command3:
			{
				OnBnClickedComboMasterctrlArmycommandCommand3();
			}
			break;
		case BTN_Command4:
			{
				OnBnClickedComboMasterctrlArmycommandCommand4();
			}
			break;
		case BTN_Command5:
			{
				OnBnClickedComboMasterctrlArmycommandCommand5();
			}
			break;
		case BTN_Command6:
			{
				OnBnClickedComboMasterctrlArmycommandCommand6();
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


void CMasterControlDlg::OnBnClickedButtonMasterctrlAddinAddedusers()
{
	// TODO: 在此添加控件通知处理程序代码

}

UINT masterRecvFromPcie(LPVOID pParam)
{
	bool loop=true;
	PACKET packet;
	CMasterControlDlg* pDlg=(CMasterControlDlg*)pParam;
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
				
						}
					break;
					default:
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

void CMasterControlDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	serverCaper->setMyHWnd(NULL,HWND_Master);
	pcieSender->setHwnd(NULL,HWND_Master);
	pcieReceiver->setHwnd(NULL,HWND_Master);
	SetEvent(m_hEventQuit);
	CDialogEx::OnClose();
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand1()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,Command1);
	pcieSender->sendPacket(HWND_Controlled,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand2()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,Command2);
	pcieSender->sendPacket(HWND_Controlled,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand3()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,Command3);
	pcieSender->sendPacket(HWND_Controlled,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand4()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,Command4);
	pcieSender->sendPacket(HWND_Controlled,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand5()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,Command5);
	pcieSender->sendPacket(HWND_Controlled,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand6()
{
	// TODO: 在此添加控件通知处理程序代码
	Busi_Command* cmd=new Busi_Command;
	SetBusi_Command(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_BasebandBoard,0,node->nodeID,Command6);
	pcieSender->sendPacket(HWND_Controlled,(unsigned char*)cmd,sizeof(Busi_AddIn));
}


void CMasterControlDlg::OnSelchangeComboMasterctrlRecognitionNodeid()
{
	// TODO: 在此添加控件通知处理程序代码
	int nodeID=mCombo_selRecognitionNodeID.GetCurSel()+1;
	mEditStr_dstNodeID.Format(_T("%d"),nodeID);
	if(nodeID==1||nodeID==3)
	{
		mEditStr_dstNodeType=_T("地面端");
	}
	else
	{
		mEditStr_dstNodeType=_T("飞机");
	}
	mEditStr_dstNodeVoiceNetID=_T("0");
	mEditStr_dstNodeAirCtrlNetID=_T("0");
	mEditStr_dstNodeNetworkState=_T("正常");
	mEditStr_dstNodeNaviQual=_T("良好");
	mEditStr_dstNodeEquipState=_T("正常");
	mEditStr_dstNodeFuelState=_T("正常");
	mEditStr_dstNodeAckState=_T("待命");
	UpdateData(FALSE);
}


void CMasterControlDlg::OnBnClickedButtonMasterctrlNavigationStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditGrdPosStr=node->trackInfo.startPosStr;
	m_EditPlanePosStr.Format(_T("(%d,%d,%d)"),237*(count++),43*(count++),880);
	m_EditNaviQualStr=_T("良好");
	UpdateData(FALSE);
	SetTimer(0,10000,NULL);
}


void CMasterControlDlg::OnBnClickedButtonMasterctrlNavigationStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditGrdPosStr=_T("--");
	m_EditPlanePosStr=_T("--");
	m_EditNaviQualStr=_T("--");
	UpdateData(FALSE);
	KillTimer(0);
}


void CMasterControlDlg::OnTimer(UINT_PTR nIDEvent)
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


void CMasterControlDlg::OnBnClickedButtonMasterctrlAirctrlStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString msgStr;
	m_ComboSelTerminal.GetWindowTextW(msgStr);
	msgStr=_T("正在控制：节点")+msgStr;
	AfxMessageBox(msgStr);
}


void CMasterControlDlg::OnBnClickedButtonMasterctrlAirctrlStop()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("停止控制"));
}
