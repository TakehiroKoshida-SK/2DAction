/*
ゲーム画面の処理を行うGameModuleクラスのヘッダ
*/


#ifndef D2_GAMEMODULE_H_
#define D2_GAMEMODULE_H_


#include "module.h"
#include "control.h"
#include "unit.h"
#include "map.h"


struct effect {
	int type;
	int flame;
	int x, y;
	int bectle;
	bool stop;
	int subX, subY;
	int subNum;
};

struct dmeffect {
	int value;
	int x, y;
	int flame;

};

struct item {
	int num;
	int x, y;
};

class GameModule :public Module {
private:
	Unit units[4];
	Control controls[4];
	Map map;
	int mode;
	int playerNum;
	int nextKey[10];
	std::vector<attack> attackTask;
	std::vector<effect> effectList;
	std::vector<dmeffect>  dmeffectList;
	int countdown;
	int sokomade;
	int count;
	int deadNum;
	int runk[4];
	std::vector<item> items;
	int damageFont, animeFont;

public:
	virtual ~GameModule() { ; }
	GameModule(int * key);
	virtual int *Run();
	void Draw();
	side  WariateSide(int num);
	point Around(point aPoint, int aBectle);
	void AtackCur();
	void PlayEffect();
	void addDmeffect(int value, int x, int y);
	void DrawDmeffect();
	void DrawStates(int num, int x, int y);
	std::vector<attack> TosshinCur(std::vector<attack> baseList, int unitNum);
	attack SettingAttack(attack aAtack, int num);
	void itemCur();
	void drawItems();

	/*if (aAtack.tosshin > 0) {
										if (aAtack.mainTosshin) aAtack.master = i;
										while (aAtack.tosshin > 0) {
											int on = map.mapData[aAtack.x][aAtack.y].on;
											if ((on != -1 && units[on].aSide != aAtack.aSide )|| (aAtack.mainTosshin && (map.mapData[aAtack.x][aAtack.y].aArea == A_WALL || on!=-1))) {
												break;
											}
											aAtack.x += urdlX[units[i].bectle];
											aAtack.y += urdlY[units[i].bectle];
											aAtack.lagFlame += 10 % units[i].spd;
											if (aAtack.mainTosshin) units[i].kouchoku += 10 % units[i].spd;
											aAtack.tosshin--;
										}
										if (aAtack.mainTosshin) {
											units[i].x = aAtack.x - urdlX[units[i].bectle];
											units[i].y = aAtack.y - urdlY[units[i].bectle];
										}
									}*/
};


#endif