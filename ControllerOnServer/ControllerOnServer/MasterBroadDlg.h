#pragma once
#include "afxwin.h"


// CMasterBroadDlg 对话框
class Node;
class MasterNode;
class CMasterBroadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMasterBroadDlg)

public:
	CMasterBroadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMasterBroadDlg();

// 对话框数据
	enum { IDD = IDD_MasterControl1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void initNodeInfo();
	MasterNode* node;

	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
public:
	CComboBox mCombo_selNetworkID;
};
