#pragma once


// CControlledDlg 对话框
class Node;
class SlaveNode;
class CControlledDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlledDlg)

public:
	CControlledDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControlledDlg();

// 对话框数据
	enum { IDD = IDD_Controlled };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void initNodeInfo();
	SlaveNode* node;
	bool isAddedInNetwork;
	bool isSendingRecogInfo;
	bool isSendingMonitoringInfo;
	CComboBox mCombo_selNetworkID;
	CProgressCtrl mProgress_AddInNetwork;
	CButton mBtn_SendRecogInfo;
	CButton mBtn_sendMonitoringInfo;
	CButton mBtn_AddInNetwork;
	afx_msg void OnBnClickedButtonControlledAddinAddinnetwork();
	afx_msg void OnBnClickedButtonControlledRecognitionSend();
	afx_msg void OnBnClickedButtonControlledMonitoringSend();
protected:
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondAccpet();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondRefuse();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondCommand1();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondCommand2();
	afx_msg void OnBnClickedButtonControlledArmycommandRespondCommand3();
};
