#include <string>
#include <list>
#include "DxLib.h"
#include "defines.h"
#include "externs.h"
#include "MyClass.h"

//グローバル変数たち
//ハンドル
int hMovie;
int hTempFrameSoftImage;

//std::wstring FilePath;
//std::wstring FileName;
//std::wstring FolderPath;
std::wstring PlayingFileName;
std::wstring PlayingFilePath;
std::list<Movie> MovieList;
std::list<Movie>::iterator itrPlayPos;

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

double Gamma = 0.5;            //ガンマ値
double GammaAdjustStep = 0.05; //ガンマ値調節の最小単位

int SoundVolume = 125;

int msPlayTime = 0; //再生位置(ミリ秒)
int miPlayTime;     //再生時間の分部分
int scPlayTime;     //再生時間の秒部分
int PlayFrame = 0;  //再生位置(フレーム)
int TotalFrame;     //再生中の動画の総フレーム数


int SourceWidth;
int SourceHeight;
unsigned int DotColor;
int Dot_r;
int Dot_g;
int Dot_b;
int FrameCounter;
int Timer;
int FPS = 0;



//フラグ
//キーボード押下フラグ
bool isPushedKey_Esc = FALSE;

bool isPushedKey_W = FALSE;
bool isPushedKey_A = FALSE;
bool isPushedKey_S = FALSE;
bool isPushedKey_D = FALSE;
bool isPushedKey_M = FALSE;
bool isPushedKey_N = FALSE;
bool isPushedKey_R = FALSE;
bool isPushedKey_F = FALSE;


bool isPushedKey_O = FALSE;
bool isPushedKey_P = FALSE;

bool isPushedKey_Up = FALSE;
bool isPushedKey_Down = FALSE;
bool isPushedKey_Left = FALSE;
bool isPushedKey_Right = FALSE;

bool isPushedKey_Space = FALSE;
bool isPushedKey_Z = FALSE;
bool isPushedKey_C = FALSE;

bool isPushedKey_F1 = FALSE;
bool isPushedKey_F3 = FALSE;
bool isPushedKey_F11 = FALSE;

bool FullscreenFlag = FALSE;    //全画面表示 入/切
bool StatusHudShowFlag = TRUE;  //ステータス表示 入/切
bool KeyGuideShowFlag = TRUE;   //操作ガイド表示 入/切
bool MonochromizeFlag = FALSE;  //モノクロ表示 入/切
bool NegativizeFlag = FALSE;    //ネガ表示 入/切
bool RepeatFlag = FALSE;        //単曲循環 入/切
bool ShuffleFlag = FALSE;       //乱順再生 入/切
bool PauseFlag = FALSE;         //一時停止 入/切

//よく使う色を変数で定義
unsigned int White = GetColor(255, 255, 255);
unsigned int Red = GetColor(255, 255, 0);
unsigned int Blue = GetColor(0, 0, 255);
unsigned int Green = GetColor(0, 255, 0);
unsigned int Black = GetColor(0, 0, 0);
unsigned int Yellow = GetColor(255, 255, 0);
unsigned int Cyan = GetColor(0, 255, 255);
unsigned int Mazenta = GetColor(255, 0, 255);
unsigned int Orange = GetColor(255, 127, 0);
unsigned int Purple = GetColor(127, 0, 255);
unsigned int Pink = GetColor(255, 0, 127);
unsigned int Sky = GetColor(0, 127, 255);

std::wstring KeyGuideForScreening = 
L"[W]......色深度+"
L"\n[S]......色深度-"
L"\n[A]......ガンマ補正+"
L"\n[D]......ガンマ補正-"
L"\n[F1].....操作説明表示<入/切>"
L"\n[F3].....ステータス表示<入/切>"
L"\n[F11]....全画面表示<入/切>"
L"\n[←].....5秒戻る"
L"\n[→].....5秒進む"
L"\n[Z]......前の動画へ"
L"\n[C]......次の動画へ"
L"\n[↑].....音量+"
L"\n[↓].....音量-"
L"\n[SPACE]..再生/一時停止"
L"\n[M]......モノクロ表示<入/切>"
L"\n[N]......ネガ表示<入/切>"
L"\n[O]......ファイルを再生待機列に追加"
L"\n[P]......フォルダを再生待機列に追加";

std::wstring KeyGuideForInitial = 
L"[O]......ファイルを開く"
L"\n[P]......フォルダを開く";
