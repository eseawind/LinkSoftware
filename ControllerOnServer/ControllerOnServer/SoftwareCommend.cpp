#include "stdafx.h"
#include "SoftwareCommend.h"
/************************************************************************/
/* ��������                                                                     */
/************************************************************************/
//��׼ͷ
void SetCommendHeader(SoftwareCommendHeadr* cmd,
						unsigned int dstNodeID,unsigned int function,unsigned int srcAddr,unsigned int dstAddr)
{
	cmd->frontFlag=FRONT_FLAG;
	cmd->dstNodeID=dstNodeID;
	cmd->function=function;
	cmd->srcAddr=srcAddr;
	cmd->dstAddr=dstAddr;
}
//����
void SetSoftwareCommend(SoftwareCommend* cmd,
					unsigned int dstNodeID,unsigned int function,unsigned int srcAddr,unsigned int dstAddr,
					unsigned int buttonID,unsigned int numComboValue,const unsigned int comboValues[])
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_Cmd,srcAddr,dstAddr);
	cmd->buttonID=buttonID;
	cmd->numComboValue=numComboValue;
	memset(cmd->comboValues,0,numComboValue*sizeof(unsigned int));
	memcpy_s(cmd->comboValues,numComboValue*sizeof(unsigned int),comboValues,numComboValue*sizeof(unsigned int));
}
//����
void SetCmd_Start(Cmd_Start* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr)
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_Start,srcAddr,dstAddr);
}
void SetCmd_Start(Cmd_Start* cmd,
	const SoftwareCommendHeadr& commonHeader)
{
	cmd->header=commonHeader;
}

//�����ļ�����
void SetCmd_DeliverConfig(Cmd_DeliverConfig* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char data[])
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_DeliverConfig,srcAddr,dstAddr);
	cmd->frameID=frameID;
	cmd->frameSize=frameSize;
	cmd->endFlag=endFlag;
	memcpy_s(cmd->data,frameSize,data,frameSize);
}
void SetCmd_DeliverConfig(Cmd_DeliverConfig* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int frameID,unsigned int frameSize,unsigned int endFlag,unsigned char data[])
{
	cmd->header=commonHeader;
	cmd->frameID=frameID;
	cmd->frameSize=frameSize;
	cmd->endFlag=endFlag;
	memcpy_s(cmd->data,frameSize,data,frameSize);
}

//�ɼ�
void SetCmd_SndCollectOrder(Cmd_SndCollectOrder* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int spot,unsigned int numTimes,unsigned int numFrames)
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_SndCollectOrder,srcAddr,dstAddr);
	cmd->spot=spot;
	cmd->numTimes=numTimes;
	cmd->numFrames=numFrames;
}
void SetCmd_SndCollectOrder(Cmd_SndCollectOrder* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int spot,unsigned int numTimes,unsigned int numFrames)
{
	cmd->header=commonHeader;
	cmd->spot=spot;
	cmd->numTimes=numTimes;
	cmd->numFrames=numFrames;
}
//����
void SetCmd_SndReloadOrder(Cmd_SndReloadOrder* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int spot,unsigned int numTimes,unsigned int numFrames)
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_SndReloadOrder,srcAddr,dstAddr);
	cmd->spot=spot;
	cmd->numTimes=numTimes;
	cmd->numFrames=numFrames;
}
void SetCmd_SndReloadOrder(Cmd_SndReloadOrder* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int spot,unsigned int numTimes,unsigned int numFrames)
{
	cmd->header=commonHeader;
	cmd->spot=spot;
	cmd->numTimes=numTimes;
	cmd->numFrames=numFrames;
}
//��Ԥ
void SetCmd_IntvModeParams(Cmd_IntvModeParams* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int spot, unsigned int paramName,unsigned int paramValue)
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_IntvNodeParams,srcAddr,dstAddr);
	cmd->spot=spot;
	cmd->paramName=paramName;
	cmd->paramValue=paramValue;
}
void SetCmd_IntvModeParams(Cmd_IntvModeParams* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int spot, unsigned int paramName,unsigned int paramValue)
{
	cmd->header=commonHeader;
	cmd->spot=spot;
	cmd->paramName=paramName;
	cmd->paramValue=paramValue;
}
//�ع�
void SetCmd_RecontructMode(Cmd_RecontructMode* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int spot,unsigned int endFlag,unsigned int frameID,unsigned int frameSize,const unsigned char data[])
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_RecontructMode,srcAddr,dstAddr);
	cmd->spot=spot;
	cmd->endFlag=endFlag;
	cmd->frameID=frameID;
	cmd->frameSize=frameSize;
	memset(cmd->data,0,512);
	memcpy_s(cmd->data,frameSize,data,frameSize);
}
void SetCmd_RecontructMode(Cmd_RecontructMode* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int spot,unsigned int endFlag,unsigned int frameID,unsigned int frameSize,const unsigned char data[])
{
	cmd->header=commonHeader;
	cmd->spot=spot;
	cmd->spot=spot;
	cmd->endFlag=endFlag;
	cmd->frameID=frameID;
	cmd->frameSize=frameSize;
	memset(cmd->data,0,512);
	memcpy_s(cmd->data,frameSize,data,frameSize);
}
//��ʾ
void SetCmd_ShowStatisticsOrder(Cmd_ShowStatisticsOrder* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int paramNum,const unsigned int paramName[MAX_NUM_PARAMS])
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_ShowStatisticsOrder,srcAddr,dstAddr);
	cmd->paramNum=paramNum;
	memset(cmd->paramName,0,MAX_NUM_PARAMS*sizeof(unsigned int));
	memcpy_s(cmd->paramName,paramNum*sizeof(unsigned int),paramName,paramNum*sizeof(unsigned int));
}
void SetCmd_ShowStatisticsOrder(Cmd_ShowStatisticsOrder* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int paramNum,const unsigned int paramName[MAX_NUM_PARAMS])
{
	cmd->header=commonHeader;
	cmd->paramNum=paramNum;
	memset(cmd->paramName,0,MAX_NUM_PARAMS*sizeof(unsigned int));
	memcpy_s(cmd->paramName,paramNum*sizeof(unsigned int),paramName,paramNum*sizeof(unsigned int));
}
void SetCmd_ShowStatisticsData(Cmd_ShowStatisticsData* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int paramNum,const float paramValue[MAX_NUM_PARAMS])
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_ShowStatisticsData,srcAddr,dstAddr);
	cmd->paramNum=paramNum;
	memset(cmd->paramValue,0,MAX_NUM_PARAMS*sizeof(float));
	memcpy_s(cmd->paramValue,paramNum*sizeof(float),paramValue,paramNum*sizeof(float));
}
void SetCmd_ShowStatisticsData(Cmd_ShowStatisticsData* cmd,
	const SoftwareCommendHeadr& commonHeader,
	unsigned int paramNum,const float paramValue[MAX_NUM_PARAMS])
{
	cmd->header=commonHeader;
	cmd->paramNum=paramNum;
	memset(cmd->paramValue,0,MAX_NUM_PARAMS*sizeof(float));
	memcpy_s(cmd->paramValue,paramNum*sizeof(float),paramValue,paramNum*sizeof(float));
}
//ҵ��
void SetBusinessHeader(Cmd_ProtoclBusinessHeader* cmd,
						unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
						unsigned int type,unsigned int frameID,unsigned int frameSize)
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_ProtocolBusiness,srcAddr,dstAddr);
	cmd->type=type;
	cmd->frameID=frameID;
	cmd->frameSize=frameSize;
}
void SetBusinessHeader(Cmd_ProtoclBusinessHeader* cmd,
						const SoftwareCommendHeadr& commonHeader,
						unsigned int type,unsigned int frameID,unsigned int frameSize)
{
	cmd->header=commonHeader;
	cmd->type=type;
	cmd->frameID=frameID;
	cmd->frameSize=frameSize;
}
//�ر�
void SetCmd_Close(Cmd_Close* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr)
{
	SetCommendHeader((SoftwareCommendHeadr*)cmd,dstNodeID,Func_Close,srcAddr,dstAddr);
}
void SetCmd_Close(Cmd_Close* cmd,
	const SoftwareCommendHeadr& commonHeader)
{
	cmd->header=commonHeader;
}

void SetBusi_AddIn(Busi_AddIn* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int frameID,
	unsigned int networkID,unsigned int nodeID,unsigned int action)
{
	SetBusinessHeader((Cmd_ProtoclBusinessHeader*)cmd,dstNodeID,srcAddr,dstAddr,Business_AddIn,frameID,sizeof(Busi_AddIn));
	cmd->networkID=networkID;
	cmd->nodeID=nodeID;
	cmd->action=action;
}
const CString COMMANDS[]={
	_T("����1"),
	_T("����2"),
	_T("����3"),
	_T("����4"),
	_T("����5"),
	_T("����6"),
};
const CString RECOMMANDS[]={
	_T("ͬ��"),
	_T("�ܾ�"),
	_T("����1"),
	_T("����2"),
	_T("����3"),
};
void SetBusi_Command(Busi_Command* cmd,
	unsigned int dstNodeID,unsigned int srcAddr,unsigned int dstAddr,
	unsigned int frameID,
	unsigned int nodeID,unsigned int commandType)
{
	SetBusinessHeader((Cmd_ProtoclBusinessHeader*)cmd,dstNodeID,srcAddr,dstAddr,Business_Command,frameID,sizeof(Busi_Command));
	cmd->nodeID=nodeID;
	cmd->commandType=commandType;
}