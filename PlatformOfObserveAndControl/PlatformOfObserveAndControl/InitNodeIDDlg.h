#pragma once
#include "afxwin.h"


// CInitNodeIDDlg �Ի���

class CInitNodeIDDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInitNodeIDDlg)

public:
	CInitNodeIDDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInitNodeIDDlg();

// �Ի�������
	enum { IDD = IDD_ConfigNodeID_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditNodeID;
	afx_msg void OnBnClickedButtonConfignodeidOk();
protected:
};
