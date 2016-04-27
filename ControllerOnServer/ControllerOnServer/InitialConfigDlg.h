#pragma once
#include "afxwin.h"

#ifndef WM_MY_RefreshNodeInfoList
#define WM_MY_RefreshNodeInfoList	(WM_USER+101)
#endif
// CInitialConfigDlg 对话框

class CInitialConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInitialConfigDlg)

public:
	CInitialConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInitialConfigDlg();

// 对话框数据
	enum { IDD = IDD_NodeConfig_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOK();
	afx_msg void OnBnClickedButtonCancel();
	int numNodes;
	CString m_ComboNodeTypeStr;
	CString m_ComboTrackTypeStr;
	CString m_ComboVRuleStr;
	CString m_ComboMsgFormatStr;
	CString m_ComboRelayTranStr;
	CString m_ComboScheAlgoStr;
	CString m_ComboNetworkManageStr;
	CString m_ComboErrCtrlStr;
	CString m_ComboTimeSlotStr;
	CString m_ComboNumSlotStr;
	CString m_ComboNumNetworkStr;
	CString m_ComboSlotDistStr;
	CString m_ComboSlotValueStr;
	CString m_ComboLinkTypeStr;
	CString m_ComboServiceSrcStr;
	CString m_ComboLinkCustom1Str;
	CString m_ComboLinkCustom2Str;
	CString m_ComboLinkCustom3Str;
	CString m_ComboSrcStructStr;
	CString m_ComboEnDeCodeStr;
	CString m_ComboSymbolInterLeaverStr;
	CString m_ComboSpreadStr;
	CString m_ComboChipInterleaverStr;
	CString m_ComboModStr;
	CString m_ComboOutFrameFormatStr;
	CString m_ComboModImageStr;
	CString m_ComboBasebandCustomStr;
	CString m_EditNodeIDStr;
	CString m_EditIntfMACStr;
	CString m_EditInitPos_LongtStr;
	CString m_EditInitPos_LatStr;
	CString m_EditInitPos_HtStr;
	CString m_EditDstPos_LongtStr;
	CString m_EditDstPos_LatStr;
	CString m_EditDstPos_HtStr;
	CString m_EditVinfStr;
	CString m_EditVsupStr;
};
