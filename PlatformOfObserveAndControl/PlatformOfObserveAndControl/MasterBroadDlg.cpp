// MasterBroadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "MasterBroadDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "SoftwareCommend.h"
#include "Node.h"
#include "WinpCapHelper.h"
#include "PhyHelper.h"


// CMasterBroadDlg 对话框

IMPLEMENT_DYNAMIC(CMasterBroadDlg, CDialogEx)

CMasterBroadDlg::CMasterBroadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMasterBroadDlg::IDD, pParent)
	, m_EditStrNodeID(_T(""))
	, m_EditStrNaviQual(_T(""))
	, count(0)
{
	m_hEventQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_EditStrAckState = _T("");
	m_EditStrAirCtrlNetID = _T("");
	m_EditStrEquipState = _T("");
	m_EditStrFuelState = _T("");
	m_EditStrNetworkState = _T("");
	m_EditStrNodeType = _T("");
	m_EditStrVoiceNetID = _T("");
}

CMasterBroadDlg::~CMasterBroadDlg()
{
}

void CMasterBroadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_Master_FileDeliver, m_ProgressReceiving);
	DDX_Control(pDX, IDC_STATIC_Master_FileDeliver_Show, m_StaticShowPic);
	DDX_Control(pDX, IDC_COMBO_MasterCtrl1_Recognition_NodeID, mCombo_SelRecognitedNodeID);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Recognition_NodeInfo_ID, m_EditStrNodeID);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Recognition_NodeInfo_NaviQuality, m_EditStrNaviQual);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Recognition_NodeInfo_AckState, m_EditStrAckState);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Recognition_NodeInfo_AirCtrlNetID, m_EditStrAirCtrlNetID);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Recognition_NodeInfo_EquipState, m_EditStrEquipState);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Recognition_NodeInfo_FuelState, m_EditStrFuelState);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Recognition_NodeInfo_NetworkState, m_EditStrNetworkState);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Recognition_NodeInfo_Type, m_EditStrNodeType);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Recognition_NodeInfo_VoiceNetID, m_EditStrVoiceNetID);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Navigation_StatePos, m_EditStrGrdPos);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Navigation_PlanePos, m_EditStrPlanePos);
	DDX_Text(pDX, IDC_EDIT_MasterCtrl1_Navigation_Quality, m_EditStrRelativeNaviQual);
}


BEGIN_MESSAGE_MAP(CMasterBroadDlg, CDialogEx)
	ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BUTTON_MasterBroad_AddIn_StartGuide, &CMasterBroadDlg::OnBnClickedButtonMasterbroadAddinStartguide)
ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CMasterBroadDlg::OnUserRecvfromwinpcap)
ON_MESSAGE(WM_USER_RECVFROMPCIE, &CMasterBroadDlg::OnUserRecvfrompcie)
ON_CBN_SELCHANGE(IDC_COMBO_MasterCtrl1_Recognition_NodeID, &CMasterBroadDlg::OnSelchangeComboMasterctrl1RecognitionNodeid)
ON_BN_CLICKED(IDC_BUTTON_MasterCtrl1_Navigation_Start, &CMasterBroadDlg::OnBnClickedButtonMasterctrl1NavigationStart)
ON_BN_CLICKED(IDC_BUTTON_MasterCtrl1_Navigation_Stop, &CMasterBroadDlg::OnBnClickedButtonMasterctrl1NavigationStop)
ON_WM_TIMER()
END_MESSAGE_MAP()


// CMasterBroadDlg 消息处理程序


BOOL CMasterBroadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initNodeInfo();
	mCombo_SelRecognitedNodeID.SetCurSel(0);
	OnSelchangeComboMasterctrl1RecognitionNodeid();
	OnBnClickedButtonMasterctrl1NavigationStop();
	serverCaper->setMyHWnd(GetSafeHwnd(),HWND_BroadGrd);
	serverCaper->setMyAddr(node->mAddrHex);
	pcieSender->setHwnd(GetSafeHwnd(),HWND_BroadGrd);
	pcieReceiver->setHwnd(GetSafeHwnd(),HWND_BroadGrd);
	//AfxBeginThread(recvFromPcie,this,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMasterBroadDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	serverCaper->setMyHWnd(NULL,HWND_BroadGrd);
	pcieSender->setHwnd(NULL,HWND_BroadGrd);
	pcieReceiver->setHwnd(NULL,HWND_BroadGrd);
	SetEvent(m_hEventQuit);
	CDialogEx::OnClose();
}


void CMasterBroadDlg::initNodeInfo()
{
	CString tempStr;

	//节点配置信息
	//节点类型
	_CEditSetText(IDC_EDIT_MasterBroad_NodeInfo_Type,node->nodeType);

	//节点ID
	tempStr.Format(_T("%d"),node->nodeID);
	_CEditSetText(IDC_EDIT_MasterBroad_NodeInfo_ID,tempStr);

	//燃油状态
	_CEditSetText(IDC_EDIT_MasterBroad_NodeInfo_FuelState,_T("正常"));

	//装备状态
	_CEditSetText(IDC_EDIT_MasterBroad_NodeInfo_EquipState,_T("正常"));

	//工作状态
	_CEditSetText(IDC_EDIT_MasterBroad_NodeInfo_WorkState,_T("正常"));

	//工作频段
	_CEditSetText(IDC_EDIT_MasterBroad_NodeInfo_FreqRange,_T("正常"));
}

bool CMasterBroadDlg::recvFile(unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char* data)
{
	char* recvData=(char*) data;
	CString stateStr;
	if(frameID==0)
	{
		//SetThreadPriority(pcieReceiver->thread_rec,THREAD_PRIORITY_HIGHEST);
		CString tempStr,fileNameStr,fileLenStr;
		tempStr=recvData;
		int index=tempStr.Find('*');
		fileNameStr=tempStr.Mid(0,index);
		fileNameStr=_T(".\\")+fileNameStr;
		if(!mFile.Open(fileNameStr,CFile::modeCreate|CFile::modeWrite))
		{
			AfxMessageBox(_T("创建配置文件失败！"));
			return true;
		}
		stateStr=_T("开始接收文件：")+fileNameStr;
		fileLenStr=tempStr.Mid(index+1);
		this->fileNameStr=fileNameStr;
		unsigned int fileSize=_ttoi(fileLenStr);
		m_ProgressReceiving.SetRange32(0,fileSize);
		m_ProgressReceiving.SetPos(0);

		//SoftwareCommend* tCmd=new SoftwareCommend;
		//SetSoftwareCommend(tCmd,0,Function::Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,Pro_FileSize,1,&fileSize);
		//serverCaper->sendPacket(0,HWND_BroadGrd,(u_char*)tCmd,sizeof(SoftwareCommend));
		//xdelete(tCmd);
	}
	else if(frameID>0)
	{
		if(mFile.m_hFile!=CFile::hFileNull)
		{
			if(endFlag==0)
			{
				mFile.Write(recvData,frameSize);
				mFile.Flush();
				stateStr=_T("文件接收中：")+mFile.GetFileName();
				unsigned int fileLen=mFile.GetLength();
				m_ProgressReceiving.SetPos(fileLen);

				//SoftwareCommend* tCmd=new SoftwareCommend;
				//SetSoftwareCommend(tCmd,0,Function::Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,Pro_FileRecved,1,&fileLen);
				//serverCaper->sendPacket(0,HWND_BroadGrd,(u_char*)tCmd,sizeof(SoftwareCommend));
				//xdelete(tCmd);
			}
			else
			{
				//SetThreadPriority(pcieReceiver->thread_rec,THREAD_PRIORITY_ABOVE_NORMAL);
				mFile.Write(recvData,frameSize);
				mFile.Flush();
				mFile.Close();
				stateStr=_T("文件接收完毕：")+mFile.GetFileName();
				showPic(_T(".\\RMB100.jpg"));
				//AfxMessageBox(_T("文件接收完成"));
				return true;
			}
		}
	}
	return false;
}

UINT recvFromPcie(LPVOID pParam)
{
	bool loop=true;
	PACKET packet;
	CMasterBroadDlg* pDlg=(CMasterBroadDlg*)pParam;
	HANDLE handles[] = {pcieReceiver->sem_packet_rec, pDlg->m_hEventQuit};
	while(loop)
	{
		switch(WaitForMultipleObjects(2, handles, FALSE, INFINITE))
			{
			case WAIT_OBJECT_0:
				{
					//WaitForSingleObject(pcieReceiver->queue_Mutex, INFINITE);
					packet=pcieReceiver->qBuff.front();
					pcieReceiver->qBuff.pop();
					//ReleaseMutex(pcieReceiver->queue_Mutex);
					SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)(packet.buf+1);
					switch(header->function)
					{
					case Func_DeliverConfig:
						{
							Cmd_DeliverConfig* cmd=(Cmd_DeliverConfig*)header;
							pDlg->recvFile(cmd->frameID,cmd->frameSize,cmd->endFlag,cmd->data);
						}
						break;
					default:
						::SendMessage(pcieReceiver->hWnd[packet.buf[0]],WM_USER_RECVFROMPCIE,NULL,(LPARAM)&packet);
						break;
					}
				}
				break;
			default:
				loop = false;
				break;
			}
	}
	//caper->sendPacket(0,HWND_Master,(u_char*)lParam,packet.len);
	AfxEndThread(0);
	return 0;
}

void CMasterBroadDlg::OnBnClickedButtonMasterbroadAddinStartguide()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CMasterBroadDlg::showPic(const CString& path)
{
	CImage img;
	HRESULT hImg;
	HDC hPicDC=m_StaticShowPic.GetDC()->m_hDC;
	CRect rect;
	m_StaticShowPic.GetClientRect(&rect);
	hImg=img.Load(path);
	if(FAILED(hImg))
	{
		return;
	}
	img.Draw(hPicDC,rect,Gdiplus::InterpolationMode::InterpolationModeHighQuality);
}

afx_msg LRESULT CMasterBroadDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
{
	unsigned char* data=(unsigned char*)lParam;
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)lParam;
	switch(header->function)
	{
	case Func_Cmd:
	{
		SoftwareCommend* cmd=(SoftwareCommend*)header;

		switch(cmd->buttonID)
		{
		case BTN_Close:
		{
			SendMessage(WM_CLOSE);
		}
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
	xdeletes(data);
	return 0;
}


afx_msg LRESULT CMasterBroadDlg::OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam)
{
	PACKET packet=*(PACKET*)lParam;
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)(packet.buf+1);
	switch(header->function)
	{
	case Func_Cmd:
	{

	}
		break;
	case Func_ProtocolBusiness:
	{
		Cmd_ProtoclBusinessHeader* businessHeader=(Cmd_ProtoclBusinessHeader*)header;
		switch(businessHeader->type)
		{
		case Business_AddIn:
		{
			Busi_AddIn* cmd=(Busi_AddIn*)businessHeader;
			serverCaper->sendPacket(0,HWND_BroadGrd,(u_char*)cmd,sizeof(Busi_AddIn));
			switch(cmd->action)
			{
			case Reaction_AddIn:
			{
				CString msg;
				node->networks.addIn(cmd->networkID,cmd->nodeID);
				msg.Format(_T("节点%d加入网络%d"),cmd->nodeID,cmd->networkID);
				AfxMessageBox(msg);
			}
				break;
			case Reaction_Exit:
			{
				CString msg;
				node->networks.quit(cmd->networkID,cmd->nodeID);
				msg.Format(_T("节点%d退出网络%d"),cmd->nodeID,cmd->networkID);
				AfxMessageBox(msg);
			}
				break;
			default:
				break;
			}
		}
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
	//caper->sendPacket(0,HWND_Controlled,(u_char*)header,packet.len);
	return 0;
}


void CMasterBroadDlg::OnSelchangeComboMasterctrl1RecognitionNodeid()
{
	// TODO: 在此添加控件通知处理程序代码
	int nodeID=mCombo_SelRecognitedNodeID.GetCurSel()+1;
	m_EditStrNodeID.Format(_T("%d"),nodeID);
	if(nodeID==1||nodeID==3)
	{
		m_EditStrNodeType=_T("地面端");
	}
	else
	{
		m_EditStrNodeType=_T("飞机");
	}
	m_EditStrVoiceNetID=_T("0");
	m_EditStrAirCtrlNetID=_T("0");
	m_EditStrNetworkState=_T("正常");
	m_EditStrNaviQual=_T("良好");
	m_EditStrEquipState=_T("正常");
	m_EditStrFuelState=_T("正常");
	m_EditStrAckState=_T("待命");
	UpdateData(FALSE);
}


void CMasterBroadDlg::OnBnClickedButtonMasterctrl1NavigationStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditStrGrdPos=node->trackInfo.startPosStr;
	m_EditStrPlanePos.Format(_T("(%d,%d,%d)"),237*(count++),43*(count++),880);
	m_EditStrRelativeNaviQual=_T("良好");
	UpdateData(FALSE);
	SetTimer(0,10000,NULL);
}


void CMasterBroadDlg::OnBnClickedButtonMasterctrl1NavigationStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditStrGrdPos=_T("--");
	m_EditStrPlanePos=_T("--");
	m_EditStrRelativeNaviQual=_T("--");
	UpdateData(FALSE);
	KillTimer(0);
}


void CMasterBroadDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 0:
		{
			m_EditStrGrdPos=node->trackInfo.startPosStr;
			m_EditStrPlanePos.Format(_T("(%d,%d,%d)"),237*(count++),43*(count++),880);
			m_EditStrRelativeNaviQual=_T("良好");
			UpdateData(FALSE);
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
