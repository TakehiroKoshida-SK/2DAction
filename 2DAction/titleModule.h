/*
タイトル画面の処理をするTitleModuleクラスのヘッダ
*/

#ifndef D2_TITLEMODULE_H_
#define D2_TITLEMODULE_H_

#include <string>
#include "module.h"
#include "control.h"
#include "unit.h"



class TitleModule :public Module{
private:
	int titleBackGraph;
	Control aControl;
	int choiceMode;
	int nextKey[10];
	Unit units[6];
	int move[6];
	int flame;
public:
	virtual ~TitleModule() { ; }
	TitleModule();
	TitleModule(int *key);
	virtual int* Run();
	void Draw();


};

#endif