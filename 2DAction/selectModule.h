/*
キャラ選択画面の処理をするSelectModuleクラスのヘッダ
*/

#ifndef D2_SELECTMODULE_H_
#define D2_SELECTMODULE_H_

#include "module.h"
#include "control.h"
#include "unit.h"
#include "UiLine.h"



class SelectModule :public Module{
private:
	int mode;
	int playerNum;
	int nextKey[10];
	side sides[4];
	Unit selectUnit[4];
	int selectNum[4];
	Unit unitList[6];
	int listFlame[6];
	Control controls[4];
	bool ok[4];
	UiLine lines[4];
	int deside;
	int kanryoFont;
public:
	virtual  ~SelectModule() { ; }
	SelectModule(int *key);
	virtual int* Run();
	void Draw();
};

#endif