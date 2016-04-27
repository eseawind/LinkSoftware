#pragma once
#include "afxwin.h"


// CAddLinksDlg 对话框

class CAddLinksDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddLinksDlg)

public:
	CAddLinksDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddLinksDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_AddLinks };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void initNodes();

	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CComboBox m_ComboNode1;
	CComboBox m_ComboNode2;
	CComboBox m_ComboLinkType;
	afx_msg void OnBnClickedButtonAddlinksOk();
};
