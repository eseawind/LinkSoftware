#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CBroadGrdDlg �Ի���
class MasterNode;
class PhyHelper;
class CBroadGrdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBroadGrdDlg)

public:
	CBroadGrdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBroadGrdDlg();

// �Ի�������
	enum { IDD = IDD_Broad_Ground_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	MasterNode* node;
	CFile mFile;
	CString fileNameStr;
	bool recvFile(unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char* data);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CListBox m_ListRecv;
	CListBox m_ListSend;
	CEdit m_EditSending;
	CStatic m_StaticPic;
	CProgressCtrl m_ProgressRecv;
	friend UINT recvFromPcie(LPVOID pParam);
	afx_msg void OnBnClickedButtonBroadgroundShowOpen();

protected:
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam);
};
