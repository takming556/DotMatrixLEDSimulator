#include <string>
#include <vector>
#include "DxLib.h"
#include "consts.h"
#include "externs.h"
#include "MyClass.h"

using std::wstring;
using std::vector;

Pixel LED[RES_LED_X][RES_LED_Y];


//�O���[�o���ϐ�����
//�n���h��
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
int R_Tones = ColorDepthCustoms[ColorDepthCustomID][0];//�ԐF�̊K����
int G_Tones = ColorDepthCustoms[ColorDepthCustomID][1];//�ΐF�̊K����
int B_Tones = ColorDepthCustoms[ColorDepthCustomID][2];//�F�̊K����
int ColorTones = R_Tones * G_Tones * B_Tones; //�\���\�ȐF�̐�

double Gamma = 0.55;            //�K���}�l
double GammaAdjustStep = 0.05; //�K���}�l���߂̍ŏ��P��

int SoundVolume = 125;

int PlayTimeMilliseconds = 0;    //�Đ��ʒu(�~���b)
int PlayFrame = 0;               //�Đ��ʒu(�t���[��)
int TotalFrames = 0;             //�Đ����̓���̑��t���[����
int TotalSeconds = 0;            //�Đ����̓���̑��Đ�����(�b)
//int TotalTime_min = 0;     //�Đ����̓���̑��Đ����Ԃ̕�����
//int TotalTime_sec = 0;     //�Đ����̓���̑��Đ����Ԃ̕b����


int SourceWidth;
int SourceHeight;
unsigned int DotColor;
int Dot_r;
int Dot_g;
int Dot_b;
int FrameCounter;
int Timer = GetNowCount();
int FPS = 0;



//�t���O
//�L�[�{�[�h�����t���O
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



bool FullscreenFlag = FALSE;    //�S��ʕ\�� ��/��
bool StatusHudShowFlag = TRUE;  //�X�e�[�^�X�\�� ��/��
bool KeyGuideShowFlag = TRUE;   //����K�C�h�\�� ��/��
bool PlayQueueShowFlag = TRUE;  //�Đ��ҋ@��\�� ��/��
bool MonochromizeFlag = FALSE;  //���m�N���\�� ��/��
bool NegativizeFlag = FALSE;    //�l�K�\�� ��/��
bool RepeatOneFlag = FALSE;     //�P�̏z�� ��/��
bool RepeatAllFlag = FALSE;     //�S�̏z�� ��/��
bool ShuffleFlag = FALSE;       //�����Đ� ��/��
bool PauseFlag = FALSE;         //�ꎞ��~ ��/��