#pragma once
#include "afxcmn.h"
#include "afxwin.h"


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
