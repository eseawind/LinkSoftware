#pragma once
#include "afxwin.h"


// CMasterBroadDlg �Ի���
class Node;
class MasterNode;
class CMasterBroadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMasterBroadDlg)

public:
	CMasterBroadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMasterBroadDlg();

// �Ի�������
	enum { IDD = IDD_MasterControl1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
