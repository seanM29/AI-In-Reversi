#include "Node.h"


Node::Node()
{
}
Node::Node(Node * par, string nodeID, bool t, set<_Coordinate> nextlocation, int Player){
	Parent = par;
	ID = nodeID;
	terminal = t;
	location = nextlocation;

	maxChildrenSize = location.size();
	player = Player;
	feasible_location = location;
}
Node::Node(Node * par, string nodeID, bool t, set<_Coordinate> nextlocation, int Player, _Coordinate position){
	Parent = par;
	ID = nodeID;
	terminal = t;
	location = nextlocation;

	maxChildrenSize = location.size();
	player = Player;
	pos = position;
	feasible_location = location;
}
Node::Node(Node * par,bool t, set<_Coordinate> nextlocation, int Player){
	Parent = par;
	terminal = t;
	location = nextlocation;
	player = Player;
	feasible_location = location;

}

Node::~Node()
{
}

bool Node::Wheather_FullyExpand()
{
	//�����ǰѡ��û���ӿ����£���Ϊ��û����չ
	if (maxChildrenSize == 0 || childist.size()<maxChildrenSize){
		return false;
	}
	else {
		return true;
	}
}

//BP���½������
void Node::update_Reward_VisitTime(int winner){
	if (winner == player){
		Reward -= 1;
	}
	else{
		Reward += 1;
	}
	VisitTime++;
}
