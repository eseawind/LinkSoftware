#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CBroadPlaneDlg �Ի���
class SlaveNode;
class PhyHelper;
class CBroadPlaneDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBroadPlaneDlg)

public:
	CBroadPlaneDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBroadPlaneDlg();

// �Ի�������
	enum { IDD = IDD_Broad_Plane_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
