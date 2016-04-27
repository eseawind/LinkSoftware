#pragma once
#include "afxwin.h"


// CInitNodeIDDlg 对话框

class CInitNodeIDDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInitNodeIDDlg)

public:
	CInitNodeIDDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInitNodeIDDlg();

// 对话框数据
	enum { IDD = IDD_ConfigNodeID_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditNodeID;
	afx_msg void OnBnClickedButtonConfignodeidOk();
protected:
};
