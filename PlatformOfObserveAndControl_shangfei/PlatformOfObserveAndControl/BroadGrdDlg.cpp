// BroadGrdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "BroadGrdDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "Node.h"
#include "SoftwareCommend.h"
#include "WinpCapHelper.h"
#include "PhyHelper.h"


// CBroadGrdDlg 对话框

IMPLEMENT_DYNAMIC(CBroadGrdDlg, CDialogEx)

CBroadGrdDlg::CBroadGrdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBroadGrdDlg::IDD, pParent)
{

}

CBroadGrdDlg::~CBroadGrdDlg()
{
}

void CBroadGrdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BroadGround_Report_Receive, m_ListRecv);
	DDX_Control(pDX, IDC_LIST_BroadGround_Report_Send, m_ListSend);
	DDX_Control(pDX, IDC_EDIT_BroadGround_Report_Send, m_EditSending);
	DDX_Control(pDX, IDC_STATIC_BroadGround_Show_RecvPic, m_StaticPic);
	DDX_Control(pDX, IDC_PROGRESS_BroadGround_Show_RecvPic, m_ProgressRecv);
}


BEGIN_MESSAGE_MAP(CBroadGrdDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_BroadGround_Show_Open, &CBroadGrdDlg::OnBnClickedButtonBroadgroundShowOpen)
	ON_MESSAGE(WM_USER_RECVFROMWINPCAP, &CBroadGrdDlg::OnUserRecvfromwinpcap)
	ON_MESSAGE(WM_USER_RECVFROMPCIE, &CBroadGrdDlg::OnUserRecvfrompcie)
END_MESSAGE_MAP()


// CBroadGrdDlg 消息处理程序


BOOL CBroadGrdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	caper->setMyHWnd(GetSafeHwnd(),HWND_BroadGrd);
	caper->setMyAddr(node->mAddrHex);
	pcieSender->setHwnd(GetSafeHwnd(),HWND_BroadGrd);
	pcieReceiver->setHwnd(GetSafeHwnd(),HWND_BroadGrd);
	AfxBeginThread(recvFromPcie,this,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBroadGrdDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	caper->setMyHWnd(NULL,HWND_BroadGrd);
	pcieSender->setHwnd(NULL,HWND_BroadGrd);
	pcieReceiver->setHwnd(NULL,HWND_BroadGrd);
	CDialogEx::OnClose();
}


void CBroadGrdDlg::OnBnClickedButtonBroadgroundShowOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileFind finder;
	if(finder.FindFile(fileNameStr))
	{
		ShellExecute(NULL, _T("open"), fileNameStr, NULL, NULL, SW_SHOW);
	}
	else 
	{
		AfxMessageBox(_T("File not found"));
	}

}
bool CBroadGrdDlg::recvFile(unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char* data)
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
		m_ProgressRecv.SetRange32(0,_ttoi(fileLenStr));
		m_ProgressRecv.SetPos(0);
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
				m_ProgressRecv.SetPos(mFile.GetLength());
			}
			else
			{
				//SetThreadPriority(pcieReceiver->thread_rec,THREAD_PRIORITY_ABOVE_NORMAL);
				mFile.Write(recvData,frameSize);
				mFile.Flush();
				mFile.Close();
				stateStr=_T("文件接收完毕：")+mFile.GetFileName();
				AfxMessageBox(_T("文件接收完成"));
				return true;
			}
		}
	}
	return false;
}

afx_msg LRESULT CBroadGrdDlg::OnUserRecvfromwinpcap(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


afx_msg LRESULT CBroadGrdDlg::OnUserRecvfrompcie(WPARAM wParam, LPARAM lParam)
{
	if(pcieReceiver->qBuff.empty())
		return 0;
	//WaitForSingleObject(pcieReceiver->queue_Mutex, INFINITE);
	PACKET packet=pcieReceiver->qBuff.front();
	pcieReceiver->qBuff.pop();
	//ReleaseMutex(pcieReceiver->queue_Mutex);
	SoftwareCommendHeadr* header=(SoftwareCommendHeadr*)(packet.buf+1);
	switch(header->function)
	{
	case Func_DeliverConfig:
		{
			Cmd_DeliverConfig* cmd=(Cmd_DeliverConfig*)header;
			recvFile(cmd->frameID,cmd->frameSize,cmd->endFlag,cmd->data);
			return 0;
		}
		break;
	
	default:
		break;
	}
	//caper->sendPacket(0,HWND_Master,(u_char*)lParam,packet.len);
	return 0;
}

UINT recvFromPcie(LPVOID pParam)
{
	PACKET packet;
	CBroadGrdDlg* pDlg=(CBroadGrdDlg*)pParam;
	while(true)
	{
		WaitForSingleObject(pcieReceiver->sem_packet_rec,INFINITE);
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
			break;
		}
	}
	//caper->sendPacket(0,HWND_Master,(u_char*)lParam,packet.len);
	AfxEndThread(0);
	return 0;
}