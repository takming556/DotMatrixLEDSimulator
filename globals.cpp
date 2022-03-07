#include <string>
#include <vector>
#include "DxLib.h"
#include "consts.h"
#include "externs.h"
#include "MyClass.h"

using std::wstring;
using std::vector;

Pixel LED[RES_LED_X][RES_LED_Y];


//グローバル変数たち
//ハンドル
int hMovie;
int hTempFrameSoftImage;

wstring PlayingFileName;
wstring PlayingFilePath;
vector<Movie> MovieTable;
vector<Movie> PlayQueue;
int NowplayingPos = -1;

int Scene = INITIAL;

int ColorDepthCustoms[15][3] = {
	{2,2,2},			//0
	{2,2,4},			//1
	{4,4,4},			//2
	{4,4,8},			//3
	{8,8,8},			//4
	{8,8,16},			//5
	{16,16,16},			//6
	{16,16,32},			//7
	{32,32,32},			//8
	{32,32,64},			//9
	{64,64,64},			//10
	{64,64,128},		//11
	{128,128,128},		//12
	{128,128,256},		//13
	{256,256,256}		//14
};
int ColorDepthCustomID = 6;
int R_Tones = ColorDepthCustoms[ColorDepthCustomID][0];//赤色の階調数
int G_Tones = ColorDepthCustoms[ColorDepthCustomID][1];//緑色の階調数
int B_Tones = ColorDepthCustoms[ColorDepthCustomID][2];//青色の階調数
int ColorTones = R_Tones * G_Tones * B_Tones; //表現可能な色の数

double Gamma = 0.55;            //ガンマ値
double GammaAdjustStep = 0.05; //ガンマ値調節の最小単位

int SoundVolume = 125;

int PlayTimeMilliseconds = 0;    //再生位置(ミリ秒)
int PlayFrame = 0;               //再生位置(フレーム)
int TotalFrames = 0;             //再生中の動画の総フレーム数
int TotalSeconds = 0;            //再生中の動画の総再生時間(秒)
//int TotalTime_min = 0;     //再生中の動画の総再生時間の分部分
//int TotalTime_sec = 0;     //再生中の動画の総再生時間の秒部分


int SourceWidth;
int SourceHeight;
unsigned int DotColor;
int Dot_r;
int Dot_g;
int Dot_b;
int FrameCounter;
int Timer = GetNowCount();
int FPS = 0;



//フラグ
//キーボード押下フラグ
bool PushFlag_Key_F1 = FALSE;
bool PushFlag_Key_F2 = FALSE;
bool PushFlag_Key_F3 = FALSE;
bool PushFlag_Key_F11 = FALSE;

bool PushFlag_Key_W = FALSE;
bool PushFlag_Key_S = FALSE;
bool PushFlag_Key_A = FALSE;
bool PushFlag_Key_D = FALSE;

bool PushFlag_Key_Space = FALSE;

bool PushFlag_Key_Left = FALSE;
bool PushFlag_Key_Right = FALSE;

bool PushFlag_Key_Z = FALSE;
bool PushFlag_Key_C = FALSE;
bool PushFlag_Key_B = FALSE;

bool PushFlag_Key_Up = FALSE;
bool PushFlag_Key_Down = FALSE;

bool PushFlag_Key_R = FALSE;
bool PushFlag_Key_T = FALSE;
bool PushFlag_Key_F = FALSE;
bool PushFlag_Key_M = FALSE;
bool PushFlag_Key_N = FALSE;

bool PushFlag_Key_O = FALSE;

bool PushFlag_Key_Enter = FALSE;
bool PushFlag_Key_Esc = FALSE;
bool PushFlag_Key_Delete = FALSE;



bool FullscreenFlag = FALSE;    //全画面表示 入/切
bool StatusHudShowFlag = TRUE;  //ステータス表示 入/切
bool KeyGuideShowFlag = TRUE;   //操作ガイド表示 入/切
bool PlayQueueShowFlag = TRUE;  //再生待機列表示 入/切
bool MonochromizeFlag = FALSE;  //モノクロ表示 入/切
bool NegativizeFlag = FALSE;    //ネガ表示 入/切
bool RepeatOneFlag = FALSE;     //単体循環 入/切
bool RepeatAllFlag = FALSE;     //全体循環 入/切
bool ShuffleFlag = FALSE;       //乱順再生 入/切
bool PauseFlag = FALSE;         //一時停止 入/切