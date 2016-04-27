#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CControlledDlg 对话框
class Node;
class SlaveNode;
class PhyHelper;
class CControlledDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlledDlg)

public:
	CControlledDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControlledDlg();

// 对话框数据
	enum { IDD = IDD_Controlled };

//自定义
public:
	void initNodeInfo();
	SlaveNode* node;
	bool isAddedInNetwork;
	bool isSendingRecogInfo;
	bool isSendingMonitoringInfo;
	int count;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox mCombo_selNetworkID;
	CProgressCtrl mProgress_AddInNetwork;
	CButton mBtn_SendRecogInfo;
	CButton mBtn_sendMonitoringInfo;
	CButton mBtn_AddInNetwork;
	afx_msg void OnBnClickedButtonControlledAddinAddinnetwork();
	afx_msg void OnBnClickedButtonControlledRecognitionSend();
	afx_msg void OnBnClickedButtonControlledMonitoringSend();
protected:
	afx_msg LRESULT OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondAccpet();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondRefuse();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondCommand1();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondCommand2();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondCommand3();
	CComboBox m_ComboSelTerminal;
	afx_msg void OnClickedRadioControlledAirctrlMasterctrl();
	afx_msg void OnBnClickedRadioControlledAirctrlControlled();
	CButton m_BtnStartControl;
	CButton m_BtnTaskMsg1;
	CButton m_BtnTaskMsg2;
	CButton m_BtnTaskMsg3;
	CButton m_BtnTaskMsg4;
	afx_msg void OnBnClickedButtonControlledAirctrlMasterctrlStart();
	afx_msg void OnBnClickedButtonControlledAirctrlControlledPreparing();
	afx_msg void OnBnClickedButtonControlledAirctrlControlledDone();
	afx_msg void OnBnClickedButtonControlledAirctrlControlledRefuse();
	afx_msg void OnBnClickedButtonControlledAirctrlControlledUnable();
	CButton m_BtnStopControl;
	afx_msg void OnBnClickedButtonControlledAirctrlMasterctrlQuit();
	afx_msg void OnBnClickedButtonControlledNavigationStop();
	afx_msg void OnBnClickedButtonControlledNavigationStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_EditGrdPosStr;
	CString m_EditPlanePosStr;
	CString m_EditNaviQualStr;
};
