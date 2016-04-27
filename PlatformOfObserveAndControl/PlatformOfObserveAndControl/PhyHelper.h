#pragma once
#include "Globe.h"

extern CRITICAL_SECTION cs_DMA;//互斥变量
extern unsigned int zero;//0常量
extern unsigned int one;//1常量

#define CMD_Addr 0x9000//指令地址
#define DATA_CHANNEL 4//0100
#define BC_CHANNEL 1//0001
#define CC_CHANNEL 2//0010
#define PKT_LEN 1500
enum Direction//DMA方向
{
	Dir_Up,//上行
	Dir_Down,//下行
};
enum ParamName//PCIE读数据的变量名
{
	Param_isConnected,
	Param_FrequenceDeviation,
	Param_ChannelEstimate,
};
struct PACKET//PCIE数据包结构体
{
	unsigned char buf[PKT_LEN];
	unsigned int len;
	unsigned int recFrom;
	unsigned int sendTo;
};
class CControlledBroadDlg;
class PhyHelper
{
public:
	PhyHelper(Direction dir);
	~PhyHelper(void);
public:
	HANDLE m_hDev;//设备句柄
	BOOL m_bStarted;//设备是否打开
	UINT m_uLen;//采集/回放长度
	UINT m_nCount;//采集/回放次数
	HANDLE m_hEvent;//DMA开始事件
	HANDLE m_hEventQuit;//DMA退出事件
	std::queue<PACKET> qBuff;//PCIE接收数据缓存队列
	int m_dir;//DMA方向（上行/下行）
	int VID,DID;
	CString m_strPath;//文件地址
	HANDLE sem_packet_rec;
	HANDLE sem_packet_send;//用于告知发送线程，我的包已经发送给发送线程了
	HANDLE queue_Mutex;//队列保护信号量
	HANDLE queueSend_Mutex;//队列保护信号量
	HANDLE pcie_Mutex;
	HWND hWnd[HWND_Number];//已注册窗口句柄
	HANDLE thread_rec;//接收线程句柄
	HANDLE thread_send;//发送线程句柄
public:
	/************************************************************************/
	/*	功能：发送数据包
		输出：无
		输入：	dstHwnd：指令作用窗口句柄
				data：数据负载
				len：数据负载长度*/
	/************************************************************************/
	void sendPacket(unsigned int dstHwnd, const unsigned char* data,int len);
	/************************************************************************/
	/*	功能：发送文件
		输出：无
		输入：	pDlg：窗口指针
				dstHwnd：指令作用窗口句柄
				path：文件路径*/
	/************************************************************************/
	void sendFile(CControlledBroadDlg* pDlg,unsigned int dstHwnd,const CString& path);
	friend UINT SendFileThread(LPVOID pParam);//发送文件线程
	void setPath(const CString& path);//设置路径
	void setHwnd(HWND hWnd,int i);//注册窗口句柄
	void PcieReset();//复位
	void PcieOpen();//打开设备
	void PcieStart();//开始运行设备
	void PcieEnd();//终止运行设备
	void PcieClose();//关闭设备
	static UINT MonitorThread(LPVOID pParam);//打开DMA监视线程
	UINT MonitorThreadFunc();//DMA监视线程
	friend DWORD WINAPI recThreadFunc(LPVOID lpParameter);//PCIE接收线程
	friend DWORD WINAPI sendThreadFunc(LPVOID lpParameter);//PCIE发送线程
	void writeCommand(unsigned int* buff,int length);//PCIE写指令
	unsigned int readParams(ParamName pName);//PCIE读数据
};

