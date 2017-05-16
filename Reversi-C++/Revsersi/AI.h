#pragma once
#include"Common.h"
#include<time.h>

#include"Node.h"

class Reversi;
class AI
{
private:
	int Simulation_time =5 ;
	int board[8][8];
	Reversi* r = NULL;
	int PlayerType;
	int AiType = -1;
public:
	AI();
	AI(int type,Reversi * re,int AiType);
	~AI();
	//void setReversi(Reversi & re);   //把reversi信息传进去
	_Coordinate Play();
	string getCurrentID();    //返回当前棋局的ID 
	Node* TreePolicy(Node *root);         //选择哪一个节点
	Node* BestChild(Node *root, double c);		//选择子节点中最好的节点
	
	Node* Expand(Node* tmp);				//扩展gametree
	int DefaultPolicy(Node* v);				//默认搜索策略
	Node* NextNode(Node* tmp);				//随机选择下一个位置
	void BP(Node* v, int winner);			//BP更新
	void BackupBoard();						//保存当前棋局
	void RestoreBoard();					//恢复棋局
	void BestChild_Policy(Node *tmp);          //最后计算每个action的policy 估值函数
	void BestChild_MCTS(Node *root, double c);		//最后选择子节点中最好的节点，加上policy的估值函数
	Node* BestChild_Both(Node *v);					//根据2个值计算出最好的node
	void Calculate_divergence(Node *tmp); //计算黑白棋的散度
	void Calculate_Pos(Node *tmp);     //计算当前落子的位置的reward 金角银边草肚皮
	void setTime(int t);
	//void updateBestchild(Node *v);
};

//计算时间的类，精确到us级别
class MyTimer
{
private:
	LARGE_INTEGER _freq;
	LARGE_INTEGER _start;
	LARGE_INTEGER _stop;
public:

	MyTimer()
	{
		QueryPerformanceFrequency(&_freq);
	}

	inline void start()
	{
		QueryPerformanceCounter(&_start);
	}

	inline void stop()
	{
		QueryPerformanceCounter(&_stop);
	}

	inline double elapse()
	{
		return 1000000 * (_stop.QuadPart - _start.QuadPart) / _freq.QuadPart;
	}

	inline long long ticks()
	{
		return _stop.QuadPart - _start.QuadPart;
	}
};
