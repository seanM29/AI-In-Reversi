#pragma once
#include"Common.h"
class Node
{
public:
	Node();

	//��ʼ�����ɽ��
	Node(Node * par, string nodeID, bool t, set<_Coordinate> nextlocation, int Player);
		Node(Node * par, string nodeID, bool t, set<_Coordinate> nextlocation, int Player, _Coordinate position);
	Node(Node * par,bool t, set<_Coordinate> nextlocation, int Player);
	~Node();
	//Node* bestchild = NULL;
	double bestchild_Reward = -1000;
	double Reward = 0;
	double Mcts_Reward = 0;				//MCTS��reward
	double Policy_Reward = 0;			//ɢ�Ⱥ�λ�ü�ֵreward
	double Diver_Reward = 0;
	double Pos_Reward = 0;
	double total_reward = 0;

	int Recommond = 0;		//1��ǿ���Ƽ���-1��ǿ�Ҳ��Ƽ���0����� ���λ������Ľ��� ������߲ݶ�Ƥ
	int VisitTime = 0;
	set<_Coordinate> location;
	set<_Coordinate> feasible_location;  //�����µĿ���λ��
	bool terminal = false;
	string ID;
	Node* Parent;						//���׽��
	vector<Node*> childist;				//�ӽڵ�
	int maxChildrenSize = -1;
	int player = -1;
	_Coordinate pos;
	bool Wheather_FullyExpand();      //����Ƿ���ȫ��չ
	void update_Reward_VisitTime(int winner);   //����BP

	
};

