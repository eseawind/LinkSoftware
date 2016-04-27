#pragma once
struct Position//λ�ýṹ��
{
	float longtitude;
	float latitude;
	float height;
};

struct TrackInfo//������Ϣ
{
	CString trackType;
	Position startPos;
	CString startPosStr;
	float velocity;
	Position dstPos;
	CString dstPosStr;
	CString VRule;
};

//�ڵ���
//����ڵ���Ϣ
class Node
{
public:
	CString nodeType;//�ڵ�����

	int nodeID;//�ڵ�ID

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