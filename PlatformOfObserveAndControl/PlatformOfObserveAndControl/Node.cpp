#include "StdAfx.h"
#include <fstream>
#include "Globe.h"
#include "Node.h"
using namespace std;
//�ڵ����
Node::Node()
	: fuelState(_T("����"))
	, equipState(_T("����"))
	, workState(_T("����"))
	, frequence(_T("1 GHz"))
	, nodeID(-1)
{

}
void Node::initial(int nodeID)
{
	USES_CONVERSION;
	this->nodeID=nodeID;
	CString configPath;
	configPath.Format(_T(".\\config[%d].ini"),nodeID); //�����ļ����·��
	const CString sectionNode=_T("NodeInfo");
	const CString sectionTrack=_T("TrackInfo");
	const CString notFoundStr=_T("--");
	const int notFoundInt=-1;
	CFileFind finder;
	CString tempStr;

	if(!finder.FindFile(configPath))
	{
		nodeType=_T("�ڵ����ô���");
		return;
	}

	//�ڵ���Ϣ
	//�ڵ�ID
	if(nodeID!=GetPrivateProfileInt(sectionNode,_T("NodeID"),notFoundInt,configPath))
	{
		nodeType=_T("�ڵ����ô���");
		return;
	}
	//�ڵ�����
	GetPrivateProfileString(sectionNode,_T("NodeType"),notFoundStr,nodeType.GetBuffer(MAX_PATH),MAX_PATH,configPath);
	nodeType.ReleaseBuffer();
	//MAC
	mAddrHex=GetPrivateProfileInt(sectionNode,_T("IntfMAC"),notFoundInt,configPath);
	mDstHex=GetPrivateProfileInt(sectionNode,_T("DstMAC"),notFoundInt,configPath);

	//������Ϣ
	//�켣����
	GetPrivateProfileString(sectionTrack,_T("TrackType"),notFoundStr,trackInfo.trackType.GetBuffer(MAX_PATH),MAX_PATH,configPath);
	trackInfo.trackType.ReleaseBuffer();
	//��ʼ����
	GetPrivateProfileString(sectionTrack,_T("StartPosition"),notFoundStr,trackInfo.startPosStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
	trackInfo.startPosStr.ReleaseBuffer();
	if(trackInfo.startPosStr!=notFoundStr)
	{
		sscanf_s(T2A(trackInfo.startPosStr.GetBuffer(trackInfo.startPosStr.GetLength())),"(%f,%f,%f)",
			&trackInfo.startPos.longtitude,&trackInfo.startPos.latitude,&trackInfo.startPos.height);
		trackInfo.startPosStr.ReleaseBuffer();
	}
	//�ٶ�
	int Vinf,Vsup;
	GetPrivateProfileString(sectionTrack,_T("Velocity"),notFoundStr,tempStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
	tempStr.ReleaseBuffer();
	if(tempStr!=notFoundStr)
	{
		sscanf_s(T2A(tempStr.GetBuffer(tempStr.GetLength())),"(%f,%f)",&Vinf,&Vsup);
		tempStr.ReleaseBuffer();
		trackInfo.velocity=rand()*(float)(Vsup-Vinf)+(float)Vinf;
	}

	//Ŀ������
	GetPrivateProfileString(sectionTrack,_T("DestinatePosition"),notFoundStr,trackInfo.dstPosStr.GetBuffer(MAX_PATH),MAX_PATH,configPath);
	trackInfo.dstPosStr.ReleaseBuffer();
	if(trackInfo.dstPosStr!=notFoundStr)
	{
		sscanf_s(T2A(trackInfo.dstPosStr.GetBuffer(trackInfo.dstPosStr.GetLength())),"(%f,%f,%f)",
			&trackInfo.dstPos.longtitude,&trackInfo.dstPos.latitude,&trackInfo.dstPos.height);
		trackInfo.dstPosStr.ReleaseBuffer();
	}
	//��ǰ����
	GetPrivateProfileString(sectionTrack,_T("VRule"),notFoundStr,trackInfo.VRule.GetBuffer(MAX_PATH),MAX_PATH,configPath);
	trackInfo.VRule.ReleaseBuffer();
}

Networks::Networks()
{
	Network network1;
	network1.isOpen=false;
	Network network2;
	network2.isOpen=true;
	mNetworks.push_back(network1);
	mNetworks.push_back(network2);
}
void Networks::open(int networkID)
{
	mNetworks.at(networkID).isOpen=true;
}
void Networks::addIn(int networkID,int nodeID)
{
	if(mNetworks.at(networkID).isOpen)
	{
		mNetworks.at(networkID).nodeList.push_back(nodeID);
	}
}
void Networks::quit(int networkID,int nodeID)
{
	vector<int>::iterator iNetwork;
	for(iNetwork=mNetworks.at(networkID).nodeList.begin();
		iNetwork!=mNetworks.at(networkID).nodeList.end();
		iNetwork++)
	{
		if(*iNetwork==nodeID)
		{
			mNetworks.at(networkID).nodeList.erase(iNetwork);
			return;
		}
	}
}
void Networks::close(int networkID)
{
	mNetworks.at(networkID).isOpen=false;
}
void Networks::clear(int networkID)
{
	mNetworks.at(networkID).nodeList.clear();
}
//���ؽڵ�
MasterNode::MasterNode(const Node& node)
{
	nodeType=node.nodeType;

	nodeID=node.nodeID;

	mAddrHex=node.mAddrHex;

	fuelState=node.fuelState;

	equipState=node.equipState;

	workState=node.workState;

	frequence=node.frequence;

	trackInfo=node.trackInfo;
}
//���ؽڵ�
SlaveNode::SlaveNode(const Node& node)
	: mNetworkID(-1)
{
	nodeType=node.nodeType;

	nodeID=node.nodeID;

	mAddrHex=node.mAddrHex;

	fuelState=node.fuelState;

	equipState=node.equipState;

	workState=node.workState;

	frequence=node.frequence;

	trackInfo=node.trackInfo;
}