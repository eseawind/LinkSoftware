#pragma once
#include "pcap.h"
#include "Globe.h"

extern CRITICAL_SECTION cs_WinpCap;//互斥变量
#define ETHER_ADDR_LEN 6//MAC地址长度
#define MY_LINK_TYPE 0x00aa//本软件所用协议类型标志
extern const u_char ETHER_BROADCASTHOST[ETHER_ADDR_LEN];//广播地址
/************************************************************************/
/* WinPcap类可以获取指定网卡的缓存数据，从而捕获通过该网卡设备的数据。
** 首先在程序中声明一个唯一的全局变量实例，然后在主窗口打开网卡设备并开启接收线程。
** 在每个需要用到网口通信的窗口中注册其句柄到该实例中，关闭窗口时通过注册NULL来注销
** 在接收线程中一旦收到合法数据包，则会根据包头中的“作用窗口句柄”来向指定窗口发消息，从而将接收到的数据包传送到对应窗口中*/
/************************************************************************/
class WinpCapHelper//WinPcap类
{
public:
	WinpCapHelper();
	~WinpCapHelper(void);
public:
	struct ether_header//以太网头部
	{
		u_char ether_dhost[ETHER_ADDR_LEN];//目的MAC地址
		u_char ether_shost[ETHER_ADDR_LEN];//源MAC地址
		u_char type[2];//协议类型——0x00aa
		u_short dstWnd;//指令作用窗口句柄
		unsigned int payloadLen;//负载长度
	};
private:
	pcap_if_t* adapter;
	pcap_t* adapterHandle;
	CWinThread* p_ThreadCatchPacket;
	u_char mAddrHosts[ETHER_ADDR_LEN];//自己的MAC地址
	HWND m_hWnd[HWND_Number];//已注册的窗口句柄
	int devID;//网卡号
public:
	static void hex2Hosts(unsigned long long hex,u_char* hosts);//长整形转MAC地址数据
	static pcap_if_t* getAllAdapters();//获取本机所有网卡设备
	static PIP_ADAPTER_INFO getNetAdaptersInfo();//获取本机所有网卡信息
	static void getMacAddress(u_char* hosts,int devID);//获取某网卡MAC地址（不对应，不可用）

	void setMyHWnd(HWND hWnd,int i);//注册窗口句柄
	void setMyAddr(unsigned long long hex);//设置源MAC地址
	bool isCurrAdapterOpen();//网卡是否打开
	bool openAdapter(int devID);//打开网卡
	void closeCurrAdapter();//关闭网卡
	bool isMyType(const u_char* packet);//收到的数据包是否是本协议包
	bool isMyMAC(const u_char* packet);//收到的数据包是否是发给本机的包

	void beginCatching();//开始捕获数据包线程
	friend UINT catchPacket(LPVOID pParam);//获取数据包线程

	bool sendPacket(unsigned long long dstAddrHex,u_char dstWnd,u_char* data,int len);//发送数据包

	void sendFile(unsigned long long dstAddrHex,const CString& filePath);//开始发送配置文件
	friend UINT threadSendFile(LPVOID pParam);//发送文件线程
};

