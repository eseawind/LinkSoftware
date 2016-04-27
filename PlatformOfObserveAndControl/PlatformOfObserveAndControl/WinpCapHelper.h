#pragma once
#include "pcap.h"
#include "Globe.h"

extern CRITICAL_SECTION cs_WinpCap;//�������
#define ETHER_ADDR_LEN 6//MAC��ַ����
#define MY_LINK_TYPE 0x00aa//���������Э�����ͱ�־
extern const u_char ETHER_BROADCASTHOST[ETHER_ADDR_LEN];//�㲥��ַ
/************************************************************************/
/* WinPcap����Ի�ȡָ�������Ļ������ݣ��Ӷ�����ͨ���������豸�����ݡ�
** �����ڳ���������һ��Ψһ��ȫ�ֱ���ʵ����Ȼ���������ڴ������豸�����������̡߳�
** ��ÿ����Ҫ�õ�����ͨ�ŵĴ�����ע����������ʵ���У��رմ���ʱͨ��ע��NULL��ע��
** �ڽ����߳���һ���յ��Ϸ����ݰ��������ݰ�ͷ�еġ����ô��ھ��������ָ�����ڷ���Ϣ���Ӷ������յ������ݰ����͵���Ӧ������*/
/************************************************************************/
class WinpCapHelper//WinPcap��
{
public:
	WinpCapHelper();
	~WinpCapHelper(void);
public:
	struct ether_header//��̫��ͷ��
	{
		u_char ether_dhost[ETHER_ADDR_LEN];//Ŀ��MAC��ַ
		u_char ether_shost[ETHER_ADDR_LEN];//ԴMAC��ַ
		u_char type[2];//Э�����͡���0x00aa
		u_short dstWnd;//ָ�����ô��ھ��
		unsigned int payloadLen;//���س���
	};
private:
	pcap_if_t* adapter;
	pcap_t* adapterHandle;
	CWinThread* p_ThreadCatchPacket;
	u_char mAddrHosts[ETHER_ADDR_LEN];//�Լ���MAC��ַ
	HWND m_hWnd[HWND_Number];//��ע��Ĵ��ھ��
	int devID;//������
public:
	static void hex2Hosts(unsigned long long hex,u_char* hosts);//������תMAC��ַ����
	static pcap_if_t* getAllAdapters();//��ȡ�������������豸
	static PIP_ADAPTER_INFO getNetAdaptersInfo();//��ȡ��������������Ϣ
	static void getMacAddress(u_char* hosts,int devID);//��ȡĳ����MAC��ַ������Ӧ�������ã�

	void setMyHWnd(HWND hWnd,int i);//ע�ᴰ�ھ��
	void setMyAddr(unsigned long long hex);//����ԴMAC��ַ
	bool isCurrAdapterOpen();//�����Ƿ��
	bool openAdapter(int devID);//������
	void closeCurrAdapter();//�ر�����
	bool isMyType(const u_char* packet);//�յ������ݰ��Ƿ��Ǳ�Э���
	bool isMyMAC(const u_char* packet);//�յ������ݰ��Ƿ��Ƿ��������İ�

	void beginCatching();//��ʼ�������ݰ��߳�
	friend UINT catchPacket(LPVOID pParam);//��ȡ���ݰ��߳�

	bool sendPacket(unsigned long long dstAddrHex,u_char dstWnd,u_char* data,int len);//�������ݰ�

	void sendFile(unsigned long long dstAddrHex,const CString& filePath);//��ʼ���������ļ�
	friend UINT threadSendFile(LPVOID pParam);//�����ļ��߳�
};

