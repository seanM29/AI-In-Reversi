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
	//void setReversi(Reversi & re);   //��reversi��Ϣ����ȥ
	_Coordinate Play();
	string getCurrentID();    //���ص�ǰ��ֵ�ID 
	Node* TreePolicy(Node *root);         //ѡ����һ���ڵ�
	Node* BestChild(Node *root, double c);		//ѡ���ӽڵ�����õĽڵ�
	
	Node* Expand(Node* tmp);				//��չgametree
	int DefaultPolicy(Node* v);				//Ĭ����������
	Node* NextNode(Node* tmp);				//���ѡ����һ��λ��
	void BP(Node* v, int winner);			//BP����
	void BackupBoard();						//���浱ǰ���
	void RestoreBoard();					//�ָ����
	void BestChild_Policy(Node *tmp);          //������ÿ��action��policy ��ֵ����
	void BestChild_MCTS(Node *root, double c);		//���ѡ���ӽڵ�����õĽڵ㣬����policy�Ĺ�ֵ����
	Node* BestChild_Both(Node *v);					//����2��ֵ�������õ�node
	void Calculate_divergence(Node *tmp); //����ڰ����ɢ��
	void Calculate_Pos(Node *tmp);     //���㵱ǰ���ӵ�λ�õ�reward ������߲ݶ�Ƥ
	void setTime(int t);
	//void updateBestchild(Node *v);
};

//����ʱ����࣬��ȷ��us����
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
