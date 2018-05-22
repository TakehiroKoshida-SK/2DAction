/*
マップに関するMapクラスのヘッダ
*/

#ifndef D2_MAP_H_
#define D2_MAP_H_

#include "DxLib.h"
#include "define.h"
#include <string>

const double bariaHp = 150;

struct mData {
	area aArea;
	int on;
	double Hp;
	side aSide;
};
class Map {
public:
	mData mapData[20][13];
	int count;
	point redPoint;
	point bluePoint;
	int frame;
public:
	Map() { ; }
	Map(int stage, int mode);
	void DrawMap(int aX, int aY);
	void DrawBaria(int x, int y, int flameColor);
	bool CheckContain(point aPoint);
};
#endif