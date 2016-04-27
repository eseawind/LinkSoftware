
// PlatformOfObserveAndControlDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

class Node;
class PhyHelper;
class WinpCapHelper;

// CPlatformOfObserveAndControlDlg 对话框
class ProtocolStatistics
{
public:
	float paramValue[8];
	unsigned int curr_time;
	unsigned int last_time;
	unsigned int init_time;
	unsigned int pktRate;
public:
	ProtocolStatistics();
	void setNetworkType(int isBroad);
	void update();
};
extern ProtocolStatistics stat;
class CPlatformOfObserveAndControlDlg : public CDialogEx
{
// 构造
public:
	CPlatformOfObserveAndControlDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CPlatformOfObserveAndControlDlg();
// 对话框数据
	enum { IDD = IDD_PLATFORMOFOBSERVEANDCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

//自定义
public:
	Node* node;
	HANDLE m_hEventQuit;
	CFile fileDataSrc;
	CFile fileConfig;
	void* pDlg;
	void initNodeInfo();
	bool isSelfTesting;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void recvFromWpCaper(const u_char* data,unsigned int len);
	void recvFile(unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char* data);
	friend UINT mainRecvFromPcie(LPVOID pParam);
	//void selectNetDevice();
	CComboBox mCombo_businessSel;
	CButton mBtn_businessStart;
	CButton mBtn_changeConfig;
	CComboBox mCombo_layerSel;
	CComboBox mCombo_spotSel;
	CComboBox mCombo_funcSel;
	CEdit mEdit_dataSrcPath;
	CEdit mEdit_numModeMulti;
	CButton mBtn_browse;
	CButton mBtn_applySettings;
	afx_msg void OnBnClickedButtonMainObsctlBrowse();
	afx_msg void OnBnClickedButtonMainBusinessEditconfig();
	afx_msg void OnBnClickedButtonMainNodeinfoRefresh();
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedButtonMainBusinessStart();
	CStatic mStatic_State;
	afx_msg void OnBnClickedButtonMainObsctlApply();
	CComboBox m_ComboChannelEncode;
	CComboBox m_ComboSpreadAlgo;
	CComboBox m_ComboModMode;
	CComboBox m_ComboBasebandCustom;
	CComboBox m_ComboFrameFormat;
	CComboBox m_ComboLinkManage;
	CComboBox m_ComboRsrcDist;
	CComboBox m_ComboLinkCustom;
	CComboBox m_ComboInterveneProtocolParam;
	CComboBox m_ComboInterveneProtocolValue;
	CComboBox m_ComboInterveneBasebandParam;
	CComboBox m_ComboInterveneBasebandValue;
	afx_msg void OnBnClickedButtonMainRecontructionBasebandRun();
	afx_msg void OnBnClickedButtonMainRecontructionLinkRun();
	afx_msg void OnBnClickedButtonMainInterveneProtocolRun();
	afx_msg void OnBnClickedButtonMainInterveneBasebandRun();
protected:
	afx_msg LRESULT OnUserHaveselecteddevice(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserConfignodeid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonReset();
};
