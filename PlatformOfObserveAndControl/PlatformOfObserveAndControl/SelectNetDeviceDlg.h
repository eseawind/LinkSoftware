#pragma once
#include "afxcmn.h"


// CSelectNetDeviceDlg �Ի���

class CSelectNetDeviceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectNetDeviceDlg)

public:
	CSelectNetDeviceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectNetDeviceDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrlNetDevs;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOk();
	CListCtrl m_ListNetDev2;
};
