#include "StdAfx.h"
#include "WinpCapHelper.h"

#include "iphlpapi.h"
#include "Globe.h"
#include "SoftwareCommend.h"
#include "PlatformOfObserveAndControl.h"

CRITICAL_SECTION cs_WinpCap;
const u_char ETHER_BROADCASTHOST[ETHER_ADDR_LEN]={0xff,0xff,0xff,0xff,0xff,0xff};
const u_char FLAG_SERVER[ETHER_ADDR_LEN]={'s','e','r','v','e','r'};
const u_char FLAG_CLIENT[ETHER_ADDR_LEN]={'c','l','i','e','n','t'};


WinpCapHelper::WinpCapHelper()
	: adapterHandle(NULL)
	, adapter(NULL)
	, devID(-1)
{
	InitializeCriticalSection(&cs_WinpCap);//初始化临界区
	
	p_ThreadCatchPacket=AfxBeginThread(catchPacket,this,0,0,CREATE_SUSPENDED,NULL);
}


WinpCapHelper::~WinpCapHelper(void)
{
}

pcap_if_t* WinpCapHelper::getAllAdapters()
{
	pcap_if_t* allAdapters;
	char errorBuffer[ PCAP_ERRBUF_SIZE ];//错误信息缓冲区
	if( pcap_findalldevs_ex( PCAP_SRC_IF_STRING, NULL, 
		&allAdapters, errorBuffer ) == -1 )
	{//检索机器连接的所有网络适配器
		AfxMessageBox(_T("获取网卡信息失败"));
		return NULL;
	}
	return allAdapters;
}

void WinpCapHelper::hex2Hosts(unsigned long long hex,u_char* hosts)
{
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		u_char AddrBit=hex%256;
		hosts[ETHER_ADDR_LEN-1-i]=AddrBit;
		hex=hex/256;
	}
}

PIP_ADAPTER_INFO WinpCapHelper::getNetAdaptersInfo()
{
	PIP_ADAPTER_INFO pAdapterInfo;  
	DWORD AdapterInfoSize;
	DWORD Err;  

	AdapterInfoSize = 0;  
	Err = GetAdaptersInfo(NULL, &AdapterInfoSize);  

	if((Err != 0) && (Err != ERROR_BUFFER_OVERFLOW))
	{  
		return   NULL;  
	}  

	//   分配网卡信息内存  
	pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, AdapterInfoSize);  
	if(pAdapterInfo == NULL)
	{  
		return   NULL;  
	}  

	if(GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize) != 0)
	{  
		GlobalFree(pAdapterInfo);  
		return   NULL;  
	}

	return pAdapterInfo;
}

void WinpCapHelper::getMacAddress(u_char* hosts,int devID)
{
	PIP_ADAPTER_INFO pAdapterInfo=getNetAdaptersInfo();
	for(int i=0;i<devID;i++)
	{
		pAdapterInfo=pAdapterInfo->Next;
	}
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		hosts[i]=pAdapterInfo->Address[i];
	}
}
void WinpCapHelper::setMyHWnd(HWND hWnd,int i)
{
	m_hWnd[i]=hWnd;
}
void WinpCapHelper::setMyAddr(unsigned long long hex)
{
	hex2Hosts(hex,mAddrHosts);
}

bool WinpCapHelper::isCurrAdapterOpen()
{
	if(adapterHandle)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
bool WinpCapHelper::openAdapter(int devID)
{
	pcap_if_t* allAdapters;
	char errorBuffer[ PCAP_ERRBUF_SIZE ];//错误信息缓冲区

	if(isCurrAdapterOpen())
	{
		closeCurrAdapter();
	}
		
	allAdapters=getAllAdapters();
	if(allAdapters==NULL)
	{
		return FALSE;
	}

	this->devID=devID;
	adapter=allAdapters;
	for(int i=0;i<devID;i++)
	{
		adapter=adapter->next;
	}
	// 打开指定适配器
	adapterHandle = pcap_open( adapter->name,						// name of the adapter
										65536,						// portion of the packet to capture
																	// 65536 guarantees that the whole 
																	// packet will be captured
										PCAP_OPENFLAG_PROMISCUOUS,	// promiscuous mode
										1000,						// read timeout - 1 millisecond
										NULL,						// authentication on the remote machine
										errorBuffer					// error buffer
										);
	if( adapterHandle == NULL )
	{//指定适配器打开失败
		pcap_freealldevs( allAdapters );// 释放适配器列表
		return false;
	}
	pcap_freealldevs( allAdapters );//释放适配器列表
	return true;
}

void WinpCapHelper::closeCurrAdapter()
{
	if(isCurrAdapterOpen())
	{
		pcap_close(adapterHandle);
	}
}

bool WinpCapHelper::isMyType(const u_char* packet)
{
	u_char type[2];
	type[0]=packet[12];
	type[1]=packet[13];
	if(type[0]==0xaa&&type[1]==0x00)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WinpCapHelper::isMyMAC(const u_char* packet)
{
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		if(packet[i]!=mAddrHosts[i]&&packet[i]!=ETHER_BROADCASTHOST[i])
		{
			return false;
		}
	}
	return true;
}

void WinpCapHelper::beginCatching()
{
	p_ThreadCatchPacket->ResumeThread();
}

UINT catchPacket(LPVOID pParam)
{
	// 开始捕获数据包
	WinpCapHelper* caper=(WinpCapHelper*)pParam;
	int retValue;
	pcap_pkthdr* packetHeader;
	const u_char* packetData;
	while( caper->m_hWnd[HWND_Main]!=NULL)
	{
		retValue = pcap_next_ex(caper->adapterHandle, 
									&packetHeader, 
									&packetData );
		if(retValue<0)
		{
			break;
		}
		// timeout elapsed if we reach this point
		if( retValue == 0 )
		{
			continue;
		}
		if(caper->isMyType(packetData)&&caper->isMyMAC(packetData))
		{
			WinpCapHelper::ether_header* etherHeader=(WinpCapHelper::ether_header*)packetData;
			unsigned char* data=new unsigned char[etherHeader->payloadLen];
			memcpy_s(data,etherHeader->payloadLen,etherHeader+1,etherHeader->payloadLen);
			::SendMessage(caper->m_hWnd[etherHeader->dstWnd],WM_USER_RECVFROMWINPCAP,(WPARAM)etherHeader->payloadLen,(LPARAM)data);
		}
	}
	// if we get here, there was an error reading the packets
	AfxEndThread(0);
	return 0;
}

bool WinpCapHelper::sendPacket(unsigned long long dstAddrHex,u_char dstWnd,u_char* data,int len)
{
	u_char dstAddrHosts[ETHER_ADDR_LEN];
	hex2Hosts(dstAddrHex,dstAddrHosts);
	u_char* etherPacket=(u_char*)malloc(sizeof(ether_header)+len*sizeof(u_char));
	ether_header* etherHeader=(ether_header*)etherPacket;
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		etherHeader->ether_dhost[i]=dstAddrHosts[i];
		etherHeader->ether_shost[i]=mAddrHosts[i];
	}
	etherHeader->type[0]=0xaa;
	etherHeader->type[1]=0x00;
	etherHeader->dstWnd=dstWnd;
	etherHeader->payloadLen=len;
	memcpy_s(etherHeader+1,len,data,len);
	if( pcap_sendpacket( adapterHandle, // the adapter handle
		etherPacket, // the packet
		sizeof(ether_header)+len*sizeof(u_char) // the length of the packet
		) != 0 )
	{
		xdelete(etherPacket);
		return false;
	}
	xdelete(etherPacket);
	return true;
}

struct Param
{
	unsigned long long dstAddrHex;
	CString filePath;
} WinpCapSendFileThreadParams;
void WinpCapHelper::sendFile(unsigned long long dstAddrHex,const CString& filePath)
{
	WinpCapSendFileThreadParams.dstAddrHex=dstAddrHex;
	WinpCapSendFileThreadParams.filePath=filePath;
	AfxBeginThread(threadSendFile,this);
}
UINT threadSendFile(LPVOID pParam)
{
	USES_CONVERSION;
	unsigned long long dstAddrHex=WinpCapSendFileThreadParams.dstAddrHex;
	CString filePath=WinpCapSendFileThreadParams.filePath;
	WinpCapHelper* caper=(WinpCapHelper*)pParam;

	int nodeID=GetPrivateProfileInt(_T("NodeInfo"),_T("NodeID"),-1,filePath);
	CFile file;
	if(!file.Open(filePath,CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(_T("文件打开失败"));
		AfxEndThread(0);
		return 0;
	}
	//
	//Cmd_DeliverConfig* cmd=new Cmd_DeliverConfig;
	////发送文件名
	//CString fileName=file.GetFileName();
	//SetCmd_DeliverConfig(cmd,nodeID,SrcAddr_Outside,DstAddr_LocalDlg,0,fileName.GetLength()+1,0,(u_char*)T2A(fileName.GetBuffer(fileName.GetLength())));
	//fileName.ReleaseBuffer();
	//caper->sendPacket(dstAddrHex,(u_char*)cmd,sizeof(Cmd_DeliverConfig));
	////发送文件体
	//int len,i;
	//char buff[1024];
	//for(i=1;;i++)
	//{
	//	len=file.Read(buff,1024);
	//	if(len<1024)//发送文件结束
	//	{
	//		SetCmd_DeliverConfig(cmd,nodeID,SrcAddr_Outside,DstAddr_LocalDlg,i,len,1,(u_char*)buff);
	//		caper->sendPacket(dstAddrHex,(u_char*)cmd,sizeof(Cmd_DeliverConfig));
	//		break;
	//	}
	//	SetCmd_DeliverConfig(cmd,nodeID,SrcAddr_Outside,DstAddr_LocalDlg,i,len,0,(u_char*)buff);
	//	caper->sendPacket(dstAddrHex,(u_char*)cmd,sizeof(Cmd_DeliverConfig));
	//}
	file.Close();
	//xdelete(cmd);
	AfxEndThread(0);
	return 0;
}

