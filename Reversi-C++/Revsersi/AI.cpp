#include "AI.h"
#include"Reversi.h"


//训练得到的3个reward的权重系数
double Policy_Const = 1;
double Divergence_Const = -0.4;
double Pos_Const1 = 0.4;
double Pos_Const2 = 0.3;


//中盘前的位置价值函数
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

//中盘后的位置价值函数
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


//主要执行程序
_Coordinate AI::Play()
{
	clock_t start, finish, begin, begin1;
	double   duration=0;
	
	Node *root = new Node(NULL, getCurrentID(), r->checkTerminal(), r->get_loaction(PlayerType),PlayerType);
	int simulation_time = 0;

	BackupBoard();
	start = clock();
	begin = clock();
	
	//MCTS 搜索
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
		//更新MCTS 散度，位置价值的reward
		BestChild_Policy(root);
		BestChild_MCTS(root, C);
		//从3个reward考虑选择最优结点
		Node *result = BestChild_Both(root);
		printf("best reward is:%lf\n", result->total_reward);
		return result->pos;
	}
}


//BP更新结点数据
void AI::BP(Node* v, int winner){
	Node*tmp = v;
	while (v != NULL) {

		v->update_Reward_VisitTime(winner);
		//updateBestchild(v);
		v = v->Parent;
	}
}

//得到棋局ID
string AI::getCurrentID(){
	string result;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result += (Board[i][j] + "");
		}
	}
	return result;
}




//MCTS 树扩展策略
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


//选择最优子节点
Node* AI::BestChild(Node *root, double c)
{
	vector<Node*> Children = root->childist;
	Node *result = NULL;
	int Rootnumber = root->VisitTime;
	double max = -1000;
	Node* children = NULL;

	//当前棋局没有子可以下
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

//选择MCTS的最优结点
void AI::BestChild_MCTS(Node *root, double c){
	vector<Node*> Children = root->childist;
	Node *result = NULL;
	int Rootnumber = root->VisitTime;
	double max = -1000;
	Node* children = NULL;

	//当前棋局没有子可以下
	while (Children.size() == 0){
		printf("fuck you");

	}

	//计算得到最大值
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

	//进行归一化操作，将最大值设为1，其他的值除以最大值更新
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

//扩展策略
Node* AI::Expand(Node* tmp){
	int nodePlayer = -1;

	//得到下一个棋子的身份
	if (tmp->player == WhitePlayer) {
		nodePlayer = BlackPlayer;
	}
	else {
		nodePlayer = WhitePlayer;
	}

	int LocationSize = tmp->feasible_location.size();
	srand((unsigned)time(0));
	
	//当前选手没有子可以下
	if (LocationSize == 0){
		_Coordinate empty(-1, -1);  //表示当前选手没有子可以下
		Node *result = new Node(tmp, tmp->ID, r->checkTerminal(), r->get_loaction(nodePlayer), nodePlayer, empty);
		return result;
	}
	int ran_num = rand() % LocationSize;
	
	int s;
	_Coordinate postion(-1,-1);


	//选择一个随机的没有被访问过的
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

	//返回结果，更新数据
	Node *result = new Node(tmp, nodeID, r->checkTerminal(), r->get_loaction(nodePlayer), nodePlayer, postion);
	tmp->childist.push_back(result);
	tmp->feasible_location.erase(postion);

	return result;
}


//MCTS 默认策略模拟
int AI::DefaultPolicy(Node* v)
{
	Node *tmp = v;
	Node *ttmp ;
	clock_t  finish2,  begin2;
	double duration = 0;

	double reward = 0;
	int winner = -1;
	begin2 = clock();


	//不断随机模拟
	while (!tmp->terminal) {
		tmp = NextNode(tmp);
	}
	
	finish2 = clock();
	duration = (double)(finish2 - begin2) / CLOCKS_PER_SEC;
	//printf("fuck:%lf#", duration);

	//释放指针
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


//随机选择下一个action来模拟
Node *AI::NextNode(Node* tmp){
	Node* result = NULL;
	int nodePlayer = -1;

	int freeLocationSize = tmp->location.size();

	//下一步是哪一种type
	if (tmp->player == WhitePlayer) {
		nodePlayer = BlackPlayer;
	}
	else {
		nodePlayer = WhitePlayer;
	}
	//当前没有地方可以落子，直接换成另一方
	if (freeLocationSize <= 0){

		result = new Node(tmp,r->checkTerminal(), r->get_loaction(nodePlayer), nodePlayer);

		return result;
	}
	srand((unsigned)time(0));
	int ran_num = rand() % freeLocationSize;
	

	//选择下一个Action
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


	//得到结果
	result = new Node(tmp, r->checkTerminal(), r->get_loaction(nodePlayer), nodePlayer);
	//result = new Node(rule.checkTerminal(), rule.get_loaction(nodePlayer), nodePlayer);
	

	return result;
}


//备份棋盘
void AI::BackupBoard(){
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = Board[i][j];
		}
	}
}

//恢复棋盘
void AI::RestoreBoard(){
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Board[i][j] = board[i][j];
		}
	}
	r->reset_location(0);
}

//计算位置价值
void AI::Calculate_Pos(Node *tmp){
	//根据是否为中盘后，选择不同的价值网络
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


//计算得到散度和位置价值的reward
void AI::BestChild_Policy(Node* v){

	double max_divergence = 0;
	double max_pos = 0;
	double Pos_Const = 0;

	//判断是否为棋局中盘
	if (Count < 25){
		Pos_Const = Pos_Const1;
	}
	else{
		Pos_Const = Pos_Const2;
	}

	//计算得到最大散度和最大位置价值
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

	//归一化操作并更新，将最大值为1，其他除以最大值并且根据权重得到reward
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


//计算散度
void AI::Calculate_divergence(Node *v){
	//printf("love you");
	int result = 0;
	_Coordinate nextPos = v->pos;
	result = r->Calculate_divergence(nextPos, v->Parent->player);
	v->Diver_Reward = result;
	//return result;

}

//选择最优子节点从3个恻泪考虑
Node* AI::BestChild_Both(Node *v){
	Node *result;
	Node *chidren;
	double Max = -100;

	//找到reward最大值
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
