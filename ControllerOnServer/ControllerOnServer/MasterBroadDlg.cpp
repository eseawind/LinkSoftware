// MasterBroadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "MasterBroadDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "Node.h"
#include "WinpCapHelper.h"
#include "SoftwareCommend.h"

// CMasterBroadDlg 对话框

IMPLEMENT_DYNAMIC(CMasterBroadDlg, CDialogEx)

CMasterBroadDlg::CMasterBroadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMasterBroadDlg::IDD, pParent)
{

}

CMasterBroadDlg::~CMasterBroadDlg()
{
}

void CMasterBroadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MasterCtrl_AddIn_SelNetwork, mCombo_selNetworkID);
}


BEGIN_MESSAGE_MAP(CMasterBroadDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CMasterBroadDlg::OnUserRecvfromwinpcap)
END_MESSAGE_MAP()


// CMasterBroadDlg 消息处理程序


void CMasterBroadDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Close,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_BroadGrd,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
	caper->setMyHWnd(NULL,node->nodeID,HWND_BroadGrd);
	CDialogEx::OnClose();
}


BOOL CMasterBroadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mCombo_selNetworkID.SetCurSel(0);
	initNodeInfo();
	caper->setMyHWnd(GetSafeHwnd(),node->nodeID,HWND_BroadGrd);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMasterBroadDlg::initNodeInfo()
{
	CString tempStr;

	//节点配置信息
	//节点类型
	_CEditSetText(IDC_EDIT_MasterCtrl1_NodeInfo_Type,node->nodeType);

	//节点ID
	tempStr.Format(_T("%d"),node->nodeID);
	_CEditSetText(IDC_EDIT_MasterCtrl1_NodeInfo_ID,tempStr);

	//燃油状态
	_CEditSetText(IDC_EDIT_MasterCtrl1_NodeInfo_FuelState,_T("正常"));

	//装备状态
	_CEditSetText(IDC_EDIT_MasterCtrl1_NodeInfo_EquipState,_T("正常"));

	//工作状态
	_CEditSetText(IDC_EDIT_MasterCtrl1_NodeInfo_WorkState,_T("正常"));

	//工作频段
	_CEditSetText(IDC_EDIT_MasterCtrl1_NodeInfo_FreqRange,_T("正常"));
}

afx_msg LRESULT CMasterBroadDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
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
