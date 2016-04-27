#include "StdAfx.h"
#include "PhyHelper.h"
#include "api.h"

#include "Globe.h"
#include "SoftwareCommend.h"
#include "PlatformOfObserveAndControl.h"
#include "ControlledBroadDlg.h"
#include "WinpCapHelper.h"
using namespace std;

CRITICAL_SECTION cs_DMA;
unsigned int zero=0;
unsigned int one=1;
unsigned int DAconfig=0x000001F9;
/************************************************************************/
/* DMA                                                                     */
/************************************************************************/
PhyHelper::PhyHelper(Direction dir)
	: m_hDev(NULL)
	, m_uLen(1048576)
	, m_bStarted(FALSE)
	, m_nCount(100)
	, VID(0x5266)
	, m_dir(dir)
	, sem_packet_rec(NULL)
	, sem_packet_send(NULL)//用于告知发送线程，我的包已经发送给发送线程了
	, queue_Mutex(NULL)
	, queueSend_Mutex(NULL)
	, pcie_Mutex(NULL)
{
	InitializeCriticalSection(&cs_DMA);//初始化临界区
	if(dir==Dir_Up)
	{
		DID=0xF1205;
	}
	else if(dir==Dir_Down)
	{
		DID=0xC1205;
	}
	m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
}

PhyHelper::~PhyHelper(void)
{
	PcieClose();
	DeleteCriticalSection(&cs_DMA);//删除临界区
}
void PhyHelper::sendPacket(unsigned int dstHwnd, const unsigned char* data,int len)
{
	//initial a packet
	PACKET packet;
	memset(packet.buf,0,PKT_LEN);
	packet.buf[0]=dstHwnd;
	len++;
	memcpy_s(packet.buf+1,len,data,len);
	packet.len=len;
	packet.recFrom=0;
	packet.sendTo=0;

	//send to queue
	if(qBuff.empty())
		TRACE(_T("Empty\r\n"));
	WaitForSingleObject(queueSend_Mutex, INFINITE);
	qBuff.push(packet);
	ReleaseMutex(queueSend_Mutex);
	ReleaseSemaphore(sem_packet_send,1,NULL);
}
struct Param
{
	unsigned int dstHwnd;
	CString filePath;
	CControlledBroadDlg* pDlg;
} PCIESendFileThreadParams;
void PhyHelper::sendFile(CControlledBroadDlg* pDlg,unsigned int dstHwnd,const CString& path)
{
	PCIESendFileThreadParams.dstHwnd=dstHwnd;
	PCIESendFileThreadParams.filePath=path;
	PCIESendFileThreadParams.pDlg=pDlg;
	AfxBeginThread(SendFileThread,this,THREAD_PRIORITY_ABOVE_NORMAL);
}
UINT SendFileThread(LPVOID pParam)
{
	USES_CONVERSION;
	unsigned int dstHwnd=PCIESendFileThreadParams.dstHwnd;
	CString filePath=PCIESendFileThreadParams.filePath;
	CControlledBroadDlg* pDlg=PCIESendFileThreadParams.pDlg;
	PhyHelper* phy=(PhyHelper*)pParam;

	int nodeID=GetPrivateProfileInt(_T("NodeInfo"),_T("NodeID"),-1,filePath);
	CFile file;
	if(!file.Open(filePath,CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(_T("文件打开失败"));
		AfxEndThread(0);
		return 0;
	}
	Cmd_DeliverConfig* cmd=new Cmd_DeliverConfig;
	//发送文件名
	unsigned int fileSize=file.GetLength();
	CString fileName=file.GetFileName();
	CString fileLenStr;
	fileLenStr.Format(_T("*%u"),fileSize);
	fileName+=fileLenStr;
	SetCmd_DeliverConfig(cmd,nodeID,SrcAddr_Outside,DstAddr_LocalDlg,0,fileName.GetLength()+1,0,(unsigned char*)T2A(fileName.GetBuffer(fileName.GetLength())));
	fileName.ReleaseBuffer();
	phy->sendPacket(dstHwnd,(u_char*)cmd,sizeof(Cmd_DeliverConfig));
	pDlg->m_ProgressSending.SetRange32(0,fileSize);
	pDlg->m_ProgressSending.SetPos(0);

	
	//SoftwareCommend* tCmd=new SoftwareCommend;
	//SetSoftwareCommend(tCmd,0,Function::Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,Pro_FileSize,1,&fileSize);
	//serverCaper->sendPacket(0,HWND_BroadPlane,(u_char*)cmd,sizeof(SoftwareCommend));
	//xdelete(tCmd);

	//发送文件体
	int len,i;
	char buff[1024];
	for(i=1;;i++)
	{
		len=file.Read(buff,1024);
		if(len<=0)//发送文件结束
		{
			SetCmd_DeliverConfig(cmd,nodeID,SrcAddr_Outside,DstAddr_LocalDlg,i,0,1,NULL);
			phy->sendPacket(dstHwnd,(u_char*)cmd,sizeof(Cmd_DeliverConfig));
			TRACE(_T("Done\r\n"));
			break;
		}
		SetCmd_DeliverConfig(cmd,nodeID,SrcAddr_Outside,DstAddr_LocalDlg,i,len,0,(unsigned char*)buff);
		phy->sendPacket(dstHwnd,(u_char*)cmd,sizeof(Cmd_DeliverConfig));
		//fileSize=((i*1024)<fileSize?(i*1024):fileSize);
		pDlg->m_ProgressSending.SetPos(((i*1024)<file.GetLength())?(i*1024):file.GetLength());

		//SoftwareCommend* tCmd=new SoftwareCommend;
		//SetSoftwareCommend(tCmd,0,Function::Func_Cmd,SrcAddr::SrcAddr_Outside,DstAddr::DstAddr_LocalDlg,Pro_FileRecved,1,&fileSize);
		//serverCaper->sendPacket(0,HWND_BroadPlane,(u_char*)cmd,sizeof(SoftwareCommend));
		//xdelete(tCmd);
	}
	file.Close();
	xdelete(cmd);
	AfxEndThread(0);
	return 0;
}
void PhyHelper::setPath(const CString& path)
{
	m_strPath=path;
}
void PhyHelper::setHwnd(HWND hWnd,int i)
{
	this->hWnd[i]=hWnd;
}
void PhyHelper::PcieReset()
{
	if(m_hDev!=NULL)
	{
		dev_memWrite((int)m_hDev,0x8020,&one,sizeof(unsigned int));
		dev_memWrite((int)m_hDev,0x8020,&zero,sizeof(unsigned int));
	}
}
void PhyHelper::PcieOpen()
{
	if(m_hDev!=NULL)
	{
		return;
	}
	if((m_hDev = (HANDLE)dev_open(VID, DID))==NULL)
	{
		AfxMessageBox(_T("打开设备失败!"));
		return;
	}
	pcie_Mutex = CreateMutex(NULL, FALSE, NULL);
	if(m_dir==Dir_Up)
	{
		queue_Mutex = CreateMutex(NULL, FALSE, NULL);
		sem_packet_rec=CreateSemaphore(NULL,0,10000,NULL);
		thread_rec = CreateThread(NULL, 0, recThreadFunc, this, CREATE_SUSPENDED, NULL);
		::SetThreadPriority(thread_rec,THREAD_PRIORITY_ABOVE_NORMAL);
		::ResumeThread(thread_rec);
	}
	else if(m_dir==Dir_Down)
	{
		queueSend_Mutex = CreateMutex(NULL, FALSE, NULL);//用于保护队列的互斥量
		sem_packet_send=CreateSemaphore(NULL,0,10000,NULL);
		thread_send = CreateThread(NULL, 0, sendThreadFunc, this, 0, NULL);
		::SetThreadPriority(thread_send,THREAD_PRIORITY_NORMAL);
		::ResumeThread(thread_send);
	}
	//start phy
	dev_memWrite((int)m_hDev,0x8004,&zero,1);
	dev_memWrite((int)m_hDev,0x8000,&one,1);

	dev_memWrite((int)m_hDev,0x800C,&DAconfig,1);
	dev_memWrite((int)m_hDev,0x8008,&zero,1);								//再启动da
	//start phy
	Sleep(4000);
	dev_memWrite((int)m_hDev,0x8008,&one,1);
}
void PhyHelper::PcieStart()
{
#ifndef _DEBUG
	if(!m_hDev)
	{
		AfxMessageBox(_T("设备未打开"));
		return;
	}
#endif
	if(m_hDev)
	{
		//	dev_dataFilePath((int)m_hDev, 0, (LPWSTR)(LPCTSTR)m_strPath, m_strPath.GetLength());
		PcieEnd(); //上下行切换时复位缓存
		dev_dataLen((int)m_hDev, 0, m_uLen);
		dev_dataRefEvent((int)m_hDev, 0, m_hEvent);
		dev_dataSetDir((int)m_hDev,0,m_dir);
		dev_dataSetMaxCount((int)m_hDev,0,m_nCount);
		PXI_FPGA_WriteDWord(m_hDev,0,0x54,0x07000000);
		AfxBeginThread(MonitorThread, this,THREAD_PRIORITY_HIGHEST);
		m_bStarted = TRUE;
	}
}
void PhyHelper::PcieEnd()
{
	if(m_bStarted)
	{
		SetEvent(m_hEventQuit);
	}
	dev_channelDisable((int)m_hDev, 0);
	dev_channelReset((int)m_hDev, 0);
	m_bStarted = FALSE;
}
void PhyHelper::PcieClose()
{
	if(m_hDev)
	{
		if(m_bStarted)
		{
			SetEvent(m_hEventQuit);
		}
		dev_channelDisable((int)m_hDev, 0);
		dev_close((int)m_hDev);
		m_hDev = NULL;
	}
}
UINT PhyHelper::MonitorThread(LPVOID pParam)
{
	PhyHelper* pHelper=(PhyHelper*)pParam;
	UINT ret;
	EnterCriticalSection(&cs_DMA);//进入临界区
	ret=pHelper->MonitorThreadFunc();
	LeaveCriticalSection(&cs_DMA);//离开临界区
	AfxEndThread(0);
	return ret;
}
UINT PhyHelper::MonitorThreadFunc()
{
	HANDLE handles[] = {m_hEvent, m_hEventQuit};
	int count = 0;
	BOOL loop = TRUE;
	BYTE* buf = new BYTE[m_uLen*8];
	CFile DataFile;
	dev_channelEnable((int)m_hDev, 0);

	if(DataFile.Open(m_strPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::shareDenyWrite))
	{
		if(m_dir==Dir_Down)
		{
			UINT nRead = DataFile.Read(buf, m_uLen*8);
			if(nRead > m_uLen)
				dev_dataWrite((int)m_hDev, 0, (UINT*)buf, nRead/sizeof(UINT));
			else
				loop = FALSE;
		}
		//if(loop)
		//{
		//    dev_channelEnable((int)m_hDev, 0);
		//    //PXI_StartDMA(m_hDev);
		//}
		while(loop)
		{
			//ULONG ret = 0;
			switch(WaitForMultipleObjects(2, handles, FALSE, INFINITE))
			{
			case WAIT_OBJECT_0:
				count++;
				if(m_dir==Dir_Down) //下行
				{
					ResetEvent(m_hEvent);
					UINT dwRead = DataFile.Read(buf, m_uLen);
					loop = count < m_nCount && dwRead == m_uLen;
					if(loop)
					{
						if(!dev_dataWrite((int)m_hDev, 0, (UINT*)buf, m_uLen/sizeof(UINT)))
						{
							--count;
							DataFile.Seek(-dwRead, CFile::current);
						}
					}
				}
				else if(m_dir==Dir_Up)//上行
				{
					if(dev_dataRead((int)m_hDev, 0, (UINT*)buf, m_uLen/sizeof(UINT))>0)
						DataFile.Write(buf, m_uLen);
					else
						--count;
					loop = count < m_nCount;
				}
				break;
			default:
				loop = FALSE;
				break;
			}
		}
		DataFile.Close();
	}
	else
	{
		AfxMessageBox(_T("请正确设置数据源路径"));
	}
	//PcieEnd();
	delete[] buf;
	return 0;
}

void PhyHelper::writeCommand(unsigned int* buff,int length)
{
	for(int i=0;i<12;i++)
	{
		unsigned int data=0;
		dev_memWrite((int)m_hDev,CMD_Addr+4*i,&data,1);
	}
	for(int i=0;i<length/sizeof(unsigned int);i++)
	{
		dev_memWrite((int)m_hDev,CMD_Addr+4*i,buff+i,1);
	}
}

DWORD WINAPI recThreadFunc(LPVOID lpParameter)
{
	PhyHelper* pHelper=(PhyHelper*) lpParameter;
	PACKET packet2;
	packet2.len=0;
	unsigned char charBuf[92];
	unsigned int uintBuf[200];
	unsigned int startPoint=0;
	unsigned int remainLen;
	unsigned int recEmpty;
	unsigned int ctr;
	unsigned int mode;
	unsigned int len=0;
	unsigned int read_len=0;
	int b;
   while(1)
   {
		//WaitForSingleObject(pHelper->pcie_Mutex, INFINITE);
	   	dev_memRead((int)pHelper->m_hDev,0x8010,&recEmpty,1);
		//ReleaseMutex(pHelper->pcie_Mutex);
		if(!recEmpty)
		{
			dev_memRead((int)pHelper->m_hDev,0x8014,&ctr,1);
			len=ctr&0x00000FFF;
			mode=ctr&0x0000F000;
			mode=mode>>12;
			if(len==91)
				read_len=23;
			if(len==66)
				read_len=17;
			if(len==125)
				read_len=32;
			//WaitForSingleObject(pHelper->pcie_Mutex, INFINITE);
			for(b=0;b<read_len;b++)
			{
			dev_memRead((int)pHelper->m_hDev,0x8018,uintBuf+b,1);
			}
			//ReleaseMutex(pHelper->pcie_Mutex);
			
			if(len==91)//data
			{
				if((uintBuf[0]&0xFF000000)==0xAA000000)
				{
					memcpy(charBuf,uintBuf,92);
					memcpy(packet2.buf+startPoint,charBuf,3);
					startPoint=3;
					memcpy(packet2.buf+startPoint,charBuf+4,84);
					startPoint=87;
					memcpy(packet2.buf+startPoint,charBuf+89,3);
					startPoint=90;
					packet2.len=packet2.len+90;
				}
				if((uintBuf[0]&0xFF000000)==0x0F000000)
				{
					memcpy(charBuf,uintBuf,92);
					memcpy(packet2.buf+startPoint,charBuf,3);
					startPoint=startPoint+3;
					memcpy(packet2.buf+startPoint,charBuf+4,84);
					startPoint=startPoint+84;
					memcpy(packet2.buf+startPoint,charBuf+89,3);
					startPoint=startPoint+3;
					packet2.len=packet2.len+90;
				}
				if((uintBuf[0]&0xFF000000)==0xF0000000)
				{
					memcpy(charBuf,uintBuf,92);
					memcpy(packet2.buf+startPoint,charBuf,2);
					startPoint=startPoint+2;
					remainLen=charBuf[2];
					memcpy(packet2.buf+startPoint,charBuf+4,remainLen-2);
					packet2.len=packet2.len+remainLen;
					packet2.recFrom=DATA_CHANNEL;
					//WaitForSingleObject(pHelper->queue_Mutex, INFINITE);
					pHelper->qBuff.push(packet2);
					//ReleaseMutex(pHelper->queue_Mutex);
					//ReleaseSemaphore(pHelper->sem_packet_rec,1,NULL);
					startPoint=0;
					packet2.len=0;
					ReleaseSemaphore(pHelper->sem_packet_rec,1,NULL);
					
				}
			}
		}
		else
		{
			
			//if(packet2.buf[0]<10)
			//	::SendMessage(pHelper->hWnd[packet2.buf[0]],WM_USER_RECVFROMPCIE,NULL,NULL);
			//else
			//	::SendMessage(pHelper->hWnd[HWND_Main],WM_USER_RECVFROMPCIE,NULL,NULL);
		}
   }
    return 0L;
}

DWORD WINAPI sendThreadFunc(LPVOID lpParameter)
{	
	PhyHelper* pHelper=(PhyHelper*) lpParameter;
	PACKET packetSend;
	int remainLen;
	unsigned char charBuf[92];
	unsigned int uintBuf[23];
	int startPoint;
	unsigned char remainBuf[90];
	unsigned int data_full;
	int b;
	unsigned int isFull;
	while(1)
	{
		WaitForSingleObject(pHelper->sem_packet_send,INFINITE);
		WaitForSingleObject(pHelper->queueSend_Mutex, INFINITE);
		packetSend=pHelper->qBuff.front();
		pHelper->qBuff.pop();
		ReleaseMutex(pHelper->queueSend_Mutex);
		//divide packet and send
		remainLen=packetSend.len;

		while(remainLen>=90)
		{
			startPoint=packetSend.len-remainLen;
			memcpy(charBuf,packetSend.buf+startPoint,3);
			if(remainLen==packetSend.len)
				charBuf[3]=0xaa;
			else
				charBuf[3]=0x0f;
			memcpy(charBuf+4,packetSend.buf+startPoint+3,84);
			charBuf[88]=0x00;
			memcpy(charBuf+89,packetSend.buf+startPoint+87,3);
			remainLen=remainLen-90;
			memcpy(uintBuf,charBuf,92);
			dev_memRead((int)pHelper->m_hDev,0x8104,&isFull,1);

			while(isFull)
			{
				dev_memRead((int)pHelper->m_hDev,0x8104,&isFull,1);
			}

			WaitForSingleObject(pHelper->pcie_Mutex, INFINITE);
			for(b=0;b<=22;b++)
				dev_memWrite((int)pHelper->m_hDev,0x8104,uintBuf+b,1);
			ReleaseMutex(pHelper->pcie_Mutex);

		}
		startPoint=packetSend.len-remainLen;
		memcpy(remainBuf,packetSend.buf+startPoint,remainLen);
		memcpy(charBuf,remainBuf,2);
		charBuf[2]=remainLen;
		charBuf[3]=0xf0;
		memcpy(charBuf+4,remainBuf+2,84);
		charBuf[88]=0x00;
		memcpy(charBuf+89,remainBuf+86,3);
		memcpy(uintBuf,charBuf,92);

		dev_memRead((int)pHelper->m_hDev,0x8104,&isFull,1);
		while(isFull)
		{
			dev_memRead((int)pHelper->m_hDev,0x8104,&isFull,1);
		}

		WaitForSingleObject(pHelper->pcie_Mutex, INFINITE);
		for(b=0;b<=22;b++)
			dev_memWrite((int)pHelper->m_hDev,0x8104,uintBuf+b,1);
		ReleaseMutex(pHelper->pcie_Mutex);
	}

}

unsigned int PhyHelper::readParams(ParamName pName)
{
	unsigned int ctr=0xffffffff;
	unsigned int addr=0xffffffff;
	switch(pName)
	{
	case Param_isConnected:
		{
			addr=0x8000;
		}
		break;
	case Param_FrequenceDeviation:
		{
			addr=0x8204;
		}
		break;
	case Param_ChannelEstimate:
		{
			addr=0x8200;
		}
		break;
		
	default:
		break;
	}
	if(addr!=0xffffffff)
	{
		dev_memRead((int)m_hDev,addr,&ctr,1);
	}
	return ctr;
}