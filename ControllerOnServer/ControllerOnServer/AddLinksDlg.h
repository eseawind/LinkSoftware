#pragma once
#include "afxwin.h"


// CAddLinksDlg �Ի���

class CAddLinksDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddLinksDlg)

public:
	CAddLinksDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddLinksDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_AddLinks };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
