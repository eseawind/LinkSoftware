#include "StdAfx.h"
#include "WinpCaper.h"

#include "iphlpapi.h"
#include "Globe.h"
#include "PlatformOfObserveAndControl.h"
#include "PlatformOfObserveAndControlDlg.h"

CRITICAL_SECTION cs_WinpCap;
const u_char ETHER_BROADCASTHOST[ETHER_ADDR_LEN]={0xff,0xff,0xff,0xff,0xff,0xff};
const u_char FLAG_SERVER[ETHER_ADDR_LEN]={'s','e','r','v','e','r'};
const u_char FLAG_CLIENT[ETHER_ADDR_LEN]={'c','l','i','e','n','t'};

WinpCaper::WinpCaper(CPlatformOfObserveAndControlDlg* pDlg)
	: pDlg(pDlg)
	, numAdapters(0)
	, adapter(NULL)
	, allAdapters(NULL)
	, adapterHandle(NULL)
	, devID(0)
{
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		mDstMACHosts[i]=ETHER_BROADCASTHOST[i];
	}
	InitializeCriticalSection(&cs_WinpCap);//初始化临界区

	p_ThreadCatchPacket=AfxBeginThread(catchPacket,this,0,0,CREATE_SUSPENDED,NULL);
}


WinpCaper::~WinpCaper(void)
{
	DeleteCriticalSection(&cs_WinpCap);//删除临界区
}

bool WinpCaper::getAllAdapters()
{
	char errorBuffer[ PCAP_ERRBUF_SIZE ];//错误信息缓冲区
	if( pcap_findalldevs_ex( PCAP_SRC_IF_STRING, NULL, 
		&allAdapters, errorBuffer ) == -1 )
	{//检索机器连接的所有网络适配器
		return false;
	}
	if( allAdapters == NULL )
	{//不存在人任何适配器
		numAdapters=0;
		return false;
	}
	for(pcap_if_t* adapter = allAdapters; adapter != NULL; adapter = adapter->next)
	{//遍历输入适配器信息(名称和描述信息)
		CString str1(adapter->name);
		CString str2(adapter->description);
		numAdapters++;
	}
	return true;
}
void WinpCaper::setDstMAC(const u_char dstMACHosts[ETHER_ADDR_LEN])
{
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		mDstMACHosts[i]=dstMACHosts[i];
	}
}
void WinpCaper::setDevIndex(int n)
{
	devID=n;
}
void WinpCaper::initial()
{
	if(adapterHandle!=NULL)
	{
		closeAdapter();
	}
	if(!openAdapter())
	{
		AfxMessageBox(_T("网络适配器打开失败"));
	}
	u_char* MAC=GetMacAddress(devID);
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		mMACHosts[i]=MAC[i];
	}
}
bool WinpCaper::openAdapter()
{
	char errorBuffer[ PCAP_ERRBUF_SIZE ];//错误信息缓冲区
	if(!getAllAdapters())
	{
		return false;
	}
	//选择要捕获数据包的适配器
	//if( adapterID < 1 || adapterID > numAdapters )
	//{
	//	pcap_freealldevs( allAdapters );
	//	return false;
	//}
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
void WinpCaper::closeAdapter()
{
	pcap_close(adapterHandle);
}
void WinpCaper::beginCatching()
{
	if(adapterHandle==NULL)
		return;
	p_ThreadCatchPacket->ResumeThread();
}
UINT catchPacket(LPVOID pParam)
{
	// 开始捕获数据包
	WinpCaper* caper=(WinpCaper*)pParam;
	int retValue;
	pcap_pkthdr* packetHeader;
	const u_char* packetData;
	while( ( retValue = pcap_next_ex(caper->adapterHandle, 
									&packetHeader, 
									&packetData ) ) >= 0 )
	{
		if(!caper)
		{
			AfxEndThread(0,true);
		}
		// timeout elapsed if we reach this point
		if( retValue == 0 )
		{
			continue;
		}
		if(caper->isMyType(packetData)&&(caper->isMyMAC(packetData)||caper->isBroadcasted(packetData)))
		{
			WinpCaper::ether_header* etherHeader=(WinpCaper::ether_header*)packetData;
			caper->pDlg->recvFromWpCaper((u_char*)(etherHeader+1),packetHeader->len);
		}
	}
	// if we get here, there was an error reading the packets
	return 0;
}

bool WinpCaper::sendPacket(const u_char dstMACHosts[ETHER_ADDR_LEN],u_char* data,int len)
{
	u_char* etherPacket=(u_char*)malloc(sizeof(ether_header)+len*sizeof(u_char));
	ether_header* etherHeader=(ether_header*)etherPacket;
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		etherHeader->ether_dhost[i]=dstMACHosts[i];
		etherHeader->ether_shost[i]=mMACHosts[i];
	}
	etherHeader->type[0]=0xaa;
	etherHeader->type[1]=0x00;
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
PIP_ADAPTER_INFO WinpCaper::GetNetAdaptersInfo()
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
u_char* WinpCaper::GetMacAddress(int n)
{
	u_char MACHosts[ETHER_ADDR_LEN];
	PIP_ADAPTER_INFO pAdapterInfo=GetNetAdaptersInfo();
	for(int i=0;i<n;i++)
	{
		pAdapterInfo=pAdapterInfo->Next;
	}
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		MACHosts[i]=pAdapterInfo->Address[i];
	}
	return MACHosts;  
}
bool WinpCaper::isMyType(const u_char* packet)
{
	u_char type[2];
	type[0]=packet[12];
	type[1]=packet[13];
	if(type[0]==0xaa&&type[1]==0x00)
	{
		return true;
	}
	return false;
}
bool WinpCaper::isMyMAC(const u_char* packet)
{
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		if(packet[i]!=mMACHosts[i])
		{
			return false;
		}
	}
	return true;
}
bool WinpCaper::isBroadcasted(const u_char* packet)
{
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		if(packet[i]!=ETHER_BROADCASTHOST[i])
		{
			return false;
		}
	}
	return true;
}

bool WinpCaper::isFromServer(const u_char* packet)
{
	for(int i=0;i<ETHER_ADDR_LEN;i++)
	{
		if(packet[12+i]!=FLAG_CLIENT[i])
		{
			return false;
		}
	}
	return true;
}