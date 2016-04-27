#pragma once
#include "pcap.h"
class CPlatformOfObserveAndControlDlg;

extern CRITICAL_SECTION cs_WinpCap;
#define ETHER_ADDR_LEN 6
#define MY_LINK_TYPE 0x00aa
extern const u_char ETHER_BROADCASTHOST[ETHER_ADDR_LEN];

class WinpCaper
{
public:
	WinpCaper(CPlatformOfObserveAndControlDlg* pDlg);
	~WinpCaper(void);
public:
	struct ether_header
	{
		u_char ether_dhost[ETHER_ADDR_LEN];
		u_char ether_shost[ETHER_ADDR_LEN];
		u_char type[2];
		u_char payloadLen;
	};
private:
	CPlatformOfObserveAndControlDlg* pDlg;
	pcap_if_t* adapter;
	pcap_if_t* allAdapters;
	pcap_t* adapterHandle;
	int numAdapters;
	int devID;
	CWinThread* p_ThreadCatchPacket;
	u_char mMACHosts[ETHER_ADDR_LEN];
	u_char mDstMACHosts[ETHER_ADDR_LEN];
public:
	void setDstMAC(const u_char dstMACHosts[ETHER_ADDR_LEN]);
	void setDevIndex(int n);
	void initial();
	bool getAllAdapters();
	bool openAdapter();
	void beginCatching();
	void closeAdapter();
	friend UINT catchPacket(LPVOID pParam);
	bool sendPacket(const u_char dstMACHosts[ETHER_ADDR_LEN],u_char* data,int len);
	static PIP_ADAPTER_INFO GetNetAdaptersInfo();
	static u_char* GetMacAddress(int i);
	bool isMyType(const u_char* packet);
	bool isMyMAC(const u_char* packet);
	bool isBroadcasted(const u_char* packet);
	bool isFromServer(const u_char* packet);
};

