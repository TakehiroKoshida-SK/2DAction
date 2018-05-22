/*
リザルト画面の処理を行うResultModuleクラスのヘッダ
*/

#ifndef D2_RESULTMODULE_H_
#define D2_RESULTMODULE_H_

#include "module.h"
#include "control.h"
#include "unit.h"


class ResultModule :public Module {
private:
	int mode;
	int playerNum;
	int nextKey[10];
	Control controls[4];
	Unit units[4];
	int runk[4];

public:
	virtual  ~ResultModule() { ; }
	ResultModule(int *key);
	virtual int* Run();
	void Draw();
	side  WariateSide(int num);
};

#endif 
