#include <string>
#include <list>
#include <math.h>
#include "DxLib.h"
#include "opencv2/opencv.hpp"
#include "strconv.h"
#include "defines.h"
#include "externs.h"
#include "MyClass.h"

#ifdef _DEBUG
#pragma comment(lib, "opencv_world455d.lib")
#else
#pragma comment(lib, "opencv_world455.lib")
#endif

#pragma warning(disable:4996)

//プロトタイプ宣言たち
Pixel GetPixelColorAveraged(int GraphHandle, int picked_x, int picked_y, int target_res_x, int target_res_y, int source_res_x, int source_res_y);
int CreateSoftImageFromGraph(int GraphHandle);
int GetFilePath(std::wstring* filename, std::wstring* filepath);
void DrawStatus();
void DrawKeyGuide();
void CheckKeyInputScreening();
void ChangeScreenMode();
int AddMovieToMovieList();
void BeginNextScreening();
void PlayPreviousMovie();
void PlayForwardMovie();
void Screening();
void Initial();

Pixel LED[RES_LED_X][RES_LED_Y];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE); //ウィンドウ表示オン
	SetAlwaysRunFlag(TRUE); //非アクティブ時でも実行を止めない
	SetMainWindowText(L"DotMatrixLEDSimulator");
	SetGraphMode(RES_SCR_X, RES_SCR_Y, 32); //画面解像度[RES_SCR_X] x [RES_SCR_Y], 24ビットカラーに設定
	SetOutApplicationLogValidFlag(FALSE); //Log.txtを出力するか否か
	if (DxLib_Init() == -1)    // ＤＸライブラリ初期化処理
	{
		return -1;    // エラーが発生したら終了
	}
	SetDrawScreen(DX_SCREEN_BACK); //描画先を裏画面に設定


	while (ProcessMessage() == 0) //ウィンドウの閉じるボタンが押されるとループを抜ける
	{
		if (isPushedKey_Esc == FALSE && CheckHitKey(KEY_INPUT_ESCAPE) == TRUE) {
			if (Scene == INITIAL)break; //初期画面でEscキーが押されると終了
		}
		if (isPushedKey_Esc == TRUE && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE) {
			isPushedKey_Esc = FALSE;
		}
		
		ClearDrawScreen();

		switch (Scene) {
		case INITIAL:
			Initial();
			break;
		case SCREENING:
			Screening();
			break;
		}

		ScreenFlip();
	}
 
	// 読み込んだムービーファイルのグラフィックハンドルの削除
	DeleteGraph(hMovie);
	DxLib_End();        // ＤＸライブラリ使用の終了処理

	return 0;        // ソフトの終了
}

void Initial() {
	DrawFormatString(0, 300, Yellow, L"%s", KeyGuideForInitial.c_str());
	if (isPushedKey_O == FALSE && CheckHitKey(KEY_INPUT_O) == TRUE) {
		isPushedKey_O = TRUE;
		if (AddMovieToMovieList() == 0) {
			itrPlayPos = MovieList.begin();
			BeginNextScreening();
		}
	}
	if (isPushedKey_O == TRUE && CheckHitKey(KEY_INPUT_O) == FALSE) {
		isPushedKey_O = FALSE;
	}

	if (isPushedKey_P == FALSE && CheckHitKey(KEY_INPUT_P) == TRUE) {
		isPushedKey_P = TRUE;

	}
	if (isPushedKey_P == TRUE && CheckHitKey(KEY_INPUT_P) == FALSE) {
		isPushedKey_P = FALSE;
	}
}

void Screening() {
	FrameCounter++;
	msPlayTime = TellMovieToGraph(hMovie);
	miPlayTime = msPlayTime / 1000 / 60;
	scPlayTime = msPlayTime / 1000 % 60;
	PlayFrame = TellMovieToGraphToFrame(hMovie);

	hTempFrameSoftImage = CreateSoftImageFromGraph(hMovie);
	GetSoftImageSize(hTempFrameSoftImage, &SourceWidth, &SourceHeight);

	for (int y = 0; y < RES_LED_Y; y++) {
		for (int x = 0; x < RES_LED_X; x++) {
			LED[x][y] = GetPixelColorAveraged(hTempFrameSoftImage, x, y, RES_LED_X, RES_LED_Y, SourceWidth, SourceHeight);
			LED[x][y].reduct_color(R_Tones, G_Tones, B_Tones);
			LED[x][y].correct_gamma(Gamma);
			if (MonochromizeFlag == TRUE)LED[x][y].monochromize();
			if (NegativizeFlag == TRUE)LED[x][y].negativize();
		}
	}
	DeleteSoftImage(hTempFrameSoftImage);
	for (int y = 0; y < RES_LED_Y; y++) {
		for (int x = 0; x < RES_LED_X; x++) {
			Dot_r = LED[x][y].r;
			Dot_g = LED[x][y].g;
			Dot_b = LED[x][y].b;
			DotColor = GetColor(Dot_r, Dot_g, Dot_b);
			DrawCircleAA((float)x * 10, (float)y * 10, 3.0f, 32, DotColor);
		}
	}

	if (StatusHudShowFlag == TRUE)DrawStatus();
	if (KeyGuideShowFlag == TRUE)DrawKeyGuide();


	//if (GetMovieStateToGraph(hMovie) == 0) {
	//	SeekMovieToGraph(hMovie, 0);
	//	PlayMovieToGraph(hMovie);
	//}

	if (PlayFrame >= TotalFrame - 1) {
		if (RepeatFlag == TRUE) {
			SeekMovieToGraph(hMovie, 0);
			PlayMovieToGraph(hMovie);
		}
		if (RepeatFlag == FALSE) {
			itrPlayPos++;
			if (itrPlayPos == MovieList.end()) {
				MovieList.clear();
				Scene = INITIAL;
			}
			if (itrPlayPos != MovieList.end()) {
				BeginNextScreening();
			}
		}
	}

	CheckKeyInputScreening();

	if (Timer + 1000 <= GetNowCount()) {
		FPS = FrameCounter;
		FrameCounter = 0;
		Timer = GetNowCount();
	}
}

void BeginNextScreening() {
	Scene = SCREENING;
	FrameCounter = 0;
	Movie movie = *itrPlayPos;
	std::wstring FileName = movie.FileName;
	std::wstring FilePath = movie.FilePath;
	PlayingFileName = FileName;
	PlayingFilePath = FilePath;
	std::string utf8FilePath = wide_to_utf8(FilePath);
	cv::VideoCapture video;
	video.open(utf8FilePath);
	TotalFrame = video.get(7);
	hMovie = LoadGraph(FilePath.c_str());
	ChangeMovieVolumeToGraph(SoundVolume, hMovie);
	PlayMovieToGraph(hMovie);
	Timer = GetNowCount();
}

void PlayPreviousMovie() {
	PauseMovieToGraph(hMovie);
	itrPlayPos--;
	BeginNextScreening();
}

void PlayForwardMovie() {
	PauseMovieToGraph(hMovie);
	itrPlayPos++;
	BeginNextScreening();
}


void CheckKeyInputScreening() {

	if (isPushedKey_Up == FALSE && CheckHitKey(KEY_INPUT_UP) == TRUE) {
		isPushedKey_Up = TRUE;
		if (SoundVolume + 5 <= 255) {
			SoundVolume += 5;
			ChangeMovieVolumeToGraph(SoundVolume, hMovie);
		}
	}
	if (isPushedKey_Up == TRUE && CheckHitKey(KEY_INPUT_UP) == FALSE) {
		isPushedKey_Up = FALSE;
	}

	if (isPushedKey_Down == FALSE && CheckHitKey(KEY_INPUT_DOWN) == TRUE) {
		isPushedKey_Down = TRUE;
		if (SoundVolume - 5 >= 0) {
			SoundVolume += -5;
			ChangeMovieVolumeToGraph(SoundVolume, hMovie);
		}
	}
	if (isPushedKey_Down == TRUE && CheckHitKey(KEY_INPUT_DOWN) == FALSE) {
		isPushedKey_Down = FALSE;
	}


	if (isPushedKey_Left == FALSE && CheckHitKey(KEY_INPUT_LEFT) == TRUE) {
		isPushedKey_Left = TRUE;
		SeekMovieToGraph(hMovie, msPlayTime - 5000);
	}
	if (isPushedKey_Left == TRUE && CheckHitKey(KEY_INPUT_LEFT) == FALSE) {
		isPushedKey_Left = FALSE;
	}

	if (isPushedKey_Right == FALSE && CheckHitKey(KEY_INPUT_RIGHT) == TRUE) {
		isPushedKey_Right = TRUE;
		SeekMovieToGraph(hMovie, msPlayTime + 5000);
	}
	if (isPushedKey_Right == TRUE && CheckHitKey(KEY_INPUT_RIGHT) == FALSE) {
		isPushedKey_Right = FALSE;
	}

	if (isPushedKey_Space == FALSE && CheckHitKey(KEY_INPUT_SPACE) == TRUE) {
		isPushedKey_Space = TRUE;
		if (PauseFlag == FALSE && GetMovieStateToGraph(hMovie) == 1) {
			PauseMovieToGraph(hMovie);
		}
		else if (PauseFlag == TRUE && GetMovieStateToGraph(hMovie) == 0) {
			PlayMovieToGraph(hMovie);
		}
		PauseFlag = !PauseFlag;
	}
	if (isPushedKey_Space == TRUE && CheckHitKey(KEY_INPUT_SPACE) == FALSE) {
		isPushedKey_Space = FALSE;
	}

	if (isPushedKey_Z == FALSE && CheckHitKey(KEY_INPUT_Z) == TRUE) {
		isPushedKey_Z = TRUE;
		if (itrPlayPos != MovieList.begin()) {
			PlayPreviousMovie();
		}
	}
	if (isPushedKey_Z == TRUE && CheckHitKey(KEY_INPUT_Z) == FALSE) {
		isPushedKey_Z = FALSE;
	}

	if (isPushedKey_C == FALSE && CheckHitKey(KEY_INPUT_C) == TRUE) {
		isPushedKey_C = TRUE;
		if (itrPlayPos != --MovieList.end()) {
			PlayForwardMovie();
		}
	}
	if (isPushedKey_C == TRUE && CheckHitKey(KEY_INPUT_C) == FALSE) {
		isPushedKey_C = FALSE;
	}

	if (isPushedKey_W == FALSE && CheckHitKey(KEY_INPUT_W) == TRUE) {
		isPushedKey_W = TRUE;
		if (ColorDepthCustomID <= 13) {
			ColorDepthCustomID++;
			R_Tones = ColorDepthCustoms[ColorDepthCustomID][0];
			G_Tones = ColorDepthCustoms[ColorDepthCustomID][1];
			B_Tones = ColorDepthCustoms[ColorDepthCustomID][2];
		}
	}
	if (isPushedKey_W == TRUE && CheckHitKey(KEY_INPUT_W) == FALSE) {
		isPushedKey_W = FALSE;
	}

	if (isPushedKey_S == FALSE && CheckHitKey(KEY_INPUT_S) == TRUE) {
		isPushedKey_S = TRUE;
		if (ColorDepthCustomID >= 1) {
			ColorDepthCustomID--;
			R_Tones = ColorDepthCustoms[ColorDepthCustomID][0];
			G_Tones = ColorDepthCustoms[ColorDepthCustomID][1];
			B_Tones = ColorDepthCustoms[ColorDepthCustomID][2];
		}
	}
	if (isPushedKey_S == TRUE && CheckHitKey(KEY_INPUT_S) == FALSE) {
		isPushedKey_S = FALSE;
	}

	if (isPushedKey_A == FALSE && CheckHitKey(KEY_INPUT_A) == TRUE) {
		isPushedKey_A = TRUE;
		if (Gamma - GammaAdjustStep > 0.1) {
			Gamma += -GammaAdjustStep;
		}
	}
	if (isPushedKey_A == TRUE && CheckHitKey(KEY_INPUT_A) == FALSE) {
		isPushedKey_A = FALSE;
	}


	if (isPushedKey_D == FALSE && CheckHitKey(KEY_INPUT_D) == TRUE) {
		isPushedKey_D = TRUE;
		if (Gamma + GammaAdjustStep < 10.0) {
			Gamma += GammaAdjustStep;
		}
	}
	if (isPushedKey_D == TRUE && CheckHitKey(KEY_INPUT_D) == FALSE) {
		isPushedKey_D = FALSE;
	}

	if (isPushedKey_F1 == FALSE && CheckHitKey(KEY_INPUT_F1) == TRUE) {
		isPushedKey_F1 = TRUE;
		KeyGuideShowFlag = !KeyGuideShowFlag;
	}
	if (isPushedKey_F1 == TRUE && CheckHitKey(KEY_INPUT_F1) == FALSE) {
		isPushedKey_F1 = FALSE;
	}

	if (isPushedKey_F3 == FALSE && CheckHitKey(KEY_INPUT_F3) == TRUE) {
		isPushedKey_F3 = TRUE;
		StatusHudShowFlag = !StatusHudShowFlag;
	}
	if (isPushedKey_F3 == TRUE && CheckHitKey(KEY_INPUT_F3) == FALSE) {
		isPushedKey_F3 = FALSE;
	}

	if (isPushedKey_F11 == FALSE && CheckHitKey(KEY_INPUT_F11) == TRUE) {
		isPushedKey_F11 = TRUE;
		FullscreenFlag = !FullscreenFlag;
		ChangeScreenMode();
	}
	if (isPushedKey_F11 == TRUE && CheckHitKey(KEY_INPUT_F11) == FALSE) {
		isPushedKey_F11 = FALSE;
	}

	if (isPushedKey_M == FALSE && CheckHitKey(KEY_INPUT_M) == TRUE) {
		isPushedKey_M = TRUE;
		MonochromizeFlag = !MonochromizeFlag;
	}
	if (isPushedKey_M == TRUE && CheckHitKey(KEY_INPUT_M) == FALSE) {
		isPushedKey_M = FALSE;
	}

	if (isPushedKey_N == FALSE && CheckHitKey(KEY_INPUT_N) == TRUE) {
		isPushedKey_N = TRUE;
		NegativizeFlag = !NegativizeFlag;
	}
	if (isPushedKey_N == TRUE && CheckHitKey(KEY_INPUT_N) == FALSE) {
		isPushedKey_N = FALSE;
	}

	if (isPushedKey_R == FALSE && CheckHitKey(KEY_INPUT_R) == TRUE) {
		isPushedKey_R = TRUE;
		RepeatFlag = !RepeatFlag;
	}
	if (isPushedKey_R == TRUE && CheckHitKey(KEY_INPUT_R) == FALSE) {
		isPushedKey_R = FALSE;
	}

	if (isPushedKey_F == FALSE && CheckHitKey(KEY_INPUT_F) == TRUE) {
		isPushedKey_F = TRUE;
		ShuffleFlag = !ShuffleFlag;
	}
	if (isPushedKey_F == TRUE && CheckHitKey(KEY_INPUT_F) == FALSE) {
		isPushedKey_F = FALSE;
	}

	if (isPushedKey_O == FALSE && CheckHitKey(KEY_INPUT_O) == TRUE) {
		isPushedKey_O = TRUE;
		if (PauseFlag == FALSE)PauseMovieToGraph(hMovie);
		AddMovieToMovieList();
		if (PauseFlag == FALSE)PlayMovieToGraph(hMovie);
	}
	if (isPushedKey_O == TRUE && CheckHitKey(KEY_INPUT_O) == FALSE) {
		isPushedKey_O = FALSE;
	}

	if (isPushedKey_P == FALSE && CheckHitKey(KEY_INPUT_P) == TRUE) {
		isPushedKey_P = TRUE;

	}
	if (isPushedKey_P == TRUE && CheckHitKey(KEY_INPUT_P) == FALSE) {
		isPushedKey_P = FALSE;
	}

	if (isPushedKey_Esc == FALSE && CheckHitKey(KEY_INPUT_ESCAPE) == TRUE) {
		isPushedKey_Esc = TRUE;
		PauseMovieToGraph(hMovie);
		MovieList.clear();
		Scene = INITIAL;
	}
	if (isPushedKey_Esc == TRUE && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE) {
		isPushedKey_Esc = FALSE;
	}

}

int AddMovieToMovieList() {
	std::wstring FileName;
	std::wstring FilePath;
	if (GetFilePath(&FileName, &FilePath) == -1)return -1;
	Movie movie(FileName, FilePath);
	MovieList.push_back(movie);
	return 0;
}


int GetFilePath(std::wstring *filename, std::wstring *filepath) {
	OPENFILENAME ofn;
	wchar_t FilePath[MAX_PATH];
	wchar_t FileName[MAX_PATH];

	//OPENFILENAME構造体を設定
	memset(&ofn, 0, sizeof(OPENFILENAME));
	memset(FilePath, 0, sizeof(FilePath));
	memset(FileName, 0, sizeof(FileName));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetMainWindowHandle();
	ofn.lpstrFilter = 
		L"すべてのファイル(*.*)\0*.*"
		L"\0動画ファイル(*.mp4; *.m4v; *.webm; *.flv; *.mpg; *.avi)\0*.mp4;*.m4v;*.webm;*.flv;*.mpg;*.avi"
		L"\0MP4ファイル(*.mp4)\0*.mp4"
		L"\0M4Vファイル(*.m4v)\0*.m4v"
		L"\0MKVファイル(*.mkv)\0*.mkv"
		L"\0WEBMファイル(*.webm)\0*.webm"
		L"\0FLVファイル(*.flv)\0*.flv"
		L"\0MPEGファイル(*.mpg)\0 *.mpg"
		L"\0AVIファイル(*.avi)\0 *.avi\0"
		L"\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFile = FilePath;
	ofn.lpstrFileTitle = FileName;
	ofn.nMaxFile = sizeof(FilePath);
	ofn.nMaxFileTitle = sizeof(FileName);
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";
	ofn.lpstrTitle = L"動画ファイルを開く";

	if (GetOpenFileName(&ofn) == 0)return -1;
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "%s", FullPath);
	//ScreenFlip();
	//WaitKey();
	*filename = FileName;
	*filepath = FilePath;
	//DrawFormatString(0, 0, GetColor(255, 0, 0), "%s", FullFileName);
	//ScreenFlip();
	//WaitKey();

	return 0;
}


int CreateSoftImageFromGraph(int GraphHandle) {
	int SourceSizeX, SourceSizeY;
	int TempScreen, SIHandle;

	// 変換したいグラフィックハンドルと同じサイズの「描画対象にできるグラフィックハンドル」を作成
	GetGraphSize(GraphHandle, &SourceSizeX, &SourceSizeY);
	TempScreen = MakeScreen(SourceSizeX, SourceSizeY, TRUE);

	// 「描画対象にできるグラフィックハンドル」を描画対象に変更
	SetDrawScreen(TempScreen);

	// 変換したいグラフィックハンドルを描画
	// 描画元の色値をそのまま書き込むためのブレンドモード DX_BLENDMODE_SRCCOLOR を使用する
	SetDrawBlendMode(DX_BLENDMODE_SRCCOLOR, 255);
	DrawGraph(0, 0, GraphHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// 変換したいグラフィックハンドルと同じサイズの「CPUで扱うイメージ」を作成
	SIHandle = MakeARGB8ColorSoftImage(SourceSizeX, SourceSizeY);

	// 「CPUで扱うイメージ」に「描画対象にできるグラフィックハンドル」の内容を取り込み
	GetDrawScreenSoftImage(0, 0, SourceSizeX, SourceSizeY, SIHandle);

	// 描画対象を裏画面に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 作業用に作成した「描画対象にできるグラフィックハンドル」を削除
	DeleteGraph(TempScreen);

	return SIHandle;
}

Pixel GetPixelColorAveraged(int GraphHandle, int picked_x, int picked_y, int target_res_x, int target_res_y, int source_res_x, int source_res_y) {
	int sampling_range_x = source_res_x / target_res_x;
	int sampling_range_y = source_res_y / target_res_y;
	int sampling_index_pos_x = (int)((double)source_res_x / (double)target_res_x * picked_x);
	int sampling_index_pos_y = (int)((double)source_res_y / (double)target_res_y * picked_y);
	int samples = sampling_range_x * sampling_range_y;

	int r_total = 0;
	int g_total = 0;
	int b_total = 0;
	int a_total = 0;
	int got_r, got_g, got_b, got_a;
	int* pgot_r = &got_r;
	int* pgot_g = &got_g;
	int* pgot_b = &got_b;
	int* pgot_a = &got_a;
	int tmp_x, tmp_y;
	for (int y = 0; y < sampling_range_y; y++) {
		for (int x = 0; x < sampling_range_x; x++) {
			tmp_x = sampling_index_pos_x + x;
			tmp_y = sampling_index_pos_y + y;
			GetPixelSoftImage(GraphHandle, tmp_x, tmp_y, pgot_r, pgot_g, pgot_b, pgot_a);
			r_total += got_r;
			g_total += got_g;
			b_total += got_b;
			a_total += got_a;
		}
	}
	int r_average = r_total / samples;
	int g_average = g_total / samples;
	int b_average = b_total / samples;
	int a_average = a_total / samples;

	Pixel averaged(r_average, g_average, b_average, a_average);
	return averaged;
}
void DrawStatus() {
	ColorTones = R_Tones * G_Tones * B_Tones;
	DrawFormatString(0, 0  , White, L"FileName:    %ls", PlayingFileName.c_str());
	DrawFormatString(0, 15 , White, L"PlayTime:    %d\'%02d\" (%dms)", miPlayTime, scPlayTime, msPlayTime);
	DrawFormatString(0, 30 , White, L"PlayFrame:   %d / %d", PlayFrame, TotalFrame);
	DrawFormatString(0, 45 , White, L"SoundVolume: %d%%", (int)((double)SoundVolume/255.0*100));
	DrawFormatString(0, 60 , White, L"ColorDepth:  %dx%dx%d=%d", R_Tones, G_Tones, B_Tones, ColorTones);
	DrawFormatString(0, 75 , White, L"Gamma:       %.2lf", Gamma);
	DrawFormatString(0, 90 , White, L"Monochrome:  %s", MonochromizeFlag ? L"True" : L"False");
	DrawFormatString(0, 105, White, L"Fullscreen:  %s", FullscreenFlag ? L"True" : L"False");
	DrawFormatString(0, 120, White, L"Negative:    %s", NegativizeFlag ? L"True" : L"False");
	DrawFormatString(0, 135, White, L"Repeat:      %s", RepeatFlag ? L"True" : L"False");
	DrawFormatString(0, 150, White, L"Shuffle:     %s", ShuffleFlag ? L"True" : L"False");
	DrawFormatString(0, 165, White, L"Pause:       %s", PauseFlag ? L"True" : L"False");
	DrawFormatString(1200, 700, White, L"%dFPS", FPS);
}

void DrawKeyGuide() {
	DrawFormatString(0, 300, Yellow, L"%s", KeyGuideForScreening.c_str());
}

void ChangeScreenMode() {
	bool WindowMode = !FullscreenFlag;
	ChangeWindowMode(WindowMode);
	hMovie = LoadGraph(PlayingFilePath.c_str());
	ChangeMovieVolumeToGraph(SoundVolume, hMovie);
	SeekMovieToGraph(hMovie, msPlayTime);
	PlayMovieToGraph(hMovie);
}