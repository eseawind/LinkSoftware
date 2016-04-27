#pragma once
#include "afxwin.h"


// CMasterControlDlg �Ի���
class Node;
class MasterNode;
class PhyHelper;

class CMasterControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMasterControlDlg)

public:
	CMasterControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMasterControlDlg();

// �Ի�������
	enum { IDD = IDD_MasterControl };
//�Զ���
public:
	void initNodeInfo();
	MasterNode* node;
	int count;
	friend UINT masterRecvFromPcie(LPVOID pParam);
	HANDLE m_hEventQuit;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
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
	afx_msg void OnBnClickedButtonMasterctrlAddinStartguide();
	afx_msg void OnBnClickedButtonMasterctrlAddinCloseguide();
	afx_msg void OnBnClickedButtonMasterctrlAddinClear();
protected:
	afx_msg LRESULT OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonMasterctrlAddinAddedusers();
	afx_msg void OnClose();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand1();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand2();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand3();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand4();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand5();
	afx_msg void OnBnClickedComboMasterctrlArmycommandCommand6();
	afx_msg void OnSelchangeComboMasterctrlRecognitionNodeid();
	CString m_EditGrdPosStr;
	CString m_EditPlanePosStr;
	CString m_EditNaviQualStr;
	afx_msg void OnBnClickedButtonMasterctrlNavigationStart();
	afx_msg void OnBnClickedButtonMasterctrlNavigationStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_ComboSelTerminal;
	afx_msg void OnBnClickedButtonMasterctrlAirctrlStart();
	afx_msg void OnBnClickedButtonMasterctrlAirctrlStop();
};
