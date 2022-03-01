#include <vector>
#include "MyClass.h"


//externêÈåæÇΩÇø


extern Pixel LED[RES_LED_X][RES_LED_Y];
extern int hMovie;
extern int hTempFrameSoftImage;

extern std::wstring PlayingFileName;
extern std::wstring PlayingFilePath;
extern std::vector<Movie> MovieTable;
extern std::vector<Movie> PlayQueue;
extern int NowplayingPos;

extern int Scene;

extern int ColorDepthCustoms[15][3];
extern int ColorDepthCustomID;
extern int R_Tones;
extern int G_Tones;
extern int B_Tones;
extern int ColorTones;

extern double Gamma;
extern double GammaAdjustStep;

extern int SoundVolume;

extern int PlayTimeMilliseconds;
extern int PlayFrame;
extern int TotalFrames;
extern int TotalTimeSeconds;
extern int TotalTime_min;
extern int TotalTime_sec;

extern int FPS;

extern int SourceWidth;
extern int SourceHeight;
extern unsigned int DotColor;
extern int Dot_r;
extern int Dot_g;
extern int Dot_b;

extern int FrameCounter;
extern int Timer;


extern bool PushFlag_Key_F1;
extern bool PushFlag_Key_F2;
extern bool PushFlag_Key_F3;
extern bool PushFlag_Key_F11;

extern bool PushFlag_Key_W;
extern bool PushFlag_Key_S;
extern bool PushFlag_Key_A;
extern bool PushFlag_Key_D;

extern bool PushFlag_Key_Space;

extern bool PushFlag_Key_Left;
extern bool PushFlag_Key_Right;

extern bool PushFlag_Key_Z;
extern bool PushFlag_Key_C;
extern bool PushFlag_Key_B;

extern bool PushFlag_Key_Up;
extern bool PushFlag_Key_Down;

extern bool PushFlag_Key_R;
extern bool PushFlag_Key_T;
extern bool PushFlag_Key_F;
extern bool PushFlag_Key_M;
extern bool PushFlag_Key_N;

extern bool PushFlag_Key_O;

extern bool PushFlag_Key_Enter;
extern bool PushFlag_Key_Esc;
extern bool PushFlag_Key_Delete;


extern bool FullscreenFlag;
extern bool StatusHudShowFlag;
extern bool KeyGuideShowFlag;
extern bool PlayQueueShowFlag;
extern bool MonochromizeFlag;
extern bool NegativizeFlag;
extern bool RepeatOneFlag;
extern bool RepeatAllFlag;
extern bool ShuffleFlag;
extern bool PauseFlag;

extern const unsigned int White;
extern const unsigned int Red;
extern const unsigned int Blue;
extern const unsigned int Green;
extern const unsigned int Black;
extern const unsigned int Yellow;
extern const unsigned int Cyan;
extern const unsigned int Mazenta;
extern const unsigned int Orange;
extern const unsigned int Purple;
extern const unsigned int Pink;
extern const unsigned int Sky;

extern std::wstring KeyGuideForScreening;
extern std::wstring KeyGuideForInitial;