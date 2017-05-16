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
	void Action();    //��Ϸִ����Ҫ�߼�
	void Update(_Coordinate pos, int type);    //�������̺�����
	void UpdateScore();
	
	int winner();
	void PrintBoard();							//��ӡ����
	bool wheather_turn(_Coordinate pos, int i);  //���pos����i���ƶ��Ƿ�����з�ת���еĻ��ͷ�ת��
	void   reset_location(int type); //����feasible location of W &B
	
	int Is_feasible2(int x, int y, int i);
	void Is_feasible(int x, int y, int i); //����λ�ã�i��j����Ե�i���˶����Ƿ�����Ч��λ�ã�����Ч�����ض�Ӧ��type����Ȼ����0
	void Is_feasible(int x, int y, int i,bool both); //����λ�ã�i��j����Ե�i���˶����Ƿ�����Ч��λ�ã�����Ч�����ض�Ӧ��type����Ȼ����0
	bool checkTerminal();   //����Ƿ��Ѿ�������
	bool check_feasible_location(int player);//������ڵ�player�Ƿ����ӿ���
	set<_Coordinate> get_loaction(int playerType);
	_Coordinate getPostion(set<_Coordinate> location);   //�������һ��position
	_Coordinate getPostion(int i);
	int Calculate_divergence(_Coordinate pos, int type);    //����ɢ��
	int get_divergence(int x, int y);
	void BackupBoard();
	void RestoreBoard();
	void Initail();
	void BlackAIPlay();
	void WhiteAIPlay();
};

