// PlatformOfObserveAndControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "PlatformOfObserveAndControlDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "SoftwareCommend.h"
#include "StatisticsDlg.h"
#include "MasterControlDlg.h"
#include "ControlledDlg.h"
#include "Node.h"
#include "WinpCapHelper.h"
#include "BroadGrdDlg.h"
#include "BroadPlaneDlg.h"
#include "MasterBroadDlg.h"
#include "ControlledBroadDlg.h"


// CPlatformOfObserveAndControlDlg 对话框

IMPLEMENT_DYNAMIC(CPlatformOfObserveAndControlDlg, CDialogEx)

CPlatformOfObserveAndControlDlg::CPlatformOfObserveAndControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlatformOfObserveAndControlDlg::IDD, pParent)
{
	node=new Node();
}

CPlatformOfObserveAndControlDlg::~CPlatformOfObserveAndControlDlg()
{
	xdelete(node);
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
	ON_BN_CLICKED(IDC_BUTTON_Main_ObsCtl_Browse, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainObsctlBrowse)
	ON_BN_CLICKED(IDC_BUTTON_Main_Business_EditConfig, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainBusinessEditconfig)
	ON_BN_CLICKED(IDC_BUTTON_Main_NodeInfo_Refresh, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainNodeinfoRefresh)
	ON_BN_CLICKED(IDC_BUTTON__Main_Statistic_OK, &CPlatformOfObserveAndControlDlg::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON_Main_Business_Start, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainBusinessStart)
	ON_BN_CLICKED(IDC_BUTTON_Main_ObsCtl_Apply, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainObsctlApply)
//	ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BUTTON_Main_Intervene_Protocol_Run, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainInterveneProtocolRun)
ON_BN_CLICKED(IDC_BUTTON_Main_Intervene_Baseband_Run, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainInterveneBasebandRun)
ON_BN_CLICKED(IDC_BUTTON_Main_Recontruction_Baseband_Run, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainRecontructionBasebandRun)
ON_BN_CLICKED(IDC_BUTTON_Main_Recontruction_Link_Run, &CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainRecontructionLinkRun)
ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CPlatformOfObserveAndControlDlg::OnUserRecvfromwinpcap)
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPlatformOfObserveAndControlDlg 消息处理程序


BOOL CPlatformOfObserveAndControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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

	caper->setMyHWnd(GetSafeHwnd(),node->nodeID,HWND_Main);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPlatformOfObserveAndControlDlg::initNodeInfo()
{
	if(node->nodeID==-1)
	{
		AfxMessageBox(_T("节点配置错误！"));
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
}

void CPlatformOfObserveAndControlDlg::recvFromWpCaper(const u_char* data,unsigned int len)
{
	u_char* recvData=new u_char[len*sizeof(u_char)];
	memcpy_s(recvData,len,data,len);
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)recvData;
	

	EnterCriticalSection(&cs_WinpCap);//进入临界区

	LeaveCriticalSection(&cs_WinpCap);//离开临界区
	xdelete(recvData);
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
		fileDataSrc.Close();
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
	CString fileName;
	fileName.Format(_T(".\\config[%d].ini"),node->nodeID);
	caper->sendConfigFile(node->mAddrHex,fileName);
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
	pDlg->mSelStats=selStats;
	pDlg->node=new MasterNode(*node);
	pDlg->Create(IDD_STATISTICS,this);
	pDlg->ShowWindow(SW_SHOW);

	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[1];
	comboValues[0]=selStats;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Statistics,1,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,sizeof(SoftwareCommend));
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
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[1];
	comboValues[0]=mCombo_businessSel.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Business_Start,1,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,sizeof(SoftwareCommend));
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainObsctlApply()
{
	// TODO: 在此添加控件通知处理程序代码
	//unsigned int* cmd=NULL;
	//int cmdLength;

	//unsigned int srcAddr=SrcAddr::SrcAddr_LocalDlg;
	//unsigned int dstAddr=mCombo_layerSel.GetCurSel()+DELTA_ADDR;
	//unsigned int spot=mCombo_spotSel.GetCurSel()+DELTA_SPOT;
	//unsigned int function=mCombo_funcSel.GetCurSel()+DELTA_FUNC;

	//switch (function)
	//{
	//case Func_DeliverConfig:
	//	{

	//		return;
	//	}
	//	break;
	//case Func_SndCollectOrder:
	//	{
	//		cmd=(unsigned int*)new Cmd_SndCollectOrder;
	//		SetCmd_SndCollectOrder((Cmd_SndCollectOrder*)cmd,node->nodeID,srcAddr,dstAddr,spot,10000,1);
	//		cmdLength=sizeof(Cmd_SndCollectOrder);
	//	}
	//	break;
	//case Func_SndReloadOrder:
	//	{
	//		cmd=(unsigned int*)new Cmd_SndReloadOrder;
	//		SetCmd_SndReloadOrder((Cmd_SndReloadOrder*)cmd,node->nodeID,srcAddr,dstAddr,spot,10000,1);
	//		cmdLength=sizeof(Cmd_SndReloadOrder);
	//	}
	//	break;
	//case Func_RecontructMode:
	//	{

	//	}
	//	break;
	//case Func_Close:
	//	{
	//		cmd=(unsigned int*)new Cmd_Close;
	//		SetCmd_Close((Cmd_Close*)cmd,node->nodeID,srcAddr,dstAddr);
	//		cmdLength=sizeof(Cmd_SndReloadOrder);
	//	}
	//	break;
	//default:
	//	break;
	//}
	//sendToPCIE(cmd,cmdLength);
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[3];
	comboValues[0]=mCombo_layerSel.GetCurSel();
	comboValues[1]=mCombo_spotSel.GetCurSel();
	comboValues[2]=mCombo_funcSel.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_ObsNCtrl_Apply,3,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}

void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainInterveneProtocolRun()
{
	// TODO: 在此添加控件通知处理程序代码
	//unsigned int* cmd=NULL;
	//int cmdLength;
	//unsigned int srcAddr=SrcAddr::SrcAddr_LocalDlg;
	//unsigned int dstAddr=DstAddr::DstAddr_ProtocolBoard;
	//unsigned int spot=0;
	//unsigned int function=Function::Func_IntvNodeParams;
	//cmd=(unsigned int*)new Cmd_IntvModeParams;
	//unsigned int paramName=m_ComboInterveneProtocolParam.GetCurSel();
	//unsigned int paramValue=m_ComboInterveneProtocolValue.GetCurSel();
	//SetCmd_IntvModeParams((Cmd_IntvModeParams*)cmd,node->nodeID,srcAddr,dstAddr,spot,paramName,paramValue);
	//cmdLength=sizeof(Cmd_IntvModeParams);
	//if(cmd)
	//{
	//	caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,cmdLength);
	//}
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[2];
	comboValues[0]=m_ComboInterveneProtocolParam.GetCurSel();
	comboValues[1]=m_ComboInterveneProtocolValue.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Intervene_Protocol_OK,2,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainInterveneBasebandRun()
{
	// TODO: 在此添加控件通知处理程序代码
	//unsigned int* cmd=NULL;
	//int cmdLength;
	//unsigned int srcAddr=SrcAddr::SrcAddr_LocalDlg;
	//unsigned int dstAddr=DstAddr::DstAddr_BasebandBoard;
	//unsigned int spot=0;
	//unsigned int function=Function::Func_IntvNodeParams;
	//cmd=(unsigned int*)new Cmd_IntvModeParams;
	//unsigned int paramName=m_ComboInterveneBasebandParam.GetCurSel();
	//unsigned int paramValue=m_ComboInterveneBasebandValue.GetCurSel();
	//SetCmd_IntvModeParams((Cmd_IntvModeParams*)cmd,node->nodeID,srcAddr,dstAddr,spot,paramName,paramValue);
	//cmdLength=sizeof(Cmd_IntvModeParams);
	//if(cmd)
	//{
	//	caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,cmdLength);
	//}
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[2];
	comboValues[0]=m_ComboInterveneBasebandParam.GetCurSel();
	comboValues[1]=m_ComboInterveneBasebandValue.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Intervene_Baseband_OK,2,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainRecontructionBasebandRun()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[3];
	comboValues[0]=m_ComboChannelEncode.GetCurSel();
	comboValues[1]=m_ComboSpreadAlgo.GetCurSel();
	comboValues[2]=m_ComboModMode.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Recontruct_Baseband_OK,3,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
}


void CPlatformOfObserveAndControlDlg::OnBnClickedButtonMainRecontructionLinkRun()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftwareCommend* cmd=new SoftwareCommend;
	unsigned int comboValues[3];
	comboValues[0]=m_ComboFrameFormat.GetCurSel();
	comboValues[1]=m_ComboLinkManage.GetCurSel();
	comboValues[2]=m_ComboRsrcDist.GetCurSel();
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Recontruct_Protocol_OK,3,comboValues);
	caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);

	//u_char testChar[]="01010110101010110100";
	//for(int i=0;i<1000;i++)
	//{
	//	caper->sendPacket(0x0000FFFFFFFFFFFF,0,testChar,20);
	//	Sleep(10);
	//}
}


afx_msg LRESULT CPlatformOfObserveAndControlDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
{
	unsigned char* data=(unsigned char*)lParam;
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)lParam;
	switch(header->function)
	{
	case Func_IntvNodeParams:
		{
			Cmd_IntvModeParams* cmd=(Cmd_IntvModeParams*)header;
			if(header->srcAddr==SrcAddr::SrcAddr_ProtocolBoard)
			{
				CString msgStr,paramStr,valueStr;
				m_ComboInterveneProtocolParam.GetWindowText(paramStr);
				m_ComboInterveneProtocolValue.GetWindowText(valueStr);
				msgStr.Format(_T("协议板卡 %s 已被置为 %s "),paramStr,valueStr);
				AfxMessageBox(msgStr);
			}
			else if(header->srcAddr==SrcAddr::SrcAddr_BasebandBoard)
			{
				CString msgStr,paramStr,valueStr;
				m_ComboInterveneBasebandParam.GetWindowText(paramStr);
				m_ComboInterveneBasebandValue.GetWindowText(valueStr);
				msgStr.Format(_T("基带板卡 %s 已被置为 %s "),paramStr,valueStr);
				AfxMessageBox(msgStr);
			}
			else
			{
			}
		}
		break;
	case Func_RecontructMode:
		{
			Cmd_RecontructMode* cmd=(Cmd_RecontructMode*)header;
			if(header->srcAddr==SrcAddr::SrcAddr_ProtocolBoard)
			{
				CString msgStr,str1,str2,str3;
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
				CString msgStr,str1,str2,str3;
				m_ComboChannelEncode.GetWindowText(str1);
				m_ComboSpreadAlgo.GetWindowText(str2);
				m_ComboModMode.GetWindowText(str3);
				msgStr.Format(_T("基带板卡重构结果:\n\
								 信道编码：\t%10s\n\
								 扩频算法：\t%10s\n\
								 调制模式：\t%10s\n"),
								 str1,str2,str3);
				AfxMessageBox(msgStr);
			}
			else
			{
			}
		}
		break;
	default:
		break;
	}
	xdeletes(data);
	return 0;
}


void CPlatformOfObserveAndControlDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//SoftwareCommend* cmd=new SoftwareCommend;
	//SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Close,0,NULL);
	//caper->sendPacket(node->mAddrHex,HWND_Main,(u_char*)cmd,sizeof(SoftwareCommend));
	//xdelete(cmd);
	caper->setMyHWnd(NULL,node->nodeID,HWND_Main);
	CDialogEx::OnClose();
}
