#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CBroadPlaneDlg 对话框
class SlaveNode;
class PhyHelper;
class CBroadPlaneDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBroadPlaneDlg)

public:
	CBroadPlaneDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBroadPlaneDlg();

// 对话框数据
	enum { IDD = IDD_Broad_Plane_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	SlaveNode* node;
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CListBox m_ListRecv;
	CListBox m_ListSend;
	CEdit m_EditSending;
	CStatic m_StaticPic;
	CEdit m_EditPath;
	CProgressCtrl m_ProgressSending;
	afx_msg void OnBnClickedButtonBroadplaneShowBrowse();
	afx_msg void OnBnClickedButtonBroadplaneShowSend();
};
