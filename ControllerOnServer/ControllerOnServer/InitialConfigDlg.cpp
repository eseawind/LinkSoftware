// InitialConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControllerOnServer.h"
#include "InitialConfigDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "WinpCaper.h"

// CInitialConfigDlg 对话框

IMPLEMENT_DYNAMIC(CInitialConfigDlg, CDialogEx)

CInitialConfigDlg::CInitialConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInitialConfigDlg::IDD, pParent)
	, numNodes(0)
	, m_ComboNodeTypeStr(_T(""))
	, m_ComboTrackTypeStr(_T(""))
	, m_ComboVRuleStr(_T(""))
	, m_ComboMsgFormatStr(_T(""))
	, m_ComboRelayTranStr(_T(""))
	, m_ComboScheAlgoStr(_T(""))
	, m_ComboNetworkManageStr(_T(""))
	, m_ComboErrCtrlStr(_T(""))
	, m_ComboTimeSlotStr(_T(""))
	, m_ComboNumSlotStr(_T(""))
	, m_ComboNumNetworkStr(_T(""))
	, m_ComboSlotDistStr(_T(""))
	, m_ComboSlotValueStr(_T(""))
	, m_ComboLinkTypeStr(_T(""))
	, m_ComboServiceSrcStr(_T(""))
	, m_ComboLinkCustom1Str(_T(""))
	, m_ComboLinkCustom2Str(_T(""))
	, m_ComboLinkCustom3Str(_T(""))
	, m_ComboSrcStructStr(_T(""))
	, m_ComboEnDeCodeStr(_T(""))
	, m_ComboSymbolInterLeaverStr(_T(""))
	, m_ComboSpreadStr(_T(""))
	, m_ComboChipInterleaverStr(_T(""))
	, m_ComboModStr(_T(""))
	, m_ComboOutFrameFormatStr(_T(""))
	, m_ComboModImageStr(_T(""))
	, m_ComboBasebandCustomStr(_T(""))
	, m_EditNodeIDStr(_T("0"))
	, m_EditIntfMACStr(_T(""))
	, m_EditInitPos_LongtStr(_T(""))
	, m_EditInitPos_LatStr(_T(""))
	, m_EditInitPos_HtStr(_T(""))
	, m_EditDstPos_LongtStr(_T(""))
	, m_EditDstPos_LatStr(_T(""))
	, m_EditDstPos_HtStr(_T(""))
	, m_EditVinfStr(_T(""))
	, m_EditVsupStr(_T(""))
{

}

CInitialConfigDlg::~CInitialConfigDlg()
{
}

void CInitialConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_Config_Node_Type, m_ComboNodeTypeStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Track_Type, m_ComboTrackTypeStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Track_VRule, m_ComboVRuleStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Protocol_MsgFormat, m_ComboMsgFormatStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Protocol_RelayTran, m_ComboRelayTranStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Protocol_ScheAlgo, m_ComboScheAlgoStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Protocol_NetworkManage, m_ComboNetworkManageStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Protocol_ErrCtrl, m_ComboErrCtrlStr);
	DDX_CBString(pDX, IDC_COMBO_Config_TimeSeq_TimeSlot, m_ComboTimeSlotStr);
	DDX_CBString(pDX, IDC_COMBO_Config_TimeSeq_NumSlot, m_ComboNumSlotStr);
	DDX_CBString(pDX, IDC_COMBO_Config_TimeSeq_NumNetwork, m_ComboNumNetworkStr);
	DDX_CBString(pDX, IDC_COMBO_Config_TimeSeq_SlotDist, m_ComboSlotDistStr);
	DDX_CBString(pDX, IDC_COMBO_Config_TimeSeq_SlotValue, m_ComboSlotValueStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Link_Type, m_ComboLinkTypeStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Link_ServiceSrc, m_ComboServiceSrcStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Link_Custom1, m_ComboLinkCustom1Str);
	DDX_CBString(pDX, IDC_COMBO_Config_Link_Custom2, m_ComboLinkCustom2Str);
	DDX_CBString(pDX, IDC_COMBO_Config_Link_Custom3, m_ComboLinkCustom3Str);
	DDX_CBString(pDX, IDC_COMBO_Config_Baseband_SrcFrameStruct, m_ComboSrcStructStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Baseband_EnDeCode, m_ComboEnDeCodeStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Baseband_SymbolInterDeLeaver, m_ComboSymbolInterLeaverStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Baseband_DeSpreading, m_ComboSpreadStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Baseband_ChipInterDeLeaver, m_ComboChipInterleaverStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Baseband_DeMod, m_ComboModStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Baseband_OutFrameStruct, m_ComboOutFrameFormatStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Baseband_ModPattern, m_ComboModImageStr);
	DDX_CBString(pDX, IDC_COMBO_Config_Baseband_Custom, m_ComboBasebandCustomStr);
	DDX_Text(pDX, IDC_EDIT_Config_Node_ID, m_EditNodeIDStr);
	DDX_Text(pDX, IDC_EDIT_Config_Node_IntfMAC, m_EditIntfMACStr);
	DDX_Text(pDX, IDC_EDIT_Config_Track_InitLongt, m_EditInitPos_LongtStr);
	DDX_Text(pDX, IDC_EDIT_Config_Track_InitLat, m_EditInitPos_LatStr);
	DDX_Text(pDX, IDC_EDIT_Config_Track_InitHt, m_EditInitPos_HtStr);
	DDX_Text(pDX, IDC_EDIT_Config_Track_DstLongt, m_EditDstPos_LongtStr);
	DDX_Text(pDX, IDC_EDIT_Config_Track_DstLat, m_EditDstPos_LatStr);
	DDX_Text(pDX, IDC_EDIT_Config_Track_DstHt, m_EditDstPos_HtStr);
	DDX_Text(pDX, IDC_EDIT_Config_Track_Vinf, m_EditVinfStr);
	DDX_Text(pDX, IDC_EDIT_Config_Track_Vsup, m_EditVsupStr);
}


BEGIN_MESSAGE_MAP(CInitialConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CInitialConfigDlg::OnBnClickedButtonOK)
	ON_BN_CLICKED(IDC_BUTTON_Cancel, &CInitialConfigDlg::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CInitialConfigDlg 消息处理程序


BOOL CInitialConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EditIntfMACStr=_T("FF:FF:FF:FF:FF:FF");
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Node_Type))->SetCurSel(0);
	((CEdit*)GetDlgItem(IDC_EDIT_Config_Node_ID))->SetWindowText(m_EditNodeIDStr);
	((CEdit*)GetDlgItem(IDC_EDIT_Config_Node_IntfMAC))->SetWindowText(m_EditIntfMACStr);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Track_Type))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Track_VRule))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Protocol_MsgFormat))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Protocol_RelayTran))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Protocol_ScheAlgo))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Protocol_NetworkManage))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Protocol_ErrCtrl))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_TimeSeq_TimeSlot))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_TimeSeq_NumSlot))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_TimeSeq_NumNetwork))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_TimeSeq_SlotDist))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_TimeSeq_SlotValue))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Link_Type))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Link_ServiceSrc))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Link_Custom1))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Link_Custom2))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Link_Custom3))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Baseband_SrcFrameStruct))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Baseband_EnDeCode))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Baseband_SymbolInterDeLeaver))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Baseband_DeSpreading))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Baseband_ChipInterDeLeaver))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Baseband_DeMod))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Baseband_OutFrameStruct))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Baseband_ModPattern))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_Config_Baseband_Custom))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CInitialConfigDlg::OnBnClickedButtonOK()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	const CString notFound=_T("");
	const CString sectionNode=_T("NodeInfo");
	const CString sectionTrack=_T("TrackInfo");
	const CString sectionProtocol=_T("Protocol");
	const CString sectionTimeSeq=_T("TimeSeqence");
	const CString sectionLink=_T("Link");
	const CString sectionBaseband=_T("Baseband");
	CFileFind finder;
	
	m_EditNodeIDStr.Trim();
	CString configPath=_T(".\\config[")+m_EditNodeIDStr+_T("].ini");//获取文件名
	if(!finder.FindFile(configPath))
	{
		CFile fileConfig;
		try
		{
			fileConfig.Open(configPath,CFile::modeCreate|CFile::modeWrite);//如没有文件则新建一个
			fileConfig.Close();
		}
		catch(CFileException *e)
		{
			CString errStr;
			errStr.Format(_T("文件创建失败:%d"),e->m_cause);
			MessageBox(errStr);
			fileConfig.Abort();
			e->Delete();
		}
	}
	CString valueStr;
	
	//NodeInfo
	//NodeType
	::WritePrivateProfileString(sectionNode,_T("NodeType"),m_ComboNodeTypeStr,configPath);
	//NodeID
	::WritePrivateProfileString(sectionNode,_T("NodeID"),m_EditNodeIDStr,configPath);
	//IntfMac
	CString intfMAC;
	intfMAC.Format(_T("%d"),0x000000000001+_ttoi(m_EditNodeIDStr));
	::WritePrivateProfileString(sectionNode,_T("IntfMac"),intfMAC,configPath);

	//TrackInfo
	float Longt,Lat,Ht;
	CString PositionStr;
	//InitialPosition
	Longt=_ttof(m_EditInitPos_LongtStr);
	Lat=_ttof(m_EditInitPos_LatStr);
	Ht=_ttof(m_EditInitPos_HtStr);
	PositionStr.Format(_T("(%.2f,%.2f,%.2f)"),Longt,Lat,Ht);
	::WritePrivateProfileString(sectionTrack,_T("InitialPositon"),PositionStr,configPath);
	//DestinatePosition
	Longt=_ttof(m_EditDstPos_LongtStr);
	Lat=_ttof(m_EditDstPos_LatStr);
	Ht=_ttof(m_EditDstPos_HtStr);
	PositionStr.Format(_T("(%.2f,%.2f,%.2f)"),Longt,Lat,Ht);
	::WritePrivateProfileString(sectionTrack,_T("DestinatePositon"),PositionStr,configPath);
	//TrackType
	::WritePrivateProfileString(sectionTrack,_T("TrackType"),m_ComboTrackTypeStr,configPath);
	//VelocityRange
	float Vinf=_ttof(m_EditVinfStr);
	float Vsup=_ttof(m_EditVsupStr);
	CString VStr;
	VStr.Format(_T("(%.2f,%.2f"),Vinf,Vsup);
	::WritePrivateProfileString(sectionTrack,_T("Velocity"),VStr,configPath);
	//RuleOfVelocityChange
	::WritePrivateProfileString(sectionTrack,_T("VRule"),m_ComboVRuleStr,configPath);
		
	//ProtocolInfo
	//MessageFormat
	::WritePrivateProfileString(sectionProtocol,_T("MessageFormat"),m_ComboMsgFormatStr,configPath);
	::WritePrivateProfileString(sectionProtocol,_T("isRelayTransmission"),m_ComboRelayTranStr,configPath);
	//ScheduleAlgorithm
	::WritePrivateProfileString(sectionProtocol,_T("ScheduleAlgorithm"),m_ComboScheAlgoStr,configPath);
	//NetworkManage
	::WritePrivateProfileString(sectionProtocol,_T("NetworkManage"),m_ComboNetworkManageStr,configPath);
	//isErrorControl
	::WritePrivateProfileString(sectionProtocol,_T("isErrorControl"),m_ComboErrCtrlStr,configPath);

	//TimeSequenceInfo
	//TimeSlot
	::WritePrivateProfileString(sectionTimeSeq,_T("TimeSlot"),m_ComboTimeSlotStr,configPath);
	//numSlots
	::WritePrivateProfileString(sectionTimeSeq,_T("numSlots"),m_ComboNumSlotStr,configPath);
	//numNetworks
	::WritePrivateProfileString(sectionTimeSeq,_T("numNetworks"),m_ComboNumNetworkStr,configPath);
	//SlotsDistribution
	::WritePrivateProfileString(sectionTimeSeq,_T("SlotsDistribution"),m_ComboSlotDistStr,configPath);
	//SubSlotValue
	::WritePrivateProfileString(sectionTimeSeq,_T("SubSlotValue"),m_ComboSlotValueStr,configPath);

	//LinkInfo
	//LinkType
	::WritePrivateProfileString(sectionLink,_T("LinkType"),m_ComboLinkTypeStr,configPath);
	//ServiceSource
	::WritePrivateProfileString(sectionLink,_T("ServiceSource"),m_ComboServiceSrcStr,configPath);

	numNodes=_ttoi(m_EditNodeIDStr);
	m_EditNodeIDStr.Format(_T("%d"),++numNodes);
	_CEditSetText(IDC_EDIT_Config_Node_ID,m_EditNodeIDStr);
}


void CInitialConfigDlg::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(GetParent()->GetSafeHwnd(),WM_MY_RefreshNodeInfoList,NULL,NULL);
	SendMessage(WM_CLOSE);
}
