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
*C++�ͷ�������Ϣͨ����
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
	bool SendToUI(int b[8][8]);				//�������壬�����º����ִ��ݸ�UI����
	bool SendToUI(int x,int y, int b[8][8]);    //AI���壬�ȴ������Ǹ��ӣ��ٴ��ݸ��º�����
	_Coordinate GetFromUI();               //����������һ����
	void CheckRead(int type);				//���ui�Ƿ��Ѿ��Ķ������ݣ���Ȼһֱ�ȵ�
	void UpdateBoard(int b[8][8]);				//����board����ֵ
	void reopen(int type, FILE *fp);			//�������´��ļ�����Ϊ�ڴ򿪹����У��ļ��п��ܱ�����
	void InitailizeJson();                     //��ʼ���ɺϷ���2��json�ļ�
};

