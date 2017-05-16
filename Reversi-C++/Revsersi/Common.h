#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include <ctime>
#include<set>
#include <math.h> 
#include<windows.h>
#include <stdlib.h>

using namespace std;

typedef pair<int, int> _Coordinate;  //坐标
const int WhitePlayer = 1;
const int BlackPlayer = 2;
const int White = 1;
const int Black = 2;
const int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 }, dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const double C = 1.5;
extern int Board[8][8];
extern int CurrentPlayer;
extern int Count;//回合数



