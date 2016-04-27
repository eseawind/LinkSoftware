// api.h

#ifndef _API_HEADER_
#define _API_HEADER_
#ifdef __cplusplus
extern "C"
{
#endif
int __stdcall dev_enumDids(int vid, int* dids, int len);
int __stdcall dev_enumSlots(int vid, int* slot, int len);
int __stdcall dev_open(int vid, int did);
void __stdcall dev_close(int handle);

int __stdcall dev_memRead(int handle, unsigned int addr, unsigned int* buf, int len);
int __stdcall dev_memWrite(int handle, unsigned int addr, unsigned int* buf, int len);

int __stdcall dev_memBlockRead(int handle, unsigned int addr, unsigned int* buf, int len);
int __stdcall dev_memBlockWrite(int handle, unsigned int addr, unsigned int* buf, int len);

int __stdcall dev_dataRead(int handle, int channel, unsigned int* buf, int len);
int __stdcall dev_dataReadSync(int handle, int channel, unsigned int* buf, int len);
int __stdcall dev_dataWrite(int handle, int channel, unsigned int* buf, int len);
int __stdcall dev_dataWriteSync(int handle, int channel, unsigned int* buf, int len);
int __stdcall dev_dataCancel(int handle, int channel);

int __stdcall dev_dataRefEvent(int handle, int channel, HANDLE tevent);
int __stdcall dev_dataDerefEvent(int handle, int channel);

int __stdcall dev_dataNum(int handle);
int __stdcall dev_dataDir(int handle, int channel);
int __stdcall dev_dataSetDir(int handle, int channel, int dir);
int __stdcall dev_dataLen(int handle, int channel, int len);
int __stdcall dev_dataFilePath(int handle, int channel, wchar_t* buf, int len);
int __stdcall dev_dataFilePos(int handle, int channel, __int64 pos);
int __stdcall dev_dataStat(int handle, int channel, unsigned int* buf, int len);
int __stdcall dev_dataSetMaxCount(int handle, int channel, int count);

int __stdcall dev_channelReset(int handle, int channel);
int __stdcall dev_channelEnable(int handle, int channel);
int __stdcall dev_channelDisable(int handle, int channel);

int __stdcall dev_version(int handle, wchar_t* buf, int len);

HANDLE	WINAPI PXI_FPGA_OpenCard(WORD VID,WORD DID,int index);

/*
获取系统中同种板卡的数目
*/
int		WINAPI PXI_FPGA_GetCardCount(WORD DID,WORD SubDID);

/*
	关闭句柄指向的设备
*/
void	WINAPI PXI_FPGA_CloseCard(HANDLE devHandle);


//	读取32bit DWORD
DWORD   WINAPI PXI_FPGA_ReadDWord(HANDLE hdl , int bar, DWORD offset, DWORD * data);

//	写入32bit DWORD
int		WINAPI PXI_FPGA_WriteDWord(HANDLE hdl , int bar, DWORD offset, DWORD value);

//DMA Stop
DWORD	WINAPI PXI_FPGA_DmaStop(HANDLE hdl , int bar, DWORD offset, DWORD * data, DWORD length);

// 将数据由FPGA 读取到 cpu
DWORD	WINAPI PXI_SetDmaWrite(HANDLE hdl, ULONG *pulAddr, ULONG size, ULONG count, ULONG pattern, ULONG *status);

// start dma
DWORD   WINAPI PXI_StartDMA(HANDLE hdl);

//映射事件句柄至驱动
HANDLE	WINAPI PXI_FPGA_MapEventHaddle(HANDLE hdl, HANDLE eventHandle, DWORD dwType);

//设置DMA REG CMD 指令
DWORD	WINAPI PXI_FPGA_SetDmaCmd(HANDLE hdl, ULONG ulCmd, ULONG ulThresholdNum);
#ifdef __cplusplus
}
#endif
#endif // _API_HEADER_
