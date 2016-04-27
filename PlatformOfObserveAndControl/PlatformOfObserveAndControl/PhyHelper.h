#pragma once
#include "Globe.h"

extern CRITICAL_SECTION cs_DMA;//�������
extern unsigned int zero;//0����
extern unsigned int one;//1����

#define CMD_Addr 0x9000//ָ���ַ
#define DATA_CHANNEL 4//0100
#define BC_CHANNEL 1//0001
#define CC_CHANNEL 2//0010
#define PKT_LEN 1500
enum Direction//DMA����
{
	Dir_Up,//����
	Dir_Down,//����
};
enum ParamName//PCIE�����ݵı�����
{
	Param_isConnected,
	Param_FrequenceDeviation,
	Param_ChannelEstimate,
};
struct PACKET//PCIE���ݰ��ṹ��
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
	HANDLE m_hDev;//�豸���
	BOOL m_bStarted;//�豸�Ƿ��
	UINT m_uLen;//�ɼ�/�طų���
	UINT m_nCount;//�ɼ�/�طŴ���
	HANDLE m_hEvent;//DMA��ʼ�¼�
	HANDLE m_hEventQuit;//DMA�˳��¼�
	std::queue<PACKET> qBuff;//PCIE�������ݻ������
	int m_dir;//DMA��������/���У�
	int VID,DID;
	CString m_strPath;//�ļ���ַ
	HANDLE sem_packet_rec;
	HANDLE sem_packet_send;//���ڸ�֪�����̣߳��ҵİ��Ѿ����͸������߳���
	HANDLE queue_Mutex;//���б����ź���
	HANDLE queueSend_Mutex;//���б����ź���
	HANDLE pcie_Mutex;
	HWND hWnd[HWND_Number];//��ע�ᴰ�ھ��
	HANDLE thread_rec;//�����߳̾��
	HANDLE thread_send;//�����߳̾��
public:
	/************************************************************************/
	/*	���ܣ��������ݰ�
		�������
		���룺	dstHwnd��ָ�����ô��ھ��
				data�����ݸ���
				len�����ݸ��س���*/
	/************************************************************************/
	void sendPacket(unsigned int dstHwnd, const unsigned char* data,int len);
	/************************************************************************/
	/*	���ܣ������ļ�
		�������
		���룺	pDlg������ָ��
				dstHwnd��ָ�����ô��ھ��
				path���ļ�·��*/
	/************************************************************************/
	void sendFile(CControlledBroadDlg* pDlg,unsigned int dstHwnd,const CString& path);
	friend UINT SendFileThread(LPVOID pParam);//�����ļ��߳�
	void setPath(const CString& path);//����·��
	void setHwnd(HWND hWnd,int i);//ע�ᴰ�ھ��
	void PcieReset();//��λ
	void PcieOpen();//���豸
	void PcieStart();//��ʼ�����豸
	void PcieEnd();//��ֹ�����豸
	void PcieClose();//�ر��豸
	static UINT MonitorThread(LPVOID pParam);//��DMA�����߳�
	UINT MonitorThreadFunc();//DMA�����߳�
	friend DWORD WINAPI recThreadFunc(LPVOID lpParameter);//PCIE�����߳�
	friend DWORD WINAPI sendThreadFunc(LPVOID lpParameter);//PCIE�����߳�
	void writeCommand(unsigned int* buff,int length);//PCIEдָ��
	unsigned int readParams(ParamName pName);//PCIE������
};

