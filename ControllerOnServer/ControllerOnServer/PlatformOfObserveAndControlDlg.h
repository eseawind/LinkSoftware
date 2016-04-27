#pragma once


// CPlatformOfObserveAndControlDlg 对话框
class Node;
class WinpCapHelper;

class CPlatformOfObserveAndControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlatformOfObserveAndControlDlg)

public:
	CPlatformOfObserveAndControlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlatformOfObserveAndControlDlg();

// 对话框数据
	enum { IDD = IDD_PLATFORMOFOBSERVEANDCONTROL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void initNodeInfo();
	void recvFromWpCaper(const u_char* data,unsigned int len);
	Node* node;
	//WinpCapHelper* caper;
	CFile fileDataSrc; 
	CFile fileConfig;
	int devID;

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
	afx_msg void OnBnClickedButtonMainInterveneProtocolRun();
	afx_msg void OnBnClickedButtonMainInterveneBasebandRun();
	afx_msg void OnBnClickedButtonMainRecontructionBasebandRun();
	afx_msg void OnBnClickedButtonMainRecontructionLinkRun();
protected:
	afx_msg LRESULT OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
};
