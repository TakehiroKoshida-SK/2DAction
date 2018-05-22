#ifndef D2_DEFINE_H_
#define D2_DEFINE_H_

#include "DxLib.h"
#include <vector>
#include <string>
struct point {
	int x;
	int y;
};
struct point_double {
	double x;
	double y;
};



//表示するブロック数やブロックの数、最大マップサイズなど
const int BLOCKSIZE = 32;
const int MAPWIDTH = 700;
const int MAXBLOCK = 200;
const int MINIMAP_X = 600;
const int MINIMAP_Y = 0;

const double PI = 3.1415926535;
const double PI_2 = 1.5707963267;

const int urdlX[4] = { 0,1,0,-1 };
const int urdlY[4] = { -1,0,1,0 };


// define for colors
const int WHITE = GetColor(255, 255, 255);	// white
const int LITEGRAY = GetColor(190, 190, 190); //litegray
const int GRAY = GetColor(125, 125, 125);	// gray
const int DARKGRAY = GetColor(65, 65, 65); //darkgray
const int BLACK = GetColor(0, 0, 0);		// bluck
const int RED = GetColor(255, 0, 0);		// red
const int BLUE = GetColor(0, 0, 255);		// blue
const int GREEN = GetColor(0, 255, 0);		// green
const int YELLOW = GetColor(255, 255, 0);
const int ORANGE = GetColor(255, 64, 0);
const int BRIGHTORANGE = GetColor(255, 128, 0);
const int PERPLE = GetColor(255, 0, 255);


enum area { A_FLAT = 0, A_WALL = 1, A_RED = 2,A_BLUE = 3};
enum side { S_RED,S_BLUE,S_YELLOW,S_GREEN};
enum key {
	K_UP = 0, K_RIGHT = 1, K_DOWN = 2, K_LEFT = 3,
	K_A = 4, K_B = 5, K_X = 6, K_Y = 7,
	K_L = 8, K_R = 9, K_SELECT = 10, K_START = 11
};

enum tosshinType { T_MAIN, T_SUB, T_SUBSUB };

#endif
