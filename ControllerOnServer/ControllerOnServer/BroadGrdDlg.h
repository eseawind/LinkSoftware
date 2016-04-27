#pragma once


// CBroadGrdDlg �Ի���
class MasterNode;
class CBroadGrdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBroadGrdDlg)

public:
	CBroadGrdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBroadGrdDlg();

// �Ի�������
	enum { IDD = IDD_BROAD_GROUND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	MasterNode* node;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CListBox m_ListRecv;
	CListBox m_ListSend;
	CEdit m_EditSending;
	CStatic m_StaticPic;
	CProgressCtrl m_ProgressRecv;
	afx_msg void OnBnClickedButtonBroadgroundShowOpen();
};
