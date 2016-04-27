#pragma once


// CBroadGrdDlg 对话框
class MasterNode;
class CBroadGrdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBroadGrdDlg)

public:
	CBroadGrdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBroadGrdDlg();

// 对话框数据
	enum { IDD = IDD_BROAD_GROUND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
