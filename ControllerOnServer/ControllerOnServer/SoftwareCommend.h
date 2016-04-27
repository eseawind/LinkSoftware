#pragma once

/************************************************************************/
/* 主类型定义                                                                     */
/************************************************************************/
const unsigned int FRONT_FLAG=0xAA;//帧头标志

enum DstNodeID//目的节点ID
{
	DstNode_Server			=0x00,
	DstNode_Plane_Narrow	=0x11,
	DstNode_Plane_Broad		=0x12,
	DstNode_Ground_Narrow	=0x21,
	DstNode_Ground_Broad	=0x22,
};

#define DELTA_FUNC 2
enum Function//功能
{
	Func_DeliverConfig,//传送配置文件
	Func_Start,//启动

	Func_SndCollectOrder,//采集
	Func_SndReloadOrder,//回放

	Func_IntvNodeParams,//干预指令
	Func_RecontructMode,//重构指令

	Func_ShowStatisticsOrder,//显示指令
	Func_ShowStatisticsData,//显示报文

	Func_ProtocolBusiness,//业务
	Func_Close,//关闭
	Func_Cmd,//远程控制指令
};

#define DELTA_ADDR 3
enum SrcAddr//源节点
{
	SrcAddr_Outside			=0x01,//外部
	SrcAddr_LocalDlg		=0x02,//本地主控
	SrcAddr_BasebandBoard	=0x03,//基带
	SrcAddr_ProtocolBoard	=0x04,//协议
};

enum DstAddr//目的节点
{
	DstAddr_Outside			=0x01,//外部
	DstAddr_LocalDlg		=0x02,//本地主控
	DstAddr_BasebandBoard	=0x03,//基带
	DstAddr_ProtocolBoard	=0x04,//协议
};

#define DELTA_SPOT 1
enum SpotType//作用层
{
	MSK						=0x01,
	Frame					=0x02,
	SymbolInterleaver		=0x03,
	Spread					=0x04,
	ChipInterleaver			=0x05,
	Encode					=0x06,
};
struct SoftwareCommendHeadr//标准包头
{
	unsigned int frontFlag;//帧头标志
	unsigned int dstNodeID;//目的节点
	unsigned int function;//功能
	unsigned int srcAddr;//源节点
	unsigned int dstAddr;//目的节点
};
void SetCommendHeader(SoftwareCommendHeadr* cmd,
					unsigned int dstNodeID,unsigned int function,unsigned int srcAddr,unsigned int dstAddr);
enum BtnType//控制指令——按钮名称
{
	//Global
	BTN_Close,
	//Main
	BTN_Recontruct_Baseband_OK,
	BTN_Recontruct_Protocol_OK,
	BTN_Intervene_Baseband_OK,
	BTN_Intervene_Protocol_OK,
	BTN_ShowStatistics,
	BTN_Business_Start,
	BTN_ObsNCtrl_Apply,
	BTN_Statistics,
	//Master
	BTN_StartGuide,
	BTN_EndGuide,
	BTN_ShowUsers,
	BTN_ClearList,
	BTN_Command1,
	BTN_Command2,
	BTN_Command3,
	BTN_Command4,
	BTN_Command5,
	BTN_Command6,
	//Controlled
	BTN_AddIn,
	BTN_ReCommandYes,
	BTN_ReCommandNo,
	BTN_ReCommand1,
	BTN_ReCommand2,
	BTN_ReCommand3,
	//BroadPlane
	BTN_SendFile,
	Pro_FileSize,
	Pro_FileRecved,

};
struct SoftwareCommend//控制指令结构体
{
	SoftwareCommendHeadr header;
	unsigned int buttonID;
	unsigned int numComboValue;
	unsigned int comboValues[4];
};
void SetSoftwareCommend(SoftwareCommend* cmd,
					unsigned int dstNodeID,unsigned int function,unsigned int srcAddr,unsigned int dstAddr,
					unsigned int buttonID,unsigned int numComboValue,const unsigned int comboValues[]);

struct Cmd_DeliverConfig//配置文件传递
{
	SoftwareCommendHeadr header;
	unsigned int frameID;
	unsigned int frameSize;
	unsigned int endFlag;
	unsigned char data[1024];
};
void SetCmd_DeliverConfig(Cmd_DeliverConfig* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char data[]);
void SetCmd_DeliverConfig(Cmd_DeliverConfig* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char data[]);

struct Cmd_Start//启动命令
{
	SoftwareCommendHeadr header;
};
void SetCmd_Start(Cmd_Start* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr);
void SetCmd_Start(Cmd_Start* cmd,
	const SoftwareCommendHeadr& commonHeader);

struct Cmd_SndCollectOrder//发送数据采集命令
{
	SoftwareCommendHeadr header;
	unsigned int spot;
	unsigned int numTimes;
	unsigned int numFrames;
};
void SetCmd_SndCollectOrder(Cmd_SndCollectOrder* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int spot,unsigned int numTimes,unsigned int numFrames);
void SetCmd_SndCollectOrder(Cmd_SndCollectOrder* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int spot,unsigned int numTimes,unsigned int numFrames);

struct Cmd_SndReloadOrder//发送数据重载命令
{
	SoftwareCommendHeadr header;
	unsigned int spot;
	unsigned int numTimes;
	unsigned int numFrames;
};
void SetCmd_SndReloadOrder(Cmd_SndReloadOrder* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int spot,unsigned int numTimes,unsigned int numFrames);
void SetCmd_SndReloadOrder(Cmd_SndReloadOrder* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int spot,unsigned int numTimes,unsigned int numFrames);

struct Cmd_IntvModeParams//模块干预参数数据
{
	SoftwareCommendHeadr header;//报头
	unsigned int spot;//作用点
	unsigned int paramName;//参数名
	unsigned int paramValue;//参数值
};
enum ParamName_Baseband
{
	ModMode,
};
enum ParamValue_ModMode
{
	ModMode_Self,
	ModMode_Combined,
};
enum ParamName_Protocol
{
	QueuePriority,//队列优先级
};
enum ParamValue_Protocol
{
	Queue_Pri0,
	Queue_Pri1,
	Queue_Pri2,
	Queue_Pri3,
	Queue_Pri4,
	Queue_Pri5,
	Queue_Pri6,
	Queue_Pri7,
};
void SetCmd_IntvModeParams(Cmd_IntvModeParams* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int spot, unsigned int paramName,unsigned int paramValue);
void SetCmd_IntvModeParams(Cmd_IntvModeParams* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int spot, unsigned int paramName,unsigned int paramValue);

struct Cmd_RecontructMode//模块重构
{
	SoftwareCommendHeadr header;//包头
	unsigned int spot;//作用点
	unsigned int endFlag;//结束标志
	unsigned int frameID;//帧号
	unsigned int frameSize;//帧长
	unsigned char data[512];//数据
};
void SetCmd_RecontructMode(Cmd_RecontructMode* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int spot,unsigned int endFlag,unsigned int frameID,unsigned int frameSize,const unsigned char data[]);
void SetCmd_RecontructMode(Cmd_RecontructMode* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int spot,unsigned int endFlag,unsigned int frameID,unsigned int frameSize,const unsigned char data[]);

#define MAX_NUM_PARAMS 10
struct Cmd_ShowStatisticsOrder//显示指令
{
	SoftwareCommendHeadr header;//报头
	unsigned int paramNum;//参数数量
	unsigned int paramName[MAX_NUM_PARAMS];//参数名
};
void SetCmd_ShowStatisticsOrder(Cmd_ShowStatisticsOrder* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int paramNum,const unsigned int paramName[MAX_NUM_PARAMS]);
void SetCmd_ShowStatisticsOrder(Cmd_ShowStatisticsOrder* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int paramNum,const unsigned int paramName[MAX_NUM_PARAMS]);
struct Cmd_ShowStatisticsData//显示报文
{
	SoftwareCommendHeadr header;//报头
	unsigned int paramNum;//参数数量
	float paramValue[MAX_NUM_PARAMS];//参数值
};
void SetCmd_ShowStatisticsData(Cmd_ShowStatisticsData* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int paramNum,const float paramValue[MAX_NUM_PARAMS]);
void SetCmd_ShowStatisticsData(Cmd_ShowStatisticsData* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int paramNum,const float paramValue[MAX_NUM_PARAMS]);

struct Cmd_ProtoclBusinessHeader//协议业务源
{
	SoftwareCommendHeadr header;
	unsigned int type;
	unsigned int frameID;
	unsigned int frameSize;
};
void SetBusinessHeader(Cmd_ProtoclBusinessHeader* cmd,
						unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
						unsigned int type,unsigned int frameID,unsigned int frameSize);
void SetBusinessHeader(Cmd_ProtoclBusinessHeader* cmd,
						const SoftwareCommendHeadr& commonHeader,
						unsigned int type,unsigned int frameID,unsigned int frameSize);

struct Cmd_Close//关闭命令
{
	SoftwareCommendHeadr header;
};
void SetCmd_Close(Cmd_Close* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr);
void SetCmd_Close(Cmd_Close* cmd,
	const SoftwareCommendHeadr& commonHeader);

/************************************************************************/
/*业务类型定义                                                                      */
/************************************************************************/
enum BusinessType
{
	Business_AddIn			=0x01,
	Business_Command		=0x02,
};

enum ActionType
{
	Action_StartGuide		=0x11,
	Action_Start			=0x12,
	Action_Clear			=0x13,

	Reaction_AddIn			=0x21,
	Reaction_Exit			=0x22,
};

struct Busi_AddIn//接入
{
	Cmd_ProtoclBusinessHeader header;
	unsigned int networkID;
	unsigned int nodeID;
	unsigned int action;
};
void SetBusi_AddIn(Busi_AddIn* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int frameID,
	unsigned int networkID,unsigned int nodeID,unsigned int action);

enum CommandType
{
	Command1,
	Command2,
	Command3,
	Command4,
	Command5,
	Command6,
};
enum ReCommandType
{
	ReCommandYes,
	ReCommandNo,
	ReCommand1,
	ReCommand2,
	ReCommand3,

	Num_ReCommands
};
extern const CString COMMANDS[];
extern const CString RECOMMANDS[];
struct Busi_Command
{
	Cmd_ProtoclBusinessHeader header;
	unsigned int nodeID;
	unsigned int commandType;
};
void SetBusi_Command(Busi_Command* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int frameID,
	unsigned int nodeID,unsigned int commandType);
/************************************************************************/
/* 操作方法                                                                     */
/************************************************************************/


//SoftwareCommendHeadr explainCommendHeader(int* cmd);
//Cmd_ProtoclBusinessHeader explainBusinessHeader(int* cmd);
//Busi_AddIn explainBusinessCommend(int* cmd);