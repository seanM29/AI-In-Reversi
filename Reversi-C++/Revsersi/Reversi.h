#pragma once

#include"Common.h"
#include"AI.h"
#include"DataTransfer.h"
class Reversi
{
private:
	set<_Coordinate> feasible_location_playerB;
	set<_Coordinate> feasible_location_playerW;
	int ScoreB=2;
	int ScoreW = 2;
	
public:
	Reversi();
	~Reversi();
	void Action();    //游戏执行主要逻辑
	void Update(_Coordinate pos, int type);    //更新棋盘和数据
	void UpdateScore();
	
	int winner();
	void PrintBoard();							//打印棋盘
	bool wheather_turn(_Coordinate pos, int i);  //针对pos，第i个移动是否可以有翻转，有的话就翻转，
	void   reset_location(int type); //重置feasible location of W &B
	
	int Is_feasible2(int x, int y, int i);
	void Is_feasible(int x, int y, int i); //测试位置（i，j）针对第i个运动，是否是有效的位置，若有效，返回对应的type，不然返回0
	void Is_feasible(int x, int y, int i,bool both); //测试位置（i，j）针对第i个运动，是否是有效的位置，若有效，返回对应的type，不然返回0
	bool checkTerminal();   //检查是否已经结束了
	bool check_feasible_location(int player);//检查现在的player是否有子可下
	set<_Coordinate> get_loaction(int playerType);
	_Coordinate getPostion(set<_Coordinate> location);   //随机产生一个position
	_Coordinate getPostion(int i);
	int Calculate_divergence(_Coordinate pos, int type);    //计算散度
	int get_divergence(int x, int y);
	void BackupBoard();
	void RestoreBoard();
	void Initail();
	void BlackAIPlay();
	void WhiteAIPlay();
};

