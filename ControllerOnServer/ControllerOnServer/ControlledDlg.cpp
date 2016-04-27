// ControlledDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "ControlledDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "Node.h"
#include "WinpCapHelper.h"
#include "SoftwareCommend.h"


// CControlledDlg �Ի���

IMPLEMENT_DYNAMIC(CControlledDlg, CDialogEx)

CControlledDlg::CControlledDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlledDlg::IDD, pParent)
	, isAddedInNetwork(FALSE)
	, isSendingRecogInfo(FALSE)
	, isSendingMonitoringInfo(FALSE)
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
}


BEGIN_MESSAGE_MAP(CControlledDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AddIn_AddInNetwork, &CControlledDlg::OnBnClickedButtonControlledAddinAddinnetwork)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Recognition_Send, &CControlledDlg::OnBnClickedButtonControlledRecognitionSend)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_Monitoring_Send, &CControlledDlg::OnBnClickedButtonControlledMonitoringSend)
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CControlledDlg::OnUserRecvfromwinpcap)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Accpet, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondAccpet)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Refuse, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondRefuse)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Command1, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand1)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Command2, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand2)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_ArmyCommand_Respond_Command3, &CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand3)
END_MESSAGE_MAP()


// CControlledDlg ��Ϣ�������


BOOL CControlledDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	initNodeInfo();
	mCombo_selNetworkID.SetCurSel(0);
	caper->setMyHWnd(GetSafeHwnd(),node->nodeID,HWND_Controlled);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CControlledDlg::initNodeInfo()
{
	CString tempStr;

	//�ڵ�������Ϣ
	//�ڵ�����
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Type,node->nodeType);

	//�ڵ�ID
	tempStr.Format(_T("%d"),node->nodeID);
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_ID,tempStr);

	//ȼ��״̬
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_FuelState,_T("����"));

	//װ��״̬
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_EquipState,_T("����"));

	//����״̬
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_WorkState,_T("����"));

	//����Ƶ��
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_FreqRange,_T("����"));

	//������Ϣ
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_Type,node->trackInfo.trackType);

	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_StartPos,node->trackInfo.startPosStr);

	tempStr.Format(_T("%.2f"),node->trackInfo.velocity);
	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_Velocity,tempStr);

	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_DstPos,node->trackInfo.dstPosStr);

	_CEditSetText(IDC_EDIT_Controlled_NodeInfo_Track_VRule,node->trackInfo.VRule);
}

void CControlledDlg::OnBnClickedButtonControlledAddinAddinnetwork()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString btnStr;
	//if(isAddedInNetwork)
	//{
	//	btnStr=_T("��������");
	//}
	//else
	//{
	//	btnStr=_T("ֹͣ����");
	//}
	//isAddedInNetwork=!isAddedInNetwork;
	//mBtn_AddInNetwork.SetWindowText(btnStr);
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[1];
	comboValues[0]=mCombo_selNetworkID.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_AddIn,1,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Controlled,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CControlledDlg::OnBnClickedButtonControlledRecognitionSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString btnStr;
	if(isSendingRecogInfo)
	{
		btnStr=_T("����ʶ����Ϣ");
	}
	else
	{
		btnStr=_T("ֹͣ����");
	}
	isSendingRecogInfo=!isSendingRecogInfo;
	mBtn_SendRecogInfo.SetWindowText(btnStr);
}


void CControlledDlg::OnBnClickedButtonControlledMonitoringSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString btnStr;
	if(isSendingMonitoringInfo)
	{
		btnStr=_T("���ͺ�����Ϣ");
	}
	else
	{
		btnStr=_T("ֹͣ����");
	}
	isSendingMonitoringInfo=!isSendingMonitoringInfo;
	mBtn_sendMonitoringInfo.SetWindowText(btnStr);
}


afx_msg LRESULT CControlledDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Close,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Controlled,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
	caper->setMyHWnd(NULL,node->nodeID,HWND_Controlled);
	CDialogEx::OnClose();
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondAccpet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_ReCommandYes,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Controlled,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondRefuse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_ReCommandNo,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Controlled,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_ReCommand1,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Controlled,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_ReCommand2,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Controlled,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CControlledDlg::OnBnClickedButtonControlledArmycommandRespondCommand3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_ReCommand3,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Controlled,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}
