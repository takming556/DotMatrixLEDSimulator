#include <string>
#include <list>
#include "DxLib.h"
#include "defines.h"
#include "externs.h"
#include "MyClass.h"

//�O���[�o���ϐ�����
//�n���h��
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
int R_Tones = ColorDepthCustoms[ColorDepthCustomID][0];//�ԐF�̊K����
int G_Tones = ColorDepthCustoms[ColorDepthCustomID][1];//�ΐF�̊K����
int B_Tones = ColorDepthCustoms[ColorDepthCustomID][2];//�F�̊K����
int ColorTones = R_Tones * G_Tones * B_Tones; //�\���\�ȐF�̐�

double Gamma = 0.5;            //�K���}�l
double GammaAdjustStep = 0.05; //�K���}�l���߂̍ŏ��P��

int SoundVolume = 125;

int msPlayTime = 0; //�Đ��ʒu(�~���b)
int miPlayTime;     //�Đ����Ԃ̕�����
int scPlayTime;     //�Đ����Ԃ̕b����
int PlayFrame = 0;  //�Đ��ʒu(�t���[��)
int TotalFrame;     //�Đ����̓���̑��t���[����


int SourceWidth;
int SourceHeight;
unsigned int DotColor;
int Dot_r;
int Dot_g;
int Dot_b;
int FrameCounter;
int Timer;
int FPS = 0;



//�t���O
//�L�[�{�[�h�����t���O
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

bool FullscreenFlag = FALSE;    //�S��ʕ\�� ��/��
bool StatusHudShowFlag = TRUE;  //�X�e�[�^�X�\�� ��/��
bool KeyGuideShowFlag = TRUE;   //����K�C�h�\�� ��/��
bool MonochromizeFlag = FALSE;  //���m�N���\�� ��/��
bool NegativizeFlag = FALSE;    //�l�K�\�� ��/��
bool RepeatFlag = FALSE;        //�P�ȏz�� ��/��
bool ShuffleFlag = FALSE;       //�����Đ� ��/��
bool PauseFlag = FALSE;         //�ꎞ��~ ��/��

//�悭�g���F��ϐ��Œ�`
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
L"[W]......�F�[�x+"
L"\n[S]......�F�[�x-"
L"\n[A]......�K���}�␳+"
L"\n[D]......�K���}�␳-"
L"\n[F1].....��������\��<��/��>"
L"\n[F3].....�X�e�[�^�X�\��<��/��>"
L"\n[F11]....�S��ʕ\��<��/��>"
L"\n[��].....5�b�߂�"
L"\n[��].....5�b�i��"
L"\n[Z]......�O�̓����"
L"\n[C]......���̓����"
L"\n[��].....����+"
L"\n[��].....����-"
L"\n[SPACE]..�Đ�/�ꎞ��~"
L"\n[M]......���m�N���\��<��/��>"
L"\n[N]......�l�K�\��<��/��>"
L"\n[O]......�t�@�C�����Đ��ҋ@��ɒǉ�"
L"\n[P]......�t�H���_���Đ��ҋ@��ɒǉ�";

std::wstring KeyGuideForInitial = 
L"[O]......�t�@�C�����J��"
L"\n[P]......�t�H���_���J��";
