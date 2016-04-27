// StatisticsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "StatisticsDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "Node.h"
#include "WinpCapHelper.h"
#include "SoftwareCommend.h"


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
	, m_EditChannelEstimateStr(_T(""))
{

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
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CStatisticsDlg::OnUserRecvfromwinpcap)
END_MESSAGE_MAP()


// CStatisticsDlg 消息处理程序


BOOL CStatisticsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initSelEdits();
	caper->setMyHWnd(GetSafeHwnd(),node->nodeID,HWND_Statistics);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CStatisticsDlg::initSelEdits()
{
	CString notChosen=_T("--");
	bool a=!(mSelStats&STAT_Pro_NumGenMsgs);
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
	if(!(mSelStats&STAT_Base_ChannelEstimate))
	{
		m_EditChannelEstimateStr=notChosen;
	}
	_CEditSetText(IDC_EDIT_STAT_Link_Custom1,notChosen);
	_CEditSetText(IDC_EDIT_STAT_Link_Custom2,notChosen);
	_CEditSetText(IDC_EDIT_STAT_Link_Custom3,notChosen);
	UpdateData(FALSE);
}

void CStatisticsDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SoftwareCommend* cmd=new SoftwareCommend;
	SetSoftwareCommend(cmd,node->nodeID,Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,BTN_Close,0,NULL);
	caper->sendPacket(node->mAddrHex,HWND_Statistics,(u_char*)cmd,sizeof(SoftwareCommend));
	xdelete(cmd);
	caper->setMyHWnd(NULL,node->nodeID,HWND_Statistics);
	CDialogEx::OnClose();
}


afx_msg LRESULT CStatisticsDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
{
	unsigned char* data=(unsigned char*)lParam;
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)data;
	switch(header->function)
	{
	case Func_ShowStatisticsData:
		{
			CString valueStr;
			Cmd_ShowStatisticsData* cmd=(Cmd_ShowStatisticsData*)header;
			if(cmd->paramNum==2)
			{
				const UINT IDCs[]={IDC_EDIT_STAT_Link_FreqDeviation,IDC_EDIT_STAT_Link_ChannelEstimate};
				const CString FORMATS[]={_T("%u"),_T("%u")};
				for(int i=0;i<2;i++)
				{
					if(mSelStats&(STAT_Base_FreqDeviation<<i))
					{
						valueStr.Format(FORMATS[i],cmd->paramValue[i]);
						_CEditSetText(IDCs[i],valueStr);
					}
				}
			}
			else if(cmd->paramNum==8)
			{
				const UINT IDCs[]={IDC_EDIT_STAT_NumGeneratedMsgs,
				IDC_EDIT_STAT_NumSendedMsgs,
				IDC_EDIT_STAT_NumRecvdMsgs,
				IDC_EDIT_STAT_NumDropedMsgs,
				IDC_EDIT_STAT_NumAvgBusiness,
				IDC_EDIT_STAT_Delay,
				IDC_EDIT_STAT_DelayJitter,
				IDC_EDIT_STAT_AvgPER,
				};
				const CString FORMATS[]={_T("%.0f"),_T("%.0f"),_T("%.0f"),_T("%0.1f"),
										_T("%0.1f"),_T("%0.6f"),_T("%0.6f"),_T("%0.2f")};
				for(int i=0;i<8;i++)
				{
					if(mSelStats&(STAT_Pro_NumGenMsgs<<i))
					{
						valueStr.Format(FORMATS[i],cmd->paramValue[i]);
						_CEditSetText(IDCs[i],valueStr);
					}
				}
			}
			
		}
		break;
	default:
		break;
	}
	xdeletes(data);
	return 0;
}
