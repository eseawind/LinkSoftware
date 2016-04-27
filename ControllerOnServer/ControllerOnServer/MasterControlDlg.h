#pragma once


// CMasterControlDlg 对话框
class Node;
class MasterNode;
class CMasterControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMasterControlDlg)

public:
	CMasterControlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMasterControlDlg();

// 对话框数据
	enum { IDD = IDD_MasterControl };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void initNodeInfo();
	MasterNode* node;
	CComboBox mCombo_selNetwork;
	CComboBox mCombo_selRecognitionNodeID;
	CComboBox mCombo_selMonitoringNodeID;
	CString mEditStr_dstNodeID;
	CString mEditStr_dstNodeType;
	CString mEditStr_dstNodeVoiceNetID;
	CString mEditStr_dstNodeAirCtrlNetID;
	CString mEditStr_dstNodeNetworkState;
	CString mEditStr_dstNodeNaviQual;
	CString mEditStr_dstNodeEquipState;
	CString mEditStr_dstNodeFuelState;
	CString mEditStr_dstNodeAckState;
	CString mEditStr_dstNodeCustom1;
protected:
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonMasterctrlAddinStartguide();
	afx_msg void OnBnClickedButtonMasterctrlAddinAddedusers();
	afx_msg void OnBnClickedButtonMasterctrlAddinCloseguide();
	afx_msg void OnBnClickedButtonMasterctrlAddinClear();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand1();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand2();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand3();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand4();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand5();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand6();
};
