#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMasterBroadDlg 对话框
class Node;
class MasterNode;
class CMasterBroadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMasterBroadDlg)

public:
	CMasterBroadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMasterBroadDlg();

// 对话框数据
	enum { IDD = IDD_MasterControl1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	MasterNode* node;
	CFile mFile;
	CString fileNameStr;
	HANDLE m_hEventQuit;
	int count;

	void initNodeInfo();
	bool recvFile(unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char* data);
	friend UINT recvFromPcie(LPVOID pParam);
	void showPic(const CString& path);

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CProgressCtrl m_ProgressReceiving;
	afx_msg void OnBnClickedButtonMasterbroadAddinStartguide();
	CStatic m_StaticShowPic;
protected:
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam);
public:
	CComboBox mCombo_SelRecognitedNodeID;
	CString m_EditStrNodeID;
	CString m_EditStrNaviQual;
	CString m_EditStrAckState;
	CString m_EditStrAirCtrlNetID;
	CString m_EditStrEquipState;
	CString m_EditStrFuelState;
	CString m_EditStrNetworkState;
	CString m_EditStrNodeType;
	CString m_EditStrVoiceNetID;
	afx_msg void OnSelchangeComboMasterctrl1RecognitionNodeid();
	afx_msg void OnBnClickedButtonMasterctrl1NavigationStart();
	afx_msg void OnBnClickedButtonMasterctrl1NavigationStop();
	CString m_EditStrGrdPos;
	CString m_EditStrPlanePos;
	CString m_EditStrRelativeNaviQual;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
