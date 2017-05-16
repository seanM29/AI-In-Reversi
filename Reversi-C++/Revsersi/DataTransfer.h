#pragma once
#include<iostream>
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "filewritestream.h"
#include "filereadstream.h"
#include <cstdio>
#include <ctime> 
#include<windows.h>
#include"Common.h"
using namespace rapidjson;
using namespace std;


/*
*C++和服务器信息通信类
*
*/
class DataTransfer
{
private:
	int type = 0;
	int ScoreB=3;
	int ScoreW =3;
	int x = 1;
	int y = 1;
	int transfer_board[8][8];
	int TimeWait = 5;						
public:
	DataTransfer();
	~DataTransfer();
	bool SendToUI(int b[8][8]);				//人类下棋，将更新后的棋局传递给UI绘制
	bool SendToUI(int x,int y, int b[8][8]);    //AI下棋，先传递下那个子，再传递更新后的棋局
	_Coordinate GetFromUI();               //人类下了那一步棋
	void CheckRead(int type);				//检查ui是否已经阅读了数据，不然一直等等
	void UpdateBoard(int b[8][8]);				//更新board来传值
	void reopen(int type, FILE *fp);			//不断重新打开文件，因为在打开过程中，文件有可能被更新
	void InitailizeJson();                     //初始生成合法的2个json文件
};

