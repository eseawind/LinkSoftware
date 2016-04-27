#pragma once
struct Position//位置结构体
{
	float longtitude;
	float latitude;
	float height;
};

struct TrackInfo//航迹信息
{
	CString trackType;
	Position startPos;
	CString startPosStr;
	float velocity;
	Position dstPos;
	CString dstPosStr;
	CString VRule;
};

//节点类
//保存节点信息
class Node
{
public:
	CString nodeType;//节点类型

	int nodeID;//节点ID

	unsigned long long mAddrHex;

	CString fuelState;

	CString equipState;

	CString workState;

	CString frequence;

	TrackInfo trackInfo;
public:
	Node();
	void initial(int nodeID);
};

class Networks
{
public:
	Networks();
public:
	struct Network
	{
		bool isOpen;
		std::vector<int> nodeList;
	};
	std::vector<Network> mNetworks;
public:
	void open(int networkID);
	void addIn(int networkID,int nodeID);
	void quit(int networkID,int nodeID);
	void close(int networkID);
	void clear(int networkID);
};

class MasterNode: public Node
{
public:
	MasterNode(const Node& node);
public:
	Networks networks;

};

class SlaveNode: public Node
{
public:
	SlaveNode(const Node& node);
public:
	int mNetworkID;
};