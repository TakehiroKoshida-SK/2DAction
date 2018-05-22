/*
マップに関するMapクラスの実装
*/

#include "map.h"

#include "DxLib.h"
#include "define.h"
#include <string>

/*
コンストラクタ
引数からステージとモードを選択する
ステージデータはgraph/mapにあるpngファイル。　簡単な画像処理でデータを引っ張り出します。
*/
Map::Map(int stage, int mode) {
	frame = 0;
	redPoint.x = redPoint.y = bluePoint.x = bluePoint.y = -1;

	//以下、画像からマップを作る処理
	//画像を読み込み、一度画面上に表示する
	std::string filename = "graph/map/map" + std::to_string(stage) + ".png";
	int handle = LoadGraph(filename.c_str());
	DrawGraph(0, 0, handle, true);
	//上部1行に各エリア種が何色に当たるのかを示したドットがあるので、その色を読み込む
	int flatColor = GetPixel(0, 0);
	int wallColor = GetPixel(1, 0);
	int redColor = GetPixel(2, 0);
	int blueColor = GetPixel(3, 0);
	int greenColor = GetPixel(4, 0);
	int yellowColor = GetPixel(5, 0);
	int redAreaColor = GetPixel(6, 0);
	int blueAreaColor = GetPixel(7, 0);

	//それ以下のドットはマップを表しているので全て読み込み
	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 13; y++) {
			int aColor = GetPixel(x, y + 1);
			//printfDx("%d,",aColor);
			mapData[x][y].aArea = A_FLAT;
			mapData[x][y].Hp = 0;
			mapData[x][y].on = -1;

			if (aColor == wallColor) { mapData[x][y].aArea = A_WALL; }
			else if (aColor == redColor) { mapData[x][y].on = 0; }
			else if (aColor == blueColor) {
				if (mode == 2) mapData[x][y].on = 2;
				else mapData[x][y].on = 1;
			}
			else if (aColor == greenColor) {
				if (mode == 1)mapData[x][y].on = 2;
				if (mode == 2) mapData[x][y].on = 3;
			}
			else if (aColor == yellowColor) {
				if (mode == 1)mapData[x][y].on = 3;
				if (mode == 2)mapData[x][y].on = 1;
			}
			else if (aColor == redAreaColor) {
				if (mode == 2) {
					mapData[x][y].aArea = A_RED;
					mapData[x][y].aSide = S_RED;
					mapData[x][y].Hp = bariaHp;
					redPoint.x = x;
					redPoint.y = y;
				}
			}
			else if (aColor == blueAreaColor) {

				if (mode == 2) {
					mapData[x][y].aArea = A_BLUE;
					mapData[x][y].aSide = S_BLUE;
					mapData[x][y].Hp = bariaHp;
					bluePoint.x = x;
					bluePoint.y = y;
				}
			}
		}
	}
	count = 0;

}

/*
マップ描画
*/
void Map::DrawMap(int aX, int aY) {
	static int flatHandle = LoadGraph("graph/map/flat.png");
	static int wallHandle = LoadGraph("graph/map/wall.png");
	static int redHandle = LoadGraph("graph/map/red.png");
	static int blueHandle = LoadGraph("graph/map/blue.png");
	static int bariaRHandle = LoadGraph("graph/other/baria_r.png");
	static int bariaBHandle = LoadGraph("graph/other/baria_b.png");
	bool bariaRLive = false;
	bool bariaBLive = false;
	frame++;
	int bariaRX = 0, bariaRY = 0, bariaBX = 0, bariaBY = 0;
	//基本マップの描画
	for (int x = 1; x < 19; x++) {
		for (int y = 1; y < 12; y++) {
			int tmpX = aX + (x - 1) * 40;
			int tmpY = aY + (y - 1) * 40;
			if (mapData[x][y].aArea == A_FLAT) DrawExtendGraph(tmpX, tmpY, tmpX + 40, tmpY + 40, flatHandle, true);
			if (mapData[x][y].aArea == A_WALL) DrawExtendGraph(tmpX, tmpY, tmpX + 40, tmpY + 40, wallHandle, true);
			if (mapData[x][y].aArea == A_RED) {
				DrawExtendGraph(tmpX, tmpY, tmpX + 40, tmpY + 40, redHandle, true);
				bariaRLive = true;
				bariaRX = x;
				bariaRY = y;

			}
			if (mapData[x][y].aArea == A_BLUE) {

				DrawExtendGraph(tmpX, tmpY, tmpX + 40, tmpY + 40, blueHandle, true);
				bariaBLive = true;
				bariaBX = x;
				bariaBY = y;
				/*
				int num = count / 10 % 10;
				int tmpGraph = DerivationGraph(num * 120, 0, 120, 120, bariaRHandle);
				SetDrawBlendMode(DX_BLENDMODE_ADD, mapData[x][y].Hp / 100 * 150 + 50);
				DrawRotaGraph(tmpX+20, tmpY+20, 0.5, 0, tmpGraph, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DeleteGraph(tmpGraph);
				*/
			}
		}
	}
	//陣地の描画
	if (redPoint.x != -1) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		for (int i = -3; i <= 3; i++) {
			for (int j = -3 + abs(i); j <= 3 - abs(i); j++) {
				point rPoint = { redPoint.x + i,redPoint.y + j };
				point bPoint = { bluePoint.x + i,  bluePoint.y + j };
				int tmpX = aX + (rPoint.x - 1) * 40;
				int tmpY = aY + (rPoint.y - 1) * 40;
				if (CheckContain(rPoint) && mapData[rPoint.x][rPoint.y].aArea == A_FLAT) {
					int tmpX = aX + (rPoint.x - 1) * 40;
					int tmpY = aY + (rPoint.y - 1) * 40;
					DrawBox(tmpX, tmpY, tmpX + 40, tmpY + 40, RED, true);
				}
				if (CheckContain(bPoint) && mapData[bPoint.x][bPoint.y].aArea == A_FLAT) {
					int tmpX = aX + (bPoint.x - 1) * 40;
					int tmpY = aY + (bPoint.y - 1) * 40;
					DrawBox(tmpX, tmpY, tmpX + 40, tmpY + 40, BLUE, true);
				}

			}
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//バリアの描画　
	static int goalGraph = LoadGraph("graph/other/goal.png");
	if (bariaRLive) {
		if (mapData[bariaRX][bariaRY].Hp > 0)DrawBaria(bariaRX, bariaRY, RED);
		else {
			DrawRectGraph(aX + bariaRX * 40 - 36, aY + bariaRY * 40 - 60, (frame / 10 % 3) * 32, 0, 32, 32, goalGraph, true, false);
		}
	}
	if (bariaBLive) {
		if (mapData[bariaBX][bariaBY].Hp > 0)DrawBaria(bariaBX, bariaBY, BLUE);
		else {
			DrawRectGraph(aX + bariaBX * 40 - 36, aY + bariaBY * 40 - 60, (frame / 10 % 3) * 32, 0, 32, 32, goalGraph, true, false);
		}
	}
	/*
	int num = count / 10 % 10;
	if (bariaRLive) {
	int tmpGraph = DerivationGraph(num * 120, 0, 120, 120, bariaRHandle);
	int tmpX = aX + (bariaRX - 1) * 40+20;
	int tmpY = aY + (bariaRY - 1) * 40+0;
	SetDrawBlendMode(DX_BLENDMODE_ADD, mapData[bariaRX][bariaRY].Hp / bariaHp * 100 + 100);
	DrawRotaGraph(tmpX, tmpY, 0.8, 0, tmpGraph, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DeleteGraph(tmpGraph);
	}
	if (bariaBLive) {
	int tmpGraph = DerivationGraph(num * 120, 0, 120, 120, bariaBHandle);
	int tmpX = aX + (bariaBX - 1) * 40+20;
	int tmpY = aY + (bariaBY - 1) * 40+0;
	SetDrawBlendMode(DX_BLENDMODE_ADD, mapData[bariaBX][bariaBY].Hp / bariaHp * 100 + 100);
	DrawRotaGraph(tmpX, tmpY, 0.8, 0, tmpGraph, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DeleteGraph(tmpGraph);
	}
	*/

	//カウント
	count++;

}

void Map::DrawBaria(int x, int y, int flameColor) {
	double hp = mapData[x][y].Hp;
	int color = GetColor(int(128 * (1 - hp / bariaHp)), int(255 * (hp / bariaHp)), 0);
	x = x * 40 + 20;
	y = y * 40 - 20;
	int dotX[4] = { x,x - 40,x,x + 40 };
	int dotY[4] = { y - 40 ,y - 20, y + 40, y - 20 };
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	for (int i = 0; i < 3; i++) {
		DrawTriangle(dotX[0], dotY[0], dotX[i + 1], dotY[i + 1], dotX[1 + (i + 1) % 3], dotY[1 + (i + 1) % 3], color, true);
	}
	DrawTriangle(dotX[1], dotY[1], dotX[2], dotY[2], dotX[3], dotY[3], color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	for (int i = 0; i < 4; i++) {
		for (int j = i; j < 4; j++) {
			DrawLine(dotX[i], dotY[i], dotX[j], dotY[j], flameColor);
		}
	}
}
bool Map::CheckContain(point aPoint) {
	if (0 <= aPoint.x && aPoint.x < 20 && 0 <= aPoint.y && aPoint.y < 13)	return true;
	else																	return false;
}
