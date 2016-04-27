#pragma once
#include "afxcmn.h"


// CSelectNetDeviceDlg 对话框

class CSelectNetDeviceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectNetDeviceDlg)

public:
	CSelectNetDeviceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectNetDeviceDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrlNetDevs;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOk();
	CListCtrl m_ListNetDev2;
};
