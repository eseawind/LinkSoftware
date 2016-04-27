// ControlledBroadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "ControlledBroadDlg.h"
#include "afxdialogex.h"
#include "Globe.h"
#include "Node.h"
#include "WinpCapHelper.h"
#include "SoftwareCommend.h"

// CControlledBroadDlg �Ի���

IMPLEMENT_DYNAMIC(CControlledBroadDlg, CDialogEx)

CControlledBroadDlg::CControlledBroadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlledBroadDlg::IDD, pParent)
{

}

CControlledBroadDlg::~CControlledBroadDlg()
{
}

void CControlledBroadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_Controlled_FileDeliver, m_ProgressSending);
	DDX_Control(pDX, IDC_EDIT_Controlled_FileDeliver_Path, m_EditFilePath);
	DDX_Control(pDX, IDC_COMBO_Controlled_AddIn_SelNetwork, mCombo_selNetworkID);
}


BEGIN_MESSAGE_MAP(CControlledBroadDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Controlled_FileDeliver_Browse, &CControlledBroadDlg::OnBnClickedButtonControlledFiledeliverBrowse)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_FileDeliver_Open, &CControlledBroadDlg::OnBnClickedButtonControlledFiledeliverOpen)
	ON_BN_CLICKED(IDC_BUTTON_Controlled_AddIn_AddInNetwork, &CControlledBroadDlg::OnBnClickedButtonControlledAddinAddinnetwork)
END_MESSAGE_MAP()


// CControlledBroadDlg ��Ϣ�������


void CControlledBroadDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Close,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_BroadPlane,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
	caper->setMyHWnd(NULL,node->nodeID,HWND_BroadPlane);
	CDialogEx::OnClose();
}


BOOL CControlledBroadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	mCombo_selNetworkID.SetCurSel(0);
	initNodeInfo();
	caper->setMyHWnd(GetSafeHwnd(),node->nodeID,HWND_BroadPlane);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CControlledBroadDlg::initNodeInfo()
{
	CString tempStr;

	//�ڵ�������Ϣ
	//�ڵ�����
	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_Type,node->nodeType);

	//�ڵ�ID
	tempStr.Format(_T("%d"),node->nodeID);
	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_ID,tempStr);

	//ȼ��״̬
	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_FuelState,_T("����"));

	//װ��״̬
	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_EquipState,_T("����"));

	//����״̬
	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_WorkState,_T("����"));

	//����Ƶ��
	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_FreqRange,_T("����"));

	//������Ϣ
	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_Track_Type,node->trackInfo.trackType);

	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_Track_StartPos,node->trackInfo.startPosStr);

	tempStr.Format(_T("%.2f"),node->trackInfo.velocity);
	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_Track_Velocity,tempStr);

	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_Track_DstPos,node->trackInfo.dstPosStr);

	_CEditSetText(IDC_EDIT_Controlled1_NodeInfo_Track_VRule,node->trackInfo.VRule);
}


void CControlledBroadDlg::OnBnClickedButtonControlledFiledeliverBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE); 
	fileDlg.m_ofn.lpstrTitle=_T("��ѡ������Դ�ļ�"); 
	fileDlg.m_ofn.lpstrFilter=_T("Picture Files\0*.bmp;*.jpeg;*.jpg;*.png\0\0");
	//fileDlg.m_ofn.lpstrFilter=_T("All Files(*.*)\0*.*\0\0");
	fileDlg.m_ofn.lpstrDefExt=_T("*.txt"); 
	if(IDOK==fileDlg.DoModal()) 
	{  
		CFile fileDataSrc;
		fileDataSrc.Open(fileDlg.GetFileName(),CFile::modeRead);
		CString filePathStr=fileDlg.GetPathName();//�ļ���+��׺
		m_EditFilePath.SetWindowText(filePathStr);
	} 
}


void CControlledBroadDlg::OnBnClickedButtonControlledFiledeliverOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_SendFile,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_BroadPlane,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CControlledBroadDlg::OnBnClickedButtonControlledAddinAddinnetwork()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[1];
	comboValues[0]=mCombo_selNetworkID.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_AddIn,1,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_BroadPlane,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}
