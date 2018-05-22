/*
操作キャラクターとなるユニットに関するUnitクラスの実装
*/

#include "unit.h"

#include "DxLib.h"
#include "define.h"
#include "math.h"
#include "unit.h"


/*
コンストラクタ
ユニットのデータは外部ファイルから取りたかったが面倒なのでここで直接打ち込む
*/
Unit::Unit(int num, side aSide, int x, int y) {
	count = 0;
	live = true;
	this->aSide = aSide;
	myNumber = num;
	switch (aSide) {
	case S_RED: asimoto = LoadGraph("graph/other/asimoto_red.png"); break;
	case S_BLUE: asimoto = LoadGraph("graph/other/asimoto_blue.png"); break;
	case S_GREEN: asimoto = LoadGraph("graph/other/asimoto_green.png"); break;
	case S_YELLOW: asimoto = LoadGraph("graph/other/asimoto_yellow.png"); break;

	}
	bectle = 0;
	gard = false;
	piyopiyo = 0;
	kouchoku = 0;
	idoukouchoku = 0;
	hirumi = prehirumi = hirumikaihuku = 0;
	muteki = 0;
	charge = 0;
	dethCount = 0;
	for (int i = 0; i < 3; i++) stateUp[i] = 0;
	enemyPoint.x = -1;
	enemyPoint.y = -1;
	tossin = 0;
	switch (num) {
	case 0: {
		name = "勇者";
		graph = LoadGraph("graph/unit/Ales.png");
		atk = 3;
		def = 4;
		spd = 3;

		attack aAttack = { aSide,7,0,0,1,1 };	attackList.push_back(aAttack);
		attack bAttack = { aSide,7,0,0,2,1 };	attackList.push_back(bAttack);

		attack cAttack = { aSide, 20,25,0,3,1 };	chargeList.push_back(cAttack);
		for (int i = 0; i < 4; i++) {
			attack cAttack = { aSide, 15, 25, urdlX[i] , urdlY[i] + 3,1 };	chargeList.push_back(cAttack);
			attack dAttack = { aSide, 10, 25, urdlX[(i * 2 + 1) % 4] , urdlY[((i / 2) * 2) % 4] + 3,1 };	chargeList.push_back(dAttack);
		}
		

	}break;
	case 1: {
		name = "剣士";
		graph = LoadGraph("graph/unit/fencer.png");
		atk = 3;
		def = 3;
		spd = 4;

		attack aAttack = { aSide,10,0,0,1,1 };	attackList.push_back(aAttack);

		attack bAttack = { aSide,18,8,0,1,1,0,0,0,3,T_MAIN };	chargeList.push_back(bAttack);
		attack cAttack = { aSide,15,5,-1,1,1,0,0,0,3,T_SUB };	chargeList.push_back(cAttack);
		attack dAttack = { aSide,15,11,1,1,1 ,0,0,0,3,T_SUB };	chargeList.push_back(dAttack);


	}break;
	case 2: {
		name = "戦士";
		graph = LoadGraph("graph/unit/fighter.png");
		atk = 5;
		def = 3;
		spd = 2;

		attack aAttack = { aSide,7,0,0,1,1 };	attackList.push_back(aAttack);
		attack bAttack = { aSide ,5,0,-1,1,1 };	 attackList.push_back(bAttack);
		attack cAttack = { aSide ,5,0,1,1,1 };	attackList.push_back(cAttack);

		attack dAttack = { aSide,20,0,0,1,1 };	chargeList.push_back(dAttack);
		attack eAttack = { aSide ,15,20,-1,2,1 };	chargeList.push_back(eAttack);
		attack fAttack = { aSide ,15,20,1,2,1 };	chargeList.push_back(fAttack);
		attack gAttack = { aSide ,15,20,0,2,1 };	chargeList.push_back(gAttack);
		attack hAttack = { aSide ,15,40,-2,3,1 };	chargeList.push_back(hAttack);
		attack iAttack = { aSide ,15,40,2,3,1 };	chargeList.push_back(iAttack);
		attack jAttack = { aSide ,15,40,0,3,1 };	chargeList.push_back(jAttack);


	}break;
	case 3: {
		name = "騎士";
		graph = LoadGraph("graph/unit/knight.png");
		atk = 4;
		def = 4;
		spd = 2;

		attack aAttack = { aSide,7,0,0,1,1 };	attackList.push_back(aAttack);
		attack bAttack = { aSide ,7,0,0,2,1 };	 attackList.push_back(bAttack);

		attack cAttack = { aSide,15,5,0,1,1,0,0,0,6,T_MAIN };	chargeList.push_back(cAttack);
		attack dAttack = { aSide,18,5,0,2,1,0,0,0,6,T_SUB };	chargeList.push_back(dAttack);
		attack eAttack = { aSide,15,5,0,3,1,0,0,0,6,T_SUBSUB };	chargeList.push_back(eAttack);



	}break;
	case 4: {
		name = "弓兵";
		graph = LoadGraph("graph/unit/archer.png");
		atk = 4;
		def = 2;
		spd = 4;

		attack aAttack = { aSide,8,3,0,2,1 };
		attackList.push_back(aAttack);
		attack bAttack = { aSide ,8,3,0,3,1 };
		attackList.push_back(bAttack);
		attack cAttack{ aSide,20,0,0,1,10,0,1,5 };
		chargeList.push_back(cAttack);
	}break;

	case 5: {
		name = "ペガオ";
		graph = LoadGraph("graph/unit/pegao.png");
		atk = 3;
		def = 2;
		spd = 5;

		attack aAttack = { aSide,6,0,0,1,1 };	attackList.push_back(aAttack);
		attack bAttack = { aSide,5,10,0,1,1 };	attackList.push_back(bAttack);

		attack cAttack = { aSide,7,0,0,1,1 };	chargeList.push_back(cAttack);
		attack dAttack = { aSide,7,0,0,1,5,0,1,10 };	chargeList.push_back(dAttack);
		attack eAttack = { aSide,7,5,0,1,5,0,1,10 };	chargeList.push_back(eAttack);
		attack fAttack = { aSide,7,10,0,1,5,0,1,10 };	chargeList.push_back(fAttack);



	}break;
	}
	hitPoint = maxHp = 100;
	this->x = x;
	this->y = y;
	int zureX = 10 - x;
	int zureY = 7 - y;
	if (abs(zureX) > abs(zureY)) {
		if (zureX > 0) bectle = 1;
		else bectle = 3;
	}
	else {
		if (zureY > 0) bectle = 2;
		else bectle = 0;
	}
}

/*
ユニットそのものの描画
*/
void Unit::DrawUnit(int x, int y, int num) {
	int tmpGraph;
	num %= 2;
	tmpGraph = DerivationGraph(num * 32, 0, 32, 32, graph);
	DrawRotaGraph(x, y, 2.5, 0, tmpGraph, true);
	DeleteGraph(tmpGraph);
}
/*
上の大きさ調整版
*/
void Unit::DrawUnit(int x, int y, int num, double rate) {
	int tmpGraph;
	num %= 2;
	tmpGraph = DerivationGraph(num * 32, 0, 32, 32, graph);
	DrawRotaGraph(x, y, rate, 0, tmpGraph, true);
	DeleteGraph(tmpGraph);
}

/*
ユニットの足元（背景？）のパネルの描画
*/
void Unit::DrawAsimoto(int x, int y, int num) {
	int tmpGraph;
	num %= 16;
	tmpGraph = DerivationGraph(num * 32, 0, 32, 32, asimoto);
	DrawRotaGraph(x, y, 2.6, 0, tmpGraph, true);
	DeleteGraph(tmpGraph);
}
/*
上の大きさ調整版
*/
void Unit::DrawAsimoto(int x, int y, int num, double rate) {
	int tmpGraph;
	num %= 16;
	tmpGraph = DerivationGraph(num * 32, 0, 32, 32, asimoto);
	DrawRotaGraph(x, y, rate, 0, tmpGraph, true);
	DeleteGraph(tmpGraph);
}

/*
マップにおけるユニットの描画
ユニットの向きやチャージ状態等のエフェクトも含む
*/
void Unit::DrawMapUnit(int aX, int aY) {
	int tmpGraph;
	int tmpX = aX + x * 40 - 20;
	int tmpY = aY + y * 40 - 20;

	int num = (count / 5) % 16;
	tmpGraph = DerivationGraph(num * 32, 0, 32, 32, asimoto);
	if (live)DrawRotaGraph(tmpX, tmpY, 1.3, bectle * PI_2, tmpGraph, true);
	DeleteGraph(tmpGraph);

	if (hirumi == 0 && kouchoku == 0 && !gard && piyopiyo == 0) tmpGraph = DerivationGraph(0, 0, 32, 32, graph);
	else tmpGraph = DerivationGraph(32, 0, 32, 32, graph);
	if (live) {
		if (muteki > 0) 		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (muteki / 5 % 2) * 150 + 50);
		DrawRotaGraph(tmpX, tmpY, 1.25, 0, tmpGraph, true);
		if (muteki > 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
	else {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - dethCount);
		DrawRotaGraph(tmpX, tmpY, 1.25, PI_2, tmpGraph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
	//printfDx("%d,%d,%d\t%d,%d,%d\t%d\n",aX,x, tmpX, aY,y,tmpY,graph);
	DeleteGraph(tmpGraph);
	if (live) {
		//チャージの描画
		static int chargeGraph = LoadGraph("graph/other/charge.png");
		static int charge2Graph = LoadGraph("graph/other/charge2.png");
		static int accel = 1;
		static int tmpNum = 5;
		if (charge > 0) {
			if (charge < GetChargeTime()) {
				num = 9 * charge / GetChargeTime() - 3;
				tmpGraph = chargeGraph;
			}

			else {
				if (count % 5 == 0) {
					if (tmpNum >= 8) accel = -1;
					else if (tmpNum <= 5)  accel = 1;
					tmpNum += accel;
				}
				num = tmpNum;
				tmpGraph = charge2Graph;
			}
			tmpGraph = DerivationGraph(num * 240, 0, 240, 240, tmpGraph);
			DrawRotaGraph(tmpX, tmpY, 0.4, 0, tmpGraph, true);
			DeleteGraph(tmpGraph);
		}

		//ピヨピヨの描画
		static int confuGraph = LoadGraph("graph/effect/confu.png");
		if (piyopiyo > 0) {
			num = piyopiyo / 20 % 3;
			tmpGraph = DerivationGraph(num * 82, 0, 82, 73, confuGraph);
			DrawRotaGraph(tmpX, tmpY + 10, 2, 0, tmpGraph, true);
			DeleteGraph(tmpGraph);
		}
		//進行方向の描画
		static int yazirushiHandle = LoadGraph("graph/other/yazirushi.png");
		DrawRotaGraph((x + urdlX[bectle]) * 40 + 20, (y + urdlY[bectle]) * 40 - 20, 0.8, bectle * PI_2, yazirushiHandle, true);

	}
}

/*
移動が可能であるかを出力
*/
int Unit::Movable() {
	if (piyopiyo <= 0 && kouchoku <= 0 && idoukouchoku <= 0 && live) return 2;
	else  if (piyopiyo <= 0 && kouchoku <= 0 && live) return 1;
	else return 0;
}

/*
フレームごとの計算処理
*/
void Unit::FlameTask() {
	count++;
	if (live) {
		if (kouchoku > 0)kouchoku--;
		if (idoukouchoku > 0)idoukouchoku--;
		if (piyopiyo > 0) {
			piyopiyo--;
			if (piyopiyo == 0) {
				if (enemyPoint.x == -1 || enemyPoint.y == -1 || (abs(enemyPoint.x - x) + abs(enemyPoint.y - y) > 3))muteki = 240;
			}
		}
		if (hirumi > 0) {
			if (prehirumi >= hirumi) hirumikaihuku += (def * 2) * 0.000001;
			else hirumikaihuku = 0;
			hirumi -= maxHp * hirumikaihuku;
		}
		prehirumi = hirumi;
		if (muteki > 0) muteki--;
		if (hitPoint < maxHp && hirumi <= 0) hitPoint += maxHp * 0.00;

		if (hirumi >(35)) {
			piyopiyo = 240;
			hirumi = 0;
		}
	}
	else {
		dethCount++;
	}
}

/*
敵陣地の記憶
*/
void Unit::MemoryEnemyPoint(point aPoint) {
	enemyPoint = aPoint;
}

/*
チャージに必要なフレーム数を返す
*/
int Unit::GetChargeTime() {
	return int(60 * pow(1.2, 3 - spd ));
}
