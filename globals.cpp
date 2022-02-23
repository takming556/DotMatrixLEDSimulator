#include <string>
#include <list>
#include "DxLib.h"
#include "defines.h"
#include "externs.h"
#include "MyClass.h"


Pixel LED[RES_LED_X][RES_LED_Y];


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
int R_Tones = ColorDepthCustoms[ColorDepthCustomID][0];//�ԐF�̊K����
int G_Tones = ColorDepthCustoms[ColorDepthCustomID][1];//�ΐF�̊K����
int B_Tones = ColorDepthCustoms[ColorDepthCustomID][2];//�F�̊K����
int ColorTones = R_Tones * G_Tones * B_Tones; //�\���\�ȐF�̐�

double Gamma = 0.55;            //�K���}�l
double GammaAdjustStep = 0.05; //�K���}�l���߂̍ŏ��P��

int SoundVolume = 125;

int PlayTimeMilliseconds = 0;    //�Đ��ʒu(�~���b)
int PlayFrame = 0;     //�Đ��ʒu(�t���[��)
int TotalFrames;       //�Đ����̓���̑��t���[����
int TotalTimeSeconds;  //�Đ����̓���̑��Đ�����(�b)
int TotalTime_min;     //�Đ����̓���̑��Đ����Ԃ̕�����
int TotalTime_sec;     //�Đ����̓���̑��Đ����Ԃ̕b����


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



bool FullscreenFlag = FALSE;    //�S��ʕ\�� ��/��
bool StatusHudShowFlag = TRUE;  //�X�e�[�^�X�\�� ��/��
bool KeyGuideShowFlag = TRUE;   //����K�C�h�\�� ��/��
bool MonochromizeFlag = FALSE;  //���m�N���\�� ��/��
bool NegativizeFlag = FALSE;    //�l�K�\�� ��/��
bool RepeatOneFlag = FALSE;     //�P�̏z�� ��/��
bool RepeatAllFlag = FALSE;     //�S�̏z�� ��/��
bool ShuffleFlag = FALSE;       //�����Đ� ��/��
bool PauseFlag = FALSE;         //�ꎞ��~ ��/��

//�悭�g�������ȐF��ϐ��ł��炩���ߒ�`���Ă���
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
const LPCTSTR fopenFilter = L"���ׂẴt�@�C��(*.*)|*.*|"
	L"�Ή����Ă���`���̓���t�@�C��(*.mp4; *.m4v; *.mkv; *.webm; *.flv; *.mpg; *.avi)|*.mp4;*.m4v;*.mkv;*.webm;*.flv;*.mpg;*.avi|"
	L"MP4�t�@�C��(*.mp4)|*.mp4|"
	L"M4V�t�@�C��(*.m4v)|*.m4v|"
	L"MKV�t�@�C��(*.mkv)|*.mkv|"
	L"WEBM�t�@�C��(*.webm)|*.webm|"
	L"FLV�t�@�C��(*.flv)|*.flv|"
	L"MPEG�t�@�C��(*.mpg)| *.mpg|"
	L"AVI�t�@�C��(*.avi)|*.avi";


std::wstring KeyGuideForScreening = 
L"[F1].....��������\��<��|��>"
L"\n[F3].....��ԕ\��<��|��>"
L"\n[F11]....�S��ʕ\��<��|��>"
L"\n[W]......�F�[�x+"
L"\n[S]......�F�[�x-"
L"\n[A]......�K���}�␳+"
L"\n[D]......�K���}�␳-"
L"\n[Space]..�Đ�/�ꎞ��~"
L"\n[��].....5�b�߂�"
L"\n[��].....5�b�i��"
L"\n[Z]......�O�̓����"
L"\n[C]......���̓����"
L"\n[B]......���̓���̐擪��"
L"\n[��].....����+"
L"\n[��].....����-"
L"\n[R]......�P�̏z��<��|��>"
L"\n[T]......�S�̏z��<��|��>"
L"\n[F]......�����Đ�<��|��>"
L"\n[M]......�P�F�\��<��|��>"
L"\n[N]......���F�\��<��|��>"
L"\n[O]......�t�@�C�����Đ��ҋ@��ɒǉ�����"
L"\n[Esc]....��f���I������";

std::wstring KeyGuideForInitial = 
L"[O]........�t�@�C�����Đ��ҋ@��ɒǉ�����"
L"\n[Delete]...�Đ��ҋ@���j������"
L"\n[Enter]....��f���J�n����"
L"\n[F11]......�S��ʕ\��<��|��>"
L"\n[Esc]......�v���O�������I������";
