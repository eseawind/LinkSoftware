#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CControlledBroadDlg 对话框
class Node;
class SlaveNode;
class CControlledBroadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlledBroadDlg)

public:
	CControlledBroadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControlledBroadDlg();

// 对话框数据
	enum { IDD = IDD_Controlled1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	SlaveNode* node;
	bool isAddedInNetwork;
	void initNodeInfo();
	bool isSendingRecogInfo;
	bool isSendingMonitoringInfo;
	int count;

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonControlledFiledeliverBrowse();
	afx_msg void OnBnClickedButtonControlledFiledeliverOpen();
	CEdit m_EditFilePath;
	CButton m_BtnBrowse;
	CProgressCtrl m_ProgressSending;
	CButton m_BtnSend;
protected:
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonControlledbroadAddinAddinnetwork();
	CButton mBtn_AddInNetwork;
	CComboBox mCombo_selNetworkID;
	CProgressCtrl m_ProgressAddIn;
	afx_msg void OnBnClickedButtonControlledRecognitionSend();
	afx_msg void OnBnClickedButtonControlledMonitoringSend();
	CButton mBtn_SendRecogInfo;
	CButton mBtn_sendMonitoringInfo;

	CString m_EditGrdPosStr;
	CString m_EditPlanePosStr;
	CString m_EditNaviQualStr;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonControlledNavigationStart();
	afx_msg void OnBnClickedButtonControlledNavigationStop();
	afx_msg void OnBnClickedRadioControlledAirctrlMasterctrl();
	afx_msg void OnBnClickedRadioControlledAirctrlControlled();

	CComboBox m_ComboSelTerminal;
	CButton m_BtnStartControl;
	CButton m_BtnTaskMsg1;
	CButton m_BtnTaskMsg2;
	CButton m_BtnTaskMsg3;
	CButton m_BtnTaskMsg4;
	CButton m_BtnStopControl;
	afx_msg void OnBnClickedButtonControlledAirctrlMasterctrlStart();
	afx_msg void OnBnClickedButtonControlledAirctrlMasterctrlQuit();
	afx_msg void OnBnClickedButtonControlledAirctrlControlledPreparing();
	afx_msg void OnBnClickedButtonControlledAirctrlControlledDone();
	afx_msg void OnBnClickedButtonControlledAirctrlControlledRefuse();
	afx_msg void OnBnClickedButtonControlledAirctrlControlledUnable();
};
