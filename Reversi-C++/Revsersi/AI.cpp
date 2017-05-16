#include "AI.h"
#include"Reversi.h"


//ѵ���õ���3��reward��Ȩ��ϵ��
double Policy_Const = 1;
double Divergence_Const = -0.4;
double Pos_Const1 = 0.4;
double Pos_Const2 = 0.3;


//����ǰ��λ�ü�ֵ����
int PostionValue1[8][8] = {
	120, 5, 20, 15, 15, 20, 5, 120,
	5, 1, 10, 11, 11, 10, 1, 5,
	20, 10, 12, 12, 12, 12, 10, 20,
	15, 11, 12, 10, 10, 12, 11, 15,
	15, 11, 12, 10, 10, 12, 11, 15,
	20, 10, 12, 12, 12, 12, 10, 20,
	5, 1, 10, 11, 11, 10, 1, 5,
	120, 5, 20, 15, 15, 20, 5, 120
};

//���̺��λ�ü�ֵ����
int PostionValue2[8][8] = {
	30, 8, 20, 15, 15, 20, 8, 30,
	8, 1, 10, 11, 11, 10, 1, 8,
	20, 10, 12, 12, 12, 12, 10, 20,
	15, 11, 12, 10, 10, 12, 11, 15,
	15, 11, 12, 10, 10, 12, 11, 15,
	20, 10, 12, 12, 12, 12, 10, 20,
	8, 1, 10, 11, 11, 10, 1, 8,
	30, 8, 20, 15, 15, 20, 8, 30
};
AI::AI()
{
}
AI::AI(int type, Reversi * re,int kind){
	PlayerType = type;
	r = re;
	AiType = kind;
}

AI::~AI()
{
}

void AI::setTime(int t){
	Simulation_time = t;
}


//��Ҫִ�г���
_Coordinate AI::Play()
{
	clock_t start, finish, begin, begin1;
	double   duration=0;
	
	Node *root = new Node(NULL, getCurrentID(), r->checkTerminal(), r->get_loaction(PlayerType),PlayerType);
	int simulation_time = 0;

	BackupBoard();
	start = clock();
	begin = clock();
	
	//MCTS ����
	while (duration < Simulation_time){
		int winner = -1;
	
		begin1 = clock();
		Node *v = TreePolicy(root);
		

		winner = DefaultPolicy(v);
	
		BP(v, winner);
		simulation_time++;
		RestoreBoard();
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
	}
	printf("%d", simulation_time);
	//printf("Reward:%lf\n", root->bestchild->Reward);
	if (AiType == 0){
		return BestChild(root, C)->pos;
	}
	else{
		//����MCTS ɢ�ȣ�λ�ü�ֵ��reward
		BestChild_Policy(root);
		BestChild_MCTS(root, C);
		//��3��reward����ѡ�����Ž��
		Node *result = BestChild_Both(root);
		printf("best reward is:%lf\n", result->total_reward);
		return result->pos;
	}
}


//BP���½������
void AI::BP(Node* v, int winner){
	Node*tmp = v;
	while (v != NULL) {

		v->update_Reward_VisitTime(winner);
		//updateBestchild(v);
		v = v->Parent;
	}
}

//�õ����ID
string AI::getCurrentID(){
	string result;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result += (Board[i][j] + "");
		}
	}
	return result;
}




//MCTS ����չ����
Node * AI::TreePolicy(Node *root){
	Node *tmp = root;
	while (!tmp->terminal){
		if (tmp->Wheather_FullyExpand()) {
			tmp = BestChild(tmp, C);
		}
		else {

			return Expand(tmp);
		}
	}
}


//ѡ�������ӽڵ�
Node* AI::BestChild(Node *root, double c)
{
	vector<Node*> Children = root->childist;
	Node *result = NULL;
	int Rootnumber = root->VisitTime;
	double max = -1000;
	Node* children = NULL;

	//��ǰ���û���ӿ�����
	while (Children.size() == 0){
		printf("fuck you");
	
	}
	for (int i = 0; i < Children.size(); i++) {
		children = Children[i];
		int childrenNumber = children->VisitTime;
		double markValue = 1.0 * children->Reward / childrenNumber + c * sqrt(2.0 * log(Rootnumber) / childrenNumber);
		if (markValue > max) {
			max = markValue;
			result = children;
		}
	}

	return result;
}

//ѡ��MCTS�����Ž��
void AI::BestChild_MCTS(Node *root, double c){
	vector<Node*> Children = root->childist;
	Node *result = NULL;
	int Rootnumber = root->VisitTime;
	double max = -1000;
	Node* children = NULL;

	//��ǰ���û���ӿ�����
	while (Children.size() == 0){
		printf("fuck you");

	}

	//����õ����ֵ
	for (int i = 0; i < Children.size(); i++) {
		children = Children[i];
		int childrenNumber = children->VisitTime;
		double markValue = 1.0 * children->Reward / childrenNumber + c * sqrt(2.0 * log(Rootnumber) / childrenNumber);
		children->Mcts_Reward = markValue;
		if (markValue > max) {
			max = markValue;
			result = children;
		}
	}

	//���й�һ�������������ֵ��Ϊ1��������ֵ�������ֵ����
	for (int i = 0; i < Children.size(); i++) {
		children = Children[i];
		if (children->Mcts_Reward > max){
			printf("fuck you");
		}
		children->Mcts_Reward = children->Mcts_Reward/max;
		printf("MCTS:%lf\tPolicy:%lf\n", children->Mcts_Reward, children->Policy_Reward);
	}

	//return result;
}

//��չ����
Node* AI::Expand(Node* tmp){
	int nodePlayer = -1;

	//�õ���һ�����ӵ����
	if (tmp->player == WhitePlayer) {
		nodePlayer = BlackPlayer;
	}
	else {
		nodePlayer = WhitePlayer;
	}

	int LocationSize = tmp->feasible_location.size();
	srand((unsigned)time(0));
	
	//��ǰѡ��û���ӿ�����
	if (LocationSize == 0){
		_Coordinate empty(-1, -1);  //��ʾ��ǰѡ��û���ӿ�����
		Node *result = new Node(tmp, tmp->ID, r->checkTerminal(), r->get_loaction(nodePlayer), nodePlayer, empty);
		return result;
	}
	int ran_num = rand() % LocationSize;
	
	int s;
	_Coordinate postion(-1,-1);


	//ѡ��һ�������û�б����ʹ���
	set<_Coordinate>::iterator iter = tmp->feasible_location.begin();
	
	int i = 0;
	while (iter != tmp->feasible_location.end())
	{
		if (i == ran_num){
			postion= *iter;
			break;
		}
		i++;
		++iter;
	}

	CurrentPlayer = nodePlayer;
	r->Update(postion, tmp->player);
	string nodeID = getCurrentID();

	//���ؽ������������
	Node *result = new Node(tmp, nodeID, r->checkTerminal(), r->get_loaction(nodePlayer), nodePlayer, postion);
	tmp->childist.push_back(result);
	tmp->feasible_location.erase(postion);

	return result;
}


//MCTS Ĭ�ϲ���ģ��
int AI::DefaultPolicy(Node* v)
{
	Node *tmp = v;
	Node *ttmp ;
	clock_t  finish2,  begin2;
	double duration = 0;

	double reward = 0;
	int winner = -1;
	begin2 = clock();


	//�������ģ��
	while (!tmp->terminal) {
		tmp = NextNode(tmp);
	}
	
	finish2 = clock();
	duration = (double)(finish2 - begin2) / CLOCKS_PER_SEC;
	//printf("fuck:%lf#", duration);

	//�ͷ�ָ��
	ttmp = tmp;
	while (tmp != v){
		ttmp = tmp->Parent;
		delete tmp;
		tmp = ttmp;
	}
	tmp = ttmp = NULL;

	//r->PrintBoard();
	return r->winner();
}


//���ѡ����һ��action��ģ��
Node *AI::NextNode(Node* tmp){
	Node* result = NULL;
	int nodePlayer = -1;

	int freeLocationSize = tmp->location.size();

	//��һ������һ��type
	if (tmp->player == WhitePlayer) {
		nodePlayer = BlackPlayer;
	}
	else {
		nodePlayer = WhitePlayer;
	}
	//��ǰû�еط��������ӣ�ֱ�ӻ�����һ��
	if (freeLocationSize <= 0){

		result = new Node(tmp,r->checkTerminal(), r->get_loaction(nodePlayer), nodePlayer);

		return result;
	}
	srand((unsigned)time(0));
	int ran_num = rand() % freeLocationSize;
	

	//ѡ����һ��Action
	set<_Coordinate>::iterator iter = tmp->location.begin();
	_Coordinate postion;
	int i = 0;
	while (iter != tmp->location.end())
	{
		if (i == ran_num){
			postion = *iter;
			break;
		}
		i++;
		++iter;
	}
	
	CurrentPlayer = nodePlayer;
	r->Update(postion, tmp->player);


	//�õ����
	result = new Node(tmp, r->checkTerminal(), r->get_loaction(nodePlayer), nodePlayer);
	//result = new Node(rule.checkTerminal(), rule.get_loaction(nodePlayer), nodePlayer);
	

	return result;
}


//��������
void AI::BackupBoard(){
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = Board[i][j];
		}
	}
}

//�ָ�����
void AI::RestoreBoard(){
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Board[i][j] = board[i][j];
		}
	}
	r->reset_location(0);
}

//����λ�ü�ֵ
void AI::Calculate_Pos(Node *tmp){
	//�����Ƿ�Ϊ���̺�ѡ��ͬ�ļ�ֵ����
	if (Count < 20){
		tmp->Pos_Reward = PostionValue1[tmp->pos.first][tmp->pos.second];
	}
	else{
		tmp->Pos_Reward = PostionValue2[tmp->pos.first][tmp->pos.second];
	}
	
	if (tmp->Pos_Reward>=100){
		tmp->Recommond = 1;
	}
	if (Count < 55){
		if (tmp->Pos_Reward < 2){
			tmp->Recommond = -1;
		}
	}
}


//����õ�ɢ�Ⱥ�λ�ü�ֵ��reward
void AI::BestChild_Policy(Node* v){

	double max_divergence = 0;
	double max_pos = 0;
	double Pos_Const = 0;

	//�ж��Ƿ�Ϊ�������
	if (Count < 25){
		Pos_Const = Pos_Const1;
	}
	else{
		Pos_Const = Pos_Const2;
	}

	//����õ����ɢ�Ⱥ����λ�ü�ֵ
	for (int i = 0; i < v->childist.size(); i++){
		
		Calculate_divergence(v->childist[i]);
		Calculate_Pos(v->childist[i]);
		

		if (v->childist[i]->Diver_Reward > max_divergence){
			max_divergence = v->childist[i]->Diver_Reward;
		}
		
		if (v->childist[i]->Pos_Reward > max_pos){
			 max_pos = v->childist[i]->Pos_Reward;
		}
	}
	if (max_divergence == 0){
		max_divergence = 1;
	}

	//��һ�����������£������ֵΪ1�������������ֵ���Ҹ���Ȩ�صõ�reward
	for (int i = 0; i < v->childist.size(); i++){
		v->childist[i]->Policy_Reward = Divergence_Const*1.0*v->childist[i]->Policy_Reward / max_divergence + Pos_Const*v->childist[i]->Pos_Reward / max_pos;
		
		if (v->childist[i]->Recommond == 1){
			v->childist[i]->Policy_Reward += 1000;
		}
		if (v->childist[i]->Recommond == -1){
			v->childist[i]->Policy_Reward -= 1000;
		}

	}



}


//����ɢ��
void AI::Calculate_divergence(Node *v){
	//printf("love you");
	int result = 0;
	_Coordinate nextPos = v->pos;
	result = r->Calculate_divergence(nextPos, v->Parent->player);
	v->Diver_Reward = result;
	//return result;

}

//ѡ�������ӽڵ��3�����ῼ��
Node* AI::BestChild_Both(Node *v){
	Node *result;
	Node *chidren;
	double Max = -100;

	//�ҵ�reward���ֵ
	for (int i = 0; i < v->childist.size(); i++){
		chidren = v->childist[i];
		chidren->total_reward = chidren->Mcts_Reward + Policy_Const*chidren->Policy_Reward;
		if (chidren->total_reward > Max){
			result = chidren;
			Max = chidren->total_reward;

		}
	}
	
	return result;

}
