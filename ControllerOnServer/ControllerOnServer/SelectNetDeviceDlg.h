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
	enum { IDD = IDD_SelectNetDevice_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int* devID;
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrlNetDevs;
	afx_msg void OnBnClickedButtonOk();
};
