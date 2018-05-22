/*
操作キャラクターとなるユニットに関するUnitクラスのヘッダ
*/
#ifndef D2_UNIT_H_
#define D2_UNIT_H_

#include "define.h"
#include "math.h"
#include "unit.h"



struct attack {
	side aSide;
	double power;
	int lagFlame;
	int x;
	int y;
	int range;
	int nextX;
	int nextY;
	int moveLag;
	int tosshin;
	tosshinType aType;
	int nowtosshin;
};



class Unit {
public:
	std::string name;
	bool live;
	int myNumber;
	side aSide;
	int graph;
	int asimoto;
	int atk;
	int def;
	int spd;
	double hitPoint;
	double maxHp;
	bool gard;
	int piyopiyo;
	int kouchoku;
	int idoukouchoku;
	double hirumi;
	double hirumikaihuku, prehirumi;
	int muteki;
	int bectle;
	std::vector<attack> attackList;
	std::vector<attack> chargeList;
	int x, y;
	int charge;
	int count;
	int dethCount;
	point  enemyPoint;
	int tossin;
	int stateUp[3];
public:
	Unit() { ; }
	Unit(int num, side aSide, int x, int y);
	void DrawUnit(int x, int y, int num);
	void DrawUnit(int x, int y, int num, double rate);
	void DrawAsimoto(int x, int y, int num);
	void DrawAsimoto(int x, int y, int num, double rate);
	void DrawMapUnit(int aX, int aY);
	int Movable();
	void FlameTask();
	void MemoryEnemyPoint(point aPoint);
	int GetChargeTime();

};

#endif