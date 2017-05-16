#include "Reversi.h"
int CurrentPlayer = -1;
int Finally_result[20][2];
int Test_Time = 15;
double result1 = 0;
double result2 = 0;
int Count = 0;

//初始棋盘
int Board[8][8] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 2, 0, 0, 0 },
		{ 0, 0, 0, 2, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
};
int b[8][8] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 2, 0, 0, 0 },
		{ 0, 0, 0, 2, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
};
int Ini_Board[8][8] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 2, 0, 0, 0 },
		{ 0, 0, 0, 2, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
};
/*
int Board[8][8] = {
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 2, 2, 2, 2, 2, 2, 2, 2 },
{ 2, 2, 1, 2, 2, 0, 0, 2 },
{ 2, 2, 2, 1, 2, 2, 1, 2 },
{ 2, 1, 2, 1, 2, 1, 0, 0 },
{ 0, 0, 1, 1, 0, 0, 0, 0 },
{ 0, 0, 2, 2, 2, 0, 0, 0 }
};*/
Reversi::Reversi()
{

}

Reversi::~Reversi()
{
}

void Reversi::Action(){
	



	reset_location(3);
	PrintBoard();


	BlackAIPlay();


	
}

//Ai palys Black
void Reversi::BlackAIPlay(){

	int EndFlag = 0;
	_Coordinate Nextpostion(-1, -1);

	AI ai_new(BlackPlayer, this, 1);

	DataTransfer transfer;

	Initail();
	while (true){
		if (!feasible_location_playerB.empty()){
			Count++;
			EndFlag++;

			//获得AI决策策略
			Nextpostion = ai_new.Play();
			printf("x:%d, y:%d\n", Nextpostion.first, Nextpostion.second);
			CurrentPlayer = WhitePlayer;
			//更新棋盘
			Update(Nextpostion, Black);
			//打印棋盘
			PrintBoard();

			//send board to the UI front
			transfer.SendToUI(Nextpostion.first, Nextpostion.second,Board);

		}
		if (!feasible_location_playerW.empty()){
			Count++;
			EndFlag++;

			printf("please input in the coordinate:\n");
			
			//get the next coordinate from UI(human player)
			Nextpostion = transfer.GetFromUI();

			CurrentPlayer = BlackPlayer;
			//更新棋盘
			Update(Nextpostion, White);
			PrintBoard();

			//send board to the UI front
			transfer.SendToUI(Board);

		}
		if (EndFlag == 0) {
			break;
		}
		EndFlag = 0;
	}
	printf("Blak:%d\nWhite:%d\n", ScoreB, ScoreW);
}


void Reversi::WhiteAIPlay(){
	int EndFlag = 0;
	_Coordinate Nextpostion(-1, -1);

	AI ai_new(WhitePlayer, this, 1);

	int number = 0;
	int i = 0;

	Initail();
	while (true){

		if (!feasible_location_playerB.empty()){
			Count++;
			EndFlag++;

			printf("please input in the coordinate:\n");
			scanf("%d %d", &Nextpostion.first, &Nextpostion.second);
			
			CurrentPlayer = WhitePlayer;
			Update(Nextpostion, Black);
			PrintBoard();


		}
		if (!feasible_location_playerW.empty()){
			Count++;
			EndFlag++;

			Nextpostion = ai_new.Play();
			printf("x:%d, y:%d\n", Nextpostion.first, Nextpostion.second);

			CurrentPlayer = BlackPlayer;
			Update(Nextpostion, White);
			PrintBoard();

		}
		if (EndFlag == 0) {
			break;
		}
		EndFlag = 0;
	}
	printf("Blak:%d\nWhite:%d\n", ScoreB, ScoreW);
}


//初始化
void Reversi::Initail(){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			Board[i][j] = Ini_Board[i][j];
		}
	}
	reset_location(3);

}



//根据落子更新棋盘
void Reversi::Update(_Coordinate Pos, int type){


	Board[Pos.first][Pos.second] = type;
	for (int i = 0; i < 8; i++){
		if (wheather_turn(Pos, i)){
			int cx = Pos.first + dx[i];
			int cy = Pos.second + dy[i];

			while (Board[cx][cy] != type) {
				Board[cx][cy] = type;
				cx = cx + dx[i];
				cy = cy + dy[i];
				// System.out.println("cx:"+cx+"cy:"+cy+"state:"+board[cx][cy]);
			}
		}
	}

	//重置得到可以落子的地方
	reset_location(0);

	UpdateScore();
}


//计算散度
int Reversi::Calculate_divergence(_Coordinate Pos, int type){
	BackupBoard();
	int result = 0;
	Board[Pos.first][Pos.second] = type;
	for (int i = 0; i < 8; i++){
		if (wheather_turn(Pos, i)){
			int cx = Pos.first + dx[i];
			int cy = Pos.second + dy[i];
			while (Board[cx][cy] != type) {
				result += get_divergence(cx, cy);
				//printf("#%ld", result);
				cx = cx + dx[i];
				cy = cy + dy[i];

			}
		}
	}

	RestoreBoard();
	return result;
}

//得到散度
int Reversi::get_divergence(int x, int y){
	int result = 0;
	int cx, cy;
	for (int i = 0; i < 8; i++){
		cx = x + dx[i];
		cy = y + dy[i];
		if (0 <= cx&&cx < 8 && 0 <= cy&&cy < 8){
			if (Board[cx][cy] == 0){
				result++;
			}
		}
	}
	return result;
}


//更新分数
void Reversi::UpdateScore(){
	ScoreB = 0;
	ScoreW = 0;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (Board[i][j] == Black){
				ScoreB++;
			}
			if (Board[i][j] == White){
				ScoreW++;
			}
		}
	}
}


//检测谁是赢家
int Reversi::winner(){
	UpdateScore();
	if (ScoreW > ScoreB){
		return WhitePlayer;
	}
	else if (ScoreB > ScoreW){
		return  BlackPlayer;
	}
	else {
		return 0;
	}
}


//检测该位置是否是可以下的地方
bool Reversi::wheather_turn(_Coordinate pos, int i){
	int state = Board[pos.first][pos.second];
	int cx = pos.first + dx[i];
	int cy = pos.second + dy[i];

	//提前剪枝
	if (cx >= 8 || cy >= 8 || cx < 0 || cy < 0 || Board[cx][cy] == state || Board[cx][cy] == 0) {
		return false;
	}
	while ((0 <= cx) && (cx < 8) && (0 <= cy) && (cy < 8)) {
		if (Board[cx][cy] == state) {
			return true;
		}
		if (Board[cx][cy] == 0){
			return false;
		}
		cx = cx + dx[i];
		cy = cy + dy[i];
	}
	return false;
}


//重置得到当前棋盘的合法落子
void Reversi::reset_location(int type) {
	feasible_location_playerB.clear();
	feasible_location_playerW.clear();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (Board[i][j] == 0){
				for (int k = 0; k < 8; k++) {
					if (type == 3){		//全部都要更新的
						Is_feasible(i, j, k, true);
						//int tmp = Is_feasible(i, j, k,true);			//该子是那一个可以下的
						/*if (tmp == Black) {
							_Coordinate a(i, j);
							feasible_location_playerB.insert(a);

							}
							if (tmp == White) {
							_Coordinate a(i, j);
							feasible_location_playerW.insert(a);

							}*/
					}
					else{
						MyTimer timer1, timer2;//只更新当前player用到的
						timer1.start();
						Is_feasible(i, j, k);
						timer1.stop();

						result1 += timer1.elapse();

						/*timer2.start();
						if (Is_feasible2(i, j, k) == Black) {
						_Coordinate a(i, j);
						feasible_location_playerB.insert(a);

						}
						;
						if (Is_feasible2(i, j, k) == White) {
						_Coordinate a(i, j);
						feasible_location_playerW.insert(a);

						}
						timer2.stop();
						result2 += timer2.elapse();
						*/
					}



				}
			}
			//	printf("j:%d",j);
		}
		//	printf("i:%d",i);
	}
	//printf("4");
}

//检测当前位置是否为合法位置
void Reversi::Is_feasible(int x, int y, int i, bool both)
{
	int cx = x + dx[i];
	int cy = y + dy[i];
	int state = 0;
	int flag = -1;

	if (cx >= 8 || cy >= 8 || cx < 0 || cy < 0 || Board[cx][cy] == 0) {
		return;
	}
	else if (Board[cx][cy] == White) {
		state = White;
	}
	else {
		state = Black;
	}

	cx = x + dx[i];
	cy = y + dy[i];
	while ((0 <= cx) && (cx < 8) && (0 <= cy) && (cy < 8)) {
		//System.out.println("cx:"+M[0]+"cy:"+M[1]);
		//printf("%d%d", dx[i], dy[i]);
		if (Board[cx][cy] == state) {
			cx = cx + dx[i];
			cy = cy + dy[i];
		}
		else if (Board[cx][cy] == 0) {
			break;
		}
		else {
			flag = Board[cx][cy];
			break;
		}
	}
	if (flag == BlackPlayer){
		_Coordinate a(x, y);
		feasible_location_playerB.insert(a);
	}
	if (flag == WhitePlayer){
		_Coordinate a(x, y);
		feasible_location_playerW.insert(a);
	}

}


//检测当前位置是否为合法可以落子的位置
void Reversi::Is_feasible(int x, int y, int i)
{
	int cx = x + dx[i];
	int cy = y + dy[i];
	int state = 0;
	int flag = -1;


	if (CurrentPlayer == White) {
		state = Black;
	}
	else {
		state = White;
	}

	if (cx >= 8 || cy >= 8 || cx < 0 || cy < 0 || Board[cx][cy] != state) {
		return;
	}



	cx = x + dx[i];
	cy = y + dy[i];
	while ((0 <= cx) && (cx < 8) && (0 <= cy) && (cy < 8)) {
		//System.out.println("cx:"+M[0]+"cy:"+M[1]);
		//printf("%d%d", dx[i], dy[i]);
		if (Board[cx][cy] == state) {
			cx = cx + dx[i];
			cy = cy + dy[i];
		}
		else if (Board[cx][cy] == 0) {
			return;
		}
		else {
			flag = Board[cx][cy];
			break;
		}
	}
	//如果是，更新结果
	if (flag == BlackPlayer){
		_Coordinate a(x, y);
		feasible_location_playerB.insert(a);
	}
	if (flag == WhitePlayer){
		_Coordinate a(x, y);
		feasible_location_playerW.insert(a);
	}
}


int Reversi::Is_feasible2(int x, int y, int i)
{
	int cx = x + dx[i];
	int cy = y + dy[i];
	int state = 0;


	if (cx >= 8 || cy >= 8 || cx < 0 || cy < 0 || Board[cx][cy] == 0) {
		return 0;
	}
	else if (Board[cx][cy] == White) {
		state = White;
	}
	else {
		state = Black;
	}

	cx = x + dx[i];
	cy = y + dy[i];
	while ((0 <= cx) && (cx < 8) && (0 <= cy) && (cy < 8)) {
		//System.out.println("cx:"+M[0]+"cy:"+M[1]);
		//printf("%d%d", dx[i], dy[i]);
		if (Board[cx][cy] == state) {
			cx = cx + dx[i];
			cy = cy + dy[i];
		}
		else if (Board[cx][cy] == 0) {
			return 0;
		}
		else {
			return Board[cx][cy];
		}
	}
	return 0;
}


//打印棋盘
void Reversi::PrintBoard(){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (i == 0 && j == 0){
				printf(" 01234567\n");
			}
			if (j == 0){
				printf("%d", i);
			}
			if (Board[i][j] == 1){
				printf("O");
			}
			if (Board[i][j] == 2){
				printf("X");
			}
			if (Board[i][j] == 0){
				printf(".");
			}
			//printf("%d",Board[i][j]);
		}
		printf("\n");
	}
	printf("########\n");
}

//得到一个随机的合法位置来落子
_Coordinate Reversi::getPostion(set<_Coordinate> location){
	srand((unsigned)time(0));
	int ran_num = rand() % location.size();


	set<_Coordinate>::iterator iter = location.begin();

	int i = 0;
	while (iter != location.end())
	{
		if (i == ran_num){
			return *iter;
		}
		i++;
		++iter;
	}

	_Coordinate error(-2, -2);
	return error;
}


//检查游戏是否结束
bool Reversi::checkTerminal()
{
	if ((!check_feasible_location(WhitePlayer)) && (!check_feasible_location(BlackPlayer))) {
		return true;
	}
	else {
		return false;
	}
}

//得到所有的合法位置
set<_Coordinate> Reversi::get_loaction(int playerType){
	if (playerType == WhitePlayer) {
		return feasible_location_playerW;
	}
	else {
		return feasible_location_playerB;
	}
}

//检查是否有合法位置可下
bool Reversi::check_feasible_location(int player) {
	if (player == BlackPlayer) {
		if (feasible_location_playerB.empty()) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		if (feasible_location_playerW.empty()) {
			return false;
		}
		else {
			return true;
		}
	}
}

_Coordinate Reversi::getPostion(int i){
	int p[200] = { 2, 0, 4, 2, 5, 2, 6, 2, 5, 1, 3, 2, 6, 1, 2, 4, 4, 1, 1, 2, 1, 4, 6, 0, 7, 0, 3, 1, 3, 0, 2, 2, 2, 5, 5, 0, 7, 2, 6, 3, 4, 5, 1, 6, 3, 6, 5, 6, 2, 1, 2, 7, 0, 3, 0, 1, 7, 4, 5, 3, 4, 7, 1, 1, 1, 3, 2, 0, 4, 0, 7, 1, 7, 3, 7, 5, 0, 6, 0, 7, 0, 0, 5, 4, 1, 5, 5, 7, 6, 6, 1, 7, 6, 4, 1, 0, 7, 6 };

	_Coordinate result;

	result.first = p[2 * i];
	result.second = p[2 * i + 1];

	return result;
}

//保存棋局
void Reversi::BackupBoard(){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			b[i][j] = Board[i][j];
		}
	}
}
//恢复棋局
void Reversi::RestoreBoard(){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			Board[i][j] = b[i][j];
		}
	}
}

