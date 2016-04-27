// StatisticsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "StatisticsDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "Node.h"
#include "SoftwareCommend.h"
#include "WinpCapHelper.h"
#include "PhyHelper.h"
#include "PlatformOfObserveAndControlDlg.h"


// CStatisticsDlg 对话框

IMPLEMENT_DYNAMIC(CStatisticsDlg, CDialogEx)

CStatisticsDlg::CStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStatisticsDlg::IDD, pParent)
	, mEditStr_numGenMsgs(_T(""))
	, mEditStr_numSndMsgs(_T(""))
	, mEditStr_numRecvMsgs(_T(""))
	, mEditStr_numDrpMsgs(_T(""))
	, mEditStr_numAvgBusiness(_T(""))
	, mEditStr_delay(_T(""))
	, mEditStr_delayJitter(_T(""))
	, mEditStr_avgPER(_T(""))
	, mSelStats(STAT_None)
	, m_EditFreqDeviationStr(_T(""))
{

	m_EditChannelEstimateStr = _T("");
}

CStatisticsDlg::~CStatisticsDlg()
{
}

void CStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STAT_NumGeneratedMsgs, mEditStr_numGenMsgs);
	DDX_Text(pDX, IDC_EDIT_STAT_NumSendedMsgs, mEditStr_numSndMsgs);
	DDX_Text(pDX, IDC_EDIT_STAT_NumRecvdMsgs, mEditStr_numRecvMsgs);
	DDX_Text(pDX, IDC_EDIT_STAT_NumDropedMsgs, mEditStr_numDrpMsgs);
	DDX_Text(pDX, IDC_EDIT_STAT_NumAvgBusiness, mEditStr_numAvgBusiness);
	DDX_Text(pDX, IDC_EDIT_STAT_Delay, mEditStr_delay);
	DDX_Text(pDX, IDC_EDIT_STAT_DelayJitter, mEditStr_delayJitter);
	DDX_Text(pDX, IDC_EDIT_STAT_AvgPER, mEditStr_avgPER);
	DDX_Text(pDX, IDC_EDIT_STAT_Link_FreqDeviation, m_EditFreqDeviationStr);
	DDX_Text(pDX, IDC_EDIT_STAT_Link_ChannelEstimate, m_EditChannelEstimateStr);
}


BEGIN_MESSAGE_MAP(CStatisticsDlg, CDialogEx)
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CStatisticsDlg::OnUserRecvfromwinpcap)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CStatisticsDlg 消息处理程序
BOOL CStatisticsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO: 在此添加额外的初始化代码
	initSelEdits();

	pcieReceiver->setHwnd(GetSafeHwnd(),HWND_Statistics);
	pcieSender->setHwnd(GetSafeHwnd(),HWND_Statistics);
	serverCaper->setMyHWnd(GetSafeHwnd(),HWND_Statistics);
	protocolCaper->setMyHWnd(GetSafeHwnd(),HWND_Statistics);
	Cmd_ShowStatisticsOrder* cmd=new Cmd_ShowStatisticsOrder;

	unsigned int values[8]={CStatisticsDlg::STAT_Pro_NumGenMsgs,
							CStatisticsDlg::STAT_Pro_NumSndMsgs,
							CStatisticsDlg::STAT_Pro_NumRcvMsgs,
							CStatisticsDlg::STAT_Pro_NumDrpMsgs,
							CStatisticsDlg::STAT_Pro_NumAvgBusiness,
							CStatisticsDlg::STAT_Pro_Delay,
							CStatisticsDlg::STAT_Pro_DelayJitter,
							CStatisticsDlg::STAT_Pro_AvgPER};
	SetCmd_ShowStatisticsOrder(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_ProtocolBoard,8,values);
	protocolCaper->sendPacket(0x00555555555555,HWND_Statistics,(u_char*)cmd,sizeof(Cmd_ShowStatisticsOrder));
	xdelete(cmd);

	SetTimer(0,1000,NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStatisticsDlg::initSelEdits()
{
	//todo 显示报文
	CString notChosen=_T("--");
	float ParamValues[2]={0xffffffff,0xffffffff};
	if(!(mSelStats&STAT_Pro_NumGenMsgs))
	{
		mEditStr_numGenMsgs=notChosen;
	}
	if(!(mSelStats&STAT_Pro_NumSndMsgs))
	{
		mEditStr_numSndMsgs=notChosen;
	}
	if(!(mSelStats&STAT_Pro_NumRcvMsgs))
	{
		mEditStr_numRecvMsgs=notChosen;
	}
	if(!(mSelStats&STAT_Pro_NumDrpMsgs))
	{
		mEditStr_numDrpMsgs=notChosen;
	}
	if(!(mSelStats&STAT_Pro_NumAvgBusiness))
	{
		mEditStr_numAvgBusiness=notChosen;
	}
	if(!(mSelStats&STAT_Pro_Delay))
	{
		mEditStr_delay=notChosen;
	}
	if(!(mSelStats&STAT_Pro_DelayJitter))
	{
		mEditStr_delayJitter=notChosen;
	}
	if(!(mSelStats&STAT_Pro_AvgPER))
	{
		mEditStr_avgPER=notChosen;
	}
	if(!(mSelStats&STAT_Base_FreqDeviation))
	{
		m_EditFreqDeviationStr=notChosen;
	}
	else
	{
		ParamValues[0]=pcieReceiver->readParams(ParamName::Param_FrequenceDeviation);
		if(ParamValues[0]!=0xffffffff)
		{
			m_EditFreqDeviationStr.Format(_T("%u"),ParamValues[0]);
		}
		else
		{

		}

	}
	if(!(mSelStats&STAT_Base_ChannelEstimate))
	{
		m_EditChannelEstimateStr=notChosen;
	}
	else
	{
		ParamValues[1]=pcieReceiver->readParams(ParamName::Param_ChannelEstimate);
		if(ParamValues[1]!=0xffffffff)
		{
			m_EditChannelEstimateStr.Format(_T("%u"),ParamValues[1]);
		}
		else
		{

		}
	}
	_CEditSetText(IDC_EDIT_STAT_Link_Custom1,notChosen);
	_CEditSetText(IDC_EDIT_STAT_Link_Custom2,notChosen);
	_CEditSetText(IDC_EDIT_STAT_Link_Custom3,notChosen);
	UpdateData(FALSE);

	Cmd_ShowStatisticsData* cmd=new Cmd_ShowStatisticsData;
	SetCmd_ShowStatisticsData(cmd,node->nodeID,DstAddr::DstAddr_LocalDlg,SrcAddr::SrcAddr_Outside,2,ParamValues);
	serverCaper->sendPacket(0,HWND_Statistics,(u_char*)cmd,sizeof(Cmd_ShowStatisticsOrder));
	xdelete(cmd);
}


afx_msg LRESULT CStatisticsDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
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
		default:
			break;
		}
	}
		break;
	case Func_ShowStatisticsData:
		{
			stat.update();

			const UINT IDCs[]={IDC_EDIT_STAT_NumGeneratedMsgs,
								IDC_EDIT_STAT_NumSendedMsgs,
								IDC_EDIT_STAT_NumRecvdMsgs,
								IDC_EDIT_STAT_NumDropedMsgs,
								IDC_EDIT_STAT_NumAvgBusiness,
								IDC_EDIT_STAT_Delay,
								IDC_EDIT_STAT_DelayJitter,
								IDC_EDIT_STAT_AvgPER};
			const CString FORMATS[]={_T("%.0f"),_T("%.0f"),_T("%.0f"),_T("%0.1f"),
									_T("%0.1f"),_T("%0.6f"),_T("%0.6f"),_T("%0.2f")};
			CString valueStr;
			//Cmd_ShowStatisticsData* cmd=(Cmd_ShowStatisticsData*)header;
			for(int i=0;i<8;i++)
			{
				if(mSelStats&(0x0001<<i))
				{
					valueStr.Format(FORMATS[i],stat.paramValue[i]);
					_CEditSetText(IDCs[i],valueStr);
				}
			}
			
			Cmd_ShowStatisticsData* cmd=new Cmd_ShowStatisticsData;
			SetCmd_ShowStatisticsData(cmd,*header,8,stat.paramValue);
			serverCaper->sendPacket(0,HWND_Statistics,(u_char*)cmd,sizeof(Cmd_ShowStatisticsOrder));
		}
		break;
	default:
		break;
	}
	xdeletes(data);
	return 0;
}


void CStatisticsDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	pcieReceiver->setHwnd(NULL,HWND_Statistics);
	pcieSender->setHwnd(NULL,HWND_Statistics);
	serverCaper->setMyHWnd(NULL,HWND_Statistics);
	protocolCaper->setMyHWnd(NULL,HWND_Statistics);
	KillTimer(0);
	CDialogEx::OnClose();
}


void CStatisticsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 0:
		{
			Cmd_ShowStatisticsOrder* cmd=new Cmd_ShowStatisticsOrder;
			unsigned int values[8]={CStatisticsDlg::STAT_Pro_NumGenMsgs,
									CStatisticsDlg::STAT_Pro_NumSndMsgs,
									CStatisticsDlg::STAT_Pro_NumRcvMsgs,
									CStatisticsDlg::STAT_Pro_NumDrpMsgs,
									CStatisticsDlg::STAT_Pro_NumAvgBusiness,
									CStatisticsDlg::STAT_Pro_Delay,
									CStatisticsDlg::STAT_Pro_DelayJitter,
									CStatisticsDlg::STAT_Pro_AvgPER};
			SetCmd_ShowStatisticsOrder(cmd,node->nodeID,SrcAddr::SrcAddr_LocalDlg,DstAddr::DstAddr_ProtocolBoard,8,values);
			protocolCaper->sendPacket(0x00555555555555,HWND_Statistics,(u_char*)cmd,sizeof(Cmd_ShowStatisticsOrder));
			xdelete(cmd);
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
