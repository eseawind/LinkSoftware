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
	enum { IDD = IDD_SelectNetDevice_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int* devID;
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrlNetDevs;
	afx_msg void OnBnClickedButtonOk();
};
