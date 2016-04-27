#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CControlledBroadDlg �Ի���
class Node;
class SlaveNode;
class CControlledBroadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlledBroadDlg)

public:
	CControlledBroadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CControlledBroadDlg();

// �Ի�������
	enum { IDD = IDD_Controlled1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void initNodeInfo();
	SlaveNode* node;

	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CProgressCtrl m_ProgressSending;
	afx_msg void OnBnClickedButtonControlledFiledeliverBrowse();
	CEdit m_EditFilePath;
	afx_msg void OnBnClickedButtonControlledFiledeliverOpen();
	afx_msg void OnBnClickedButtonControlledAddinAddinnetwork();
	CComboBox mCombo_selNetworkID;
};
