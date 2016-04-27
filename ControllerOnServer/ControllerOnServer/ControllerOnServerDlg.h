
// ControllerOnServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CControllerOnServerDlg �Ի���
class CControllerOnServerDlg : public CDialogEx
{
// ����
public:
	CControllerOnServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONTROLLERONSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNodeAdd();
	CListCtrl m_ListNodes;
	void initListCtrls();
	int numNodes;
	int devID;

protected:
	afx_msg LRESULT OnMyRefreshnodeinfolist(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnItemdblclickListNodes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListNodes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnClose();
protected:
	afx_msg LRESULT OnUserSelectednetdevice(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonLinkadd();
	afx_msg void OnBnClickedButtonSceneconfig();
protected:
};
