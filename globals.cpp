#include <string>
#include <list>
#include "DxLib.h"
#include "defines.h"
#include "externs.h"
#include "MyClass.h"


Pixel LED[RES_LED_X][RES_LED_Y];


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
std::list<Movie>::iterator itrMovieList;

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
int PlayFrame = 0;     //再生位置(フレーム)
int TotalFrames;       //再生中の動画の総フレーム数
int TotalTimeSeconds;  //再生中の動画の総再生時間(秒)
int TotalTime_min;     //再生中の動画の総再生時間の分部分
int TotalTime_sec;     //再生中の動画の総再生時間の秒部分


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
bool PushFlag_Key_F1 = FALSE;
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
//bool PushFlag_Key_P = FALSE;

bool PushFlag_Key_Enter = FALSE;
bool PushFlag_Key_Esc = FALSE;
bool PushFlag_Key_Delete = FALSE;



bool FullscreenFlag = FALSE;    //全画面表示 入/切
bool StatusHudShowFlag = TRUE;  //ステータス表示 入/切
bool KeyGuideShowFlag = TRUE;   //操作ガイド表示 入/切
bool MonochromizeFlag = FALSE;  //モノクロ表示 入/切
bool NegativizeFlag = FALSE;    //ネガ表示 入/切
bool RepeatOneFlag = FALSE;     //単体循環 入/切
bool RepeatAllFlag = FALSE;     //全体循環 入/切
bool ShuffleFlag = FALSE;       //乱順再生 入/切
bool PauseFlag = FALSE;         //一時停止 入/切

//よく使いそうな色を変数であらかじめ定義しておく
const unsigned int White = GetColor(255, 255, 255);
const unsigned int Red = GetColor(255, 0, 0);
const unsigned int Blue = GetColor(0, 0, 255);
const unsigned int Green = GetColor(0, 255, 0);
const unsigned int Black = GetColor(0, 0, 0);
const unsigned int Yellow = GetColor(255, 255, 0);
const unsigned int Cyan = GetColor(0, 255, 255);
const unsigned int Mazenta = GetColor(255, 0, 255);
const unsigned int Orange = GetColor(255, 127, 0);
const unsigned int Purple = GetColor(127, 0, 255);
const unsigned int Pink = GetColor(255, 0, 127);
const unsigned int Sky = GetColor(0, 127, 255);

const DWORD fopenFlags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY;
const LPCTSTR fopenFilter = L"すべてのファイル(*.*)|*.*|"
	L"対応している形式の動画ファイル(*.mp4; *.m4v; *.mkv; *.webm; *.flv; *.mpg; *.avi)|*.mp4;*.m4v;*.mkv;*.webm;*.flv;*.mpg;*.avi|"
	L"MP4ファイル(*.mp4)|*.mp4|"
	L"M4Vファイル(*.m4v)|*.m4v|"
	L"MKVファイル(*.mkv)|*.mkv|"
	L"WEBMファイル(*.webm)|*.webm|"
	L"FLVファイル(*.flv)|*.flv|"
	L"MPEGファイル(*.mpg)| *.mpg|"
	L"AVIファイル(*.avi)|*.avi";


std::wstring KeyGuideForScreening = 
L"[F1].....操作説明表示<入|切>"
L"\n[F3].....状態表示<入|切>"
L"\n[F11]....全画面表示<入|切>"
L"\n[W]......色深度+"
L"\n[S]......色深度-"
L"\n[A]......ガンマ補正+"
L"\n[D]......ガンマ補正-"
L"\n[Space]..再生/一時停止"
L"\n[←].....5秒戻る"
L"\n[→].....5秒進む"
L"\n[Z]......前の動画へ"
L"\n[C]......次の動画へ"
L"\n[B]......この動画の先頭へ"
L"\n[↑].....音量+"
L"\n[↓].....音量-"
L"\n[R]......単体循環<入|切>"
L"\n[T]......全体循環<入|切>"
L"\n[F]......乱順再生<入|切>"
L"\n[M]......単色表示<入|切>"
L"\n[N]......反色表示<入|切>"
L"\n[O]......ファイルを再生待機列に追加する"
L"\n[Esc]....上映を終了する";

std::wstring KeyGuideForInitial = 
L"[O]........ファイルを再生待機列に追加する"
L"\n[Delete]...再生待機列を破棄する"
L"\n[Enter]....上映を開始する"
L"\n[F11]......全画面表示<入|切>"
L"\n[Esc]......プログラムを終了する";
