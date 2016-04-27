// MasterControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "MasterControlDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "Node.h"
#include "WinpCapHelper.h"
#include "SoftwareCommend.h"

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
{

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
}


BEGIN_MESSAGE_MAP(CMasterControlDlg, CDialogEx)
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CMasterControlDlg::OnUserRecvfromwinpcap)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AddIn_StartGuide, &CMasterControlDlg::OnBnClickedButtonMasterctrlAddinStartguide)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AddIn_AddedUsers, &CMasterControlDlg::OnBnClickedButtonMasterctrlAddinAddedusers)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AddIn_CloseGuide, &CMasterControlDlg::OnBnClickedButtonMasterctrlAddinCloseguide)
	ON_BN_CLICKED(IDC_BUTTON_MasterCtrl_AddIn_Clear, &CMasterControlDlg::OnBnClickedButtonMasterctrlAddinClear)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command1, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand1)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command2, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand2)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command3, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand3)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command4, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand4)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command5, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand5)
	ON_BN_CLICKED(IDC_COMBO_MasterCtrl_ArmyCommand_Command6, &CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand6)
END_MESSAGE_MAP()


// CMasterControlDlg 消息处理程序


BOOL CMasterControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initNodeInfo();
	mCombo_selNetwork.SetCurSel(0);
	mCombo_selRecognitionNodeID.SetCurSel(0);
	mCombo_selMonitoringNodeID.SetCurSel(0);

	caper->setMyHWnd(GetSafeHwnd(),node->nodeID,HWND_Master);
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

afx_msg LRESULT CMasterControlDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
{
	unsigned char* data=(unsigned char*)lParam;
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)lParam;
	switch(header->function)
	{
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


void CMasterControlDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Close,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
	caper->setMyHWnd(NULL,node->nodeID,HWND_Master);
	CDialogEx::OnClose();
}


void CMasterControlDlg::OnBnClickedButtonMasterctrlAddinStartguide()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[1];
	comboValues[0]=mCombo_selNetwork.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_StartGuide,1,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CMasterControlDlg::OnBnClickedButtonMasterctrlAddinAddedusers()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[1];
	comboValues[0]=mCombo_selNetwork.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_ShowUsers,1,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CMasterControlDlg::OnBnClickedButtonMasterctrlAddinCloseguide()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[1];
	comboValues[0]=mCombo_selNetwork.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_EndGuide,1,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CMasterControlDlg::OnBnClickedButtonMasterctrlAddinClear()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[1];
	comboValues[0]=mCombo_selNetwork.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_ClearList,1,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand1()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Command1,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand2()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Command2,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand3()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Command3,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand4()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Command4,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand5()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Command5,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CMasterControlDlg::OnBnClickedComboMasterctrlArmycommandCommand6()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Command6,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Master,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}
