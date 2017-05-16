#pragma once
#include"Common.h"
class Node
{
public:
	Node();

	//初始化生成结点
	Node(Node * par, string nodeID, bool t, set<_Coordinate> nextlocation, int Player);
		Node(Node * par, string nodeID, bool t, set<_Coordinate> nextlocation, int Player, _Coordinate position);
	Node(Node * par,bool t, set<_Coordinate> nextlocation, int Player);
	~Node();
	//Node* bestchild = NULL;
	double bestchild_Reward = -1000;
	double Reward = 0;
	double Mcts_Reward = 0;				//MCTS的reward
	double Policy_Reward = 0;			//散度和位置价值reward
	double Diver_Reward = 0;
	double Pos_Reward = 0;
	double total_reward = 0;

	int Recommond = 0;		//1是强烈推荐，-1是强烈不推荐，0是随便 针对位置提出的建议 金角银边草肚皮
	int VisitTime = 0;
	set<_Coordinate> location;
	set<_Coordinate> feasible_location;  //可以下的空余位置
	bool terminal = false;
	string ID;
	Node* Parent;						//父亲结点
	vector<Node*> childist;				//子节点
	int maxChildrenSize = -1;
	int player = -1;
	_Coordinate pos;
	bool Wheather_FullyExpand();      //检测是否被完全扩展
	void update_Reward_VisitTime(int winner);   //更新BP

	
};

