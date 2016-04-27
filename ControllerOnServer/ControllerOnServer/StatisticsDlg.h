#pragma once
#include "afxwin.h"


// CStatisticsDlg 对话框
class MasterNode;
class CStatisticsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatisticsDlg)

public:
	CStatisticsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStatisticsDlg();

// 对话框数据
	enum { IDD = IDD_STATISTICS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	enum SelStats
	{
		STAT_None				=	0x0000,

		STAT_Pro_NumGenMsgs		=	0x0001,
		STAT_Pro_NumSndMsgs		=	0x0002,
		STAT_Pro_NumRcvMsgs		=	0x0004,
		STAT_Pro_NumDrpMsgs		=	0x0008,
		STAT_Pro_NumAvgBusiness	=	0x0010,
		STAT_Pro_Delay			=	0x0020,
		STAT_Pro_DelayJitter	=	0x0040,
		STAT_Pro_AvgPER			=	0x0080,

		STAT_Base_FreqDeviation	=	0x0100,
		STAT_Base_ChannelEstimate	=	0x0200,

		STAT_All				=	0xff,
	};
	int mSelStats;
	MasterNode* node;
	void initSelEdits();
	CString mEditStr_numGenMsgs;
	CString mEditStr_numSndMsgs;
	CString mEditStr_numRecvMsgs;
	CString mEditStr_numDrpMsgs;
	CString mEditStr_numAvgBusiness;
	CString mEditStr_delay;
	CString mEditStr_delayJitter;
	CString mEditStr_avgPER;
	CString m_EditFreqDeviationStr;
	afx_msg void OnClose();
	CString m_EditChannelEstimateStr;
protected:
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
};
