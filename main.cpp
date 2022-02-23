#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <list>
#include <math.h>
#include <ShlObj.h>
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
//bool GetFilePath(std::wstring* FileName, std::wstring* FilePath);
void OpenFiles();
//bool GetDirectoryPath(std::wstring* DirectoryPath);
//BOOL GetOpenDirectoryPath(std::wstring* DirectoryPath);
void DrawStatus();
void DrawKeyGuide();
void CheckKeyInput_onScreening();
void ChangeScreenMode_onScreening();
int AddMovieToMovieList(std::wstring FileName, std::wstring FilePath);
void BeginScreening(bool ResumeFlag=FALSE);
void PlayPreviousMovie();
void PlayForwardMovie();
void Screening();
void Initial();



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
		if (PushFlag_Key_Esc == FALSE && CheckHitKey(KEY_INPUT_ESCAPE) == TRUE) {
			if (Scene == INITIAL)break; //初期画面でEscキーが押されると終了
		}
		if (PushFlag_Key_Esc == TRUE && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE) {
			PushFlag_Key_Esc = FALSE;
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


	if (PushFlag_Key_O == FALSE && CheckHitKey(KEY_INPUT_O) == TRUE) {
		PushFlag_Key_O = TRUE;
		OpenFiles();
		//std::wstring FileName;
		//std::wstring FilePath;
		//if (GetFilePath(&FileName, &FilePath) == TRUE) {
		//	AddMovieToMovieList(FileName, FilePath);
		//}
	}
	if (PushFlag_Key_O == TRUE && CheckHitKey(KEY_INPUT_O) == FALSE) {
		PushFlag_Key_O = FALSE;
	}


	//if (PushFlag_Key_P == FALSE && CheckHitKey(KEY_INPUT_P) == TRUE) {
	//	PushFlag_Key_P = TRUE;
	//	std::wstring DirectoryPath;
	//	if (GetDirectoryPath(&DirectoryPath) == TRUE) {
	//		DrawFormatString(0, 0, GetColor(255, 0, 0), L"%ls", DirectoryPath.c_str());
	//		ScreenFlip();
	//		WaitKey();
	//	}
	//}
	//if (PushFlag_Key_P == TRUE && CheckHitKey(KEY_INPUT_P) == FALSE) {
	//	PushFlag_Key_P = FALSE;
	//}


	if (PushFlag_Key_F11 == FALSE && CheckHitKey(KEY_INPUT_F11) == TRUE) {
		PushFlag_Key_F11 = TRUE;
		FullscreenFlag = !FullscreenFlag;
		ChangeWindowMode(!FullscreenFlag);
	}
	if (PushFlag_Key_F11 == TRUE && CheckHitKey(KEY_INPUT_F11) == FALSE) {
		PushFlag_Key_F11 = FALSE;
	}


	if (PushFlag_Key_Enter == FALSE && CheckHitKey(KEY_INPUT_RETURN) == TRUE) {
		PushFlag_Key_Enter = TRUE;
		if (MovieList.size() > 0) {
			itrMovieList = MovieList.begin();
			BeginScreening();
		}
	}
	if (PushFlag_Key_Enter == TRUE && CheckHitKey(KEY_INPUT_RETURN) == FALSE) {
		PushFlag_Key_Enter = FALSE;
	}


	if (PushFlag_Key_Delete == FALSE && CheckHitKey(KEY_INPUT_DELETE) == TRUE) {
		PushFlag_Key_Delete = TRUE;
		MovieList.clear();
	}
	if (PushFlag_Key_Delete == TRUE && CheckHitKey(KEY_INPUT_DELETE) == FALSE) {
		PushFlag_Key_Delete = FALSE;
	}



}


void Screening() {
	FrameCounter++;
	PlayTimeMilliseconds = TellMovieToGraph(hMovie);
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
			DrawCircleAA((float)x * 10, (float)y * 10, 3.0f, 16, DotColor);
			//DrawCircle((float)x * 10, (float)y * 10, 3.0f, DotColor);
		}
	}

	if (StatusHudShowFlag == TRUE)DrawStatus();
	if (KeyGuideShowFlag == TRUE)DrawKeyGuide();

	if (GetMovieStateToGraph(hMovie) == 0 && PauseFlag == FALSE) {
		if (RepeatOneFlag == TRUE) {
			SeekMovieToGraph(hMovie, 0);
			PlayMovieToGraph(hMovie);
		}
		if (RepeatOneFlag == FALSE) {
			++itrMovieList;
			if (itrMovieList == MovieList.end()) {
				if (RepeatAllFlag == TRUE) {
					itrMovieList = MovieList.begin();
					BeginScreening();
				}
				if (RepeatAllFlag == FALSE) {
					//MovieList.clear();
					DeleteGraph(hMovie);
					Scene = INITIAL;
				}
			}
			if (itrMovieList != MovieList.end()) {
				BeginScreening();
			}
		}
	}

	CheckKeyInput_onScreening();

	if (Timer + 1000 <= GetNowCount()) {
		FPS = FrameCounter;
		FrameCounter = 0;
		Timer = GetNowCount();
	}
}



void BeginScreening(bool ResumeFlag) {
	Scene = SCREENING;
	FrameCounter = 0;
	Movie movie = *itrMovieList;
	std::wstring FileName = movie.FileName;
	std::wstring FilePath = movie.FilePath;
	PlayingFileName = FileName;
	PlayingFilePath = FilePath;
	TotalFrames = movie.TotalFrames;
	TotalTimeSeconds = movie.TotalTimeSeconds;
	TotalTime_min = TotalTimeSeconds / 60;
	TotalTime_sec = TotalTimeSeconds % 60;
	DeleteGraph(hMovie);
	hMovie = LoadGraph(FilePath.c_str());
	ChangeMovieVolumeToGraph(SoundVolume, hMovie);
	if (ResumeFlag == TRUE)SeekMovieToGraphToFrame(hMovie, movie.PlayResumeFrame);
	//if (ResumeFlag == TRUE)SeekMovieToGraphToFrame(hMovie, 2000);
	PlayMovieToGraph(hMovie);
	Timer = GetNowCount();
}



void PlayPreviousMovie() {
	PauseMovieToGraph(hMovie);
	//Movie movie = *itrMovieList;
	itrMovieList->PlayResumeFrame = TellMovieToGraphToFrame(hMovie);
	--itrMovieList;
	BeginScreening(TRUE);
}


void PlayForwardMovie() {
	PauseMovieToGraph(hMovie);
	//Movie movie = *itrMovieList;
	itrMovieList->PlayResumeFrame = TellMovieToGraphToFrame(hMovie);
	++itrMovieList;
	BeginScreening(TRUE);
}



int AddMovieToMovieList(std::wstring FileName, std::wstring FilePath) {
	std::string utf8FilePath = wide_to_utf8(FilePath);
	cv::VideoCapture video;
	video.open(utf8FilePath);
	if (video.isOpened() == FALSE)return -1;
	int totalframes = video.get(cv::CAP_PROP_FRAME_COUNT);
	double fps = video.get(cv::CAP_PROP_FPS);
	Movie movie(FileName, FilePath, 0, totalframes, fps);
	MovieList.push_back(movie);
	return 0;
}



void OpenFiles() {
	HRESULT hr;
	IFileOpenDialog* pIFileOpenDialog = nullptr;

	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIFileOpenDialog));
	if (FAILED(hr) || pIFileOpenDialog == NULL)return;

	DWORD options;
	pIFileOpenDialog->SetTitle(L"動画ファイルを開く");
	pIFileOpenDialog->GetOptions(&options);
	pIFileOpenDialog->SetOptions(options | FOS_ALLOWMULTISELECT);

	COMDLG_FILTERSPEC filterspec[] = {
		{L"すべてのファイル(*.*)" ,L"*.*"},
		{L"対応している形式の動画ファイル(*.mp4; *.m4v; *.mkv; *.webm; *.mpg; *.avi)" ,L"*.mp4;*.m4v;*.mkv;*.webm;*.mpg;*.avi"},
		{L"MP4ファイル(*.mp4)" ,L"*.mp4"},
		{L"M4Vファイル(*.m4v)" ,L"*.m4v"},
		{L"MKVファイル(*.mkv)" ,L"*.mkv"},
		{L"WEBMファイル(*.webm)" ,L"*.webm"},
		{L"MPEGファイル(*.mpg)" ,L"*.mpg"},
		{L"AVIファイル(*.avi)" ,L"*.avi"}
	};
	hr = pIFileOpenDialog->SetFileTypes(_countof(filterspec), filterspec);
	if (FAILED(hr))return;

	hr = pIFileOpenDialog->SetFileTypeIndex(2);
	if (FAILED(hr))return;

	hr = pIFileOpenDialog->Show(GetMainWindowHandle());
	if (FAILED(hr))return;

	IShellItemArray* pIShellItemArray = nullptr;

	hr = pIFileOpenDialog->GetResults(&pIShellItemArray);
	if (FAILED(hr))return;

	DWORD ItemCount;

	hr = pIShellItemArray->GetCount(&ItemCount);
	if (FAILED(hr))return;

	//std::wofstream fout;
	//fout.imbue(std::locale(".65001"));
	//fout.open(L"D:\\Users\\takum\\source\\repos\\DotMatrixLEDSimulator\\filenames.txt");
	for (DWORD i = 0; i < ItemCount; i++) {
		IShellItem* pIShellItem = nullptr;
		LPWSTR lpszFileName;
		LPWSTR lpszFilePath;

		hr = pIShellItemArray->GetItemAt(i, &pIShellItem);
		if (FAILED(hr))continue;
		hr = pIShellItem->GetDisplayName(SIGDN_NORMALDISPLAY, &lpszFileName);
		if (FAILED(hr))continue;
		hr = pIShellItem->GetDisplayName(SIGDN_FILESYSPATH, &lpszFilePath);
		if (FAILED(hr))continue;

		AddMovieToMovieList(std::wstring(lpszFileName), std::wstring(lpszFilePath));
		//DrawFormatString(0, i * 15, Orange, L"%ls", lpszFilePath);
		//fout << lpszFilePath << std::endl;

		pIShellItem->Release();
		CoTaskMemFree(lpszFileName);
		CoTaskMemFree(lpszFilePath);
	}
	//fout.close();
	//ScreenFlip();
	//WaitKey();
	pIShellItemArray->Release();
	pIFileOpenDialog->Release();


					//wchar_t* lpszFilePath = nullptr;
					//wchar_t* lpszFileName = nullptr;

					//pIShellItem->GetDisplayName(SIGDN_NORMALDISPLAY, &lpszFileName);
					//pIShellItem->GetDisplayName(SIGDN_FILESYSPATH, &lpszFilePath);

					//DrawFormatString(0, 0, Red, L"Dir:%ls", lpszFilePath);
					//DrawFormatString(0, 15, Red, L"Dir:%ls", lpszFileName);

					//ScreenFlip();
					//WaitKey();

	//CFileDialog fopenDlg(TRUE, NULL, NULL, fopenFlags, fopenFilter, (CWnd*)GetMainWindowHandle(), 0, TRUE);
	//if (fopenDlg.DoModal() == IDOK);


	//OPENFILENAME ofn;
	//const int BufferSize = 37772;
	//wchar_t lpstrBuffer[BufferSize];

	//memset(&ofn, 0, sizeof(OPENFILENAME));
	//memset(lpstrBuffer, 0, sizeof(lpstrBuffer));

	//ofn.lStructSize = sizeof(OPENFILENAME);
	//ofn.hwndOwner = GetMainWindowHandle();
	//ofn.lpstrFilter =
	//	L"すべてのファイル(*.*)\0*.*"
	//	L"\0対応している形式の動画ファイル(*.mp4; *.m4v; *.mkv; *.webm; *.flv; *.mpg; *.avi)\0*.mp4;*.m4v;*.mkv;*.webm;*.flv;*.mpg;*.avi"
	//	L"\0MP4ファイル(*.mp4)\0*.mp4"
	//	L"\0M4Vファイル(*.m4v)\0*.m4v"
	//	L"\0MKVファイル(*.mkv)\0*.mkv"
	//	L"\0WEBMファイル(*.webm)\0*.webm"
	//	L"\0FLVファイル(*.flv)\0*.flv"
	//	L"\0MPEGファイル(*.mpg)\0 *.mpg"
	//	L"\0AVIファイル(*.avi)\0 *.avi\0"
	//	L"\0";
	//ofn.nFilterIndex = 1;
	//ofn.lpstrFile = lpstrBuffer;
	//ofn.nMaxFile = sizeof(lpstrBuffer);
	//ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	//ofn.lpstrTitle = L"動画ファイルを開く";
	//
	//if (GetOpenFileName(&ofn) == 1) {
	//	std::wstring FileName;
	//	std::wstring DirectoryPath;
	//	std::vector<std::wstring> FileNames;
	//	std::vector<wchar_t> vtrBuffer[BufferSize];
	//	for (int i = 0; i < BufferSize; i++) {
	//		vtrBuffer->push_back(lpstrBuffer[i]);
	//	}
	//	std::wstring wstringraw(vtrBuffer->begin(), vtrBuffer->end());
	//	std::wstringstream wssraw(wstringraw);

	//	std::getline(wssraw, DirectoryPath, L'\0');

	//	while (std::getline(wssraw, FileName, L'\0')) {
	//		if(FileName != L"\0")FileNames.push_back(FileName);
	//	}

	//	std::wstring FilePath;
	//	if (FileNames.size() == 0) {
	//		FileName = DirectoryPath.substr(ofn.nFileOffset, DirectoryPath.length() - ofn.nFileOffset);
	//		FilePath = DirectoryPath;
	//		AddMovieToMovieList(FileName, FilePath);
	//	}
	//	else if (FileNames.size() >= 1) {
	//		for (int i = 0; i < FileNames.size(); i++) {
	//			FileName = FileNames[i];
	//			FilePath = DirectoryPath + L"\\" + FileName;
	//			AddMovieToMovieList(FileName, FilePath);
	//		}
	//	}

	//	DrawFormatString(300, 0, Red, L"Dir:%ls", DirectoryPath.c_str());
	//	for (int i = 0, y = 0; i < FileNames.size(); i++) {
	//		y = 15 + 15 * i;
	//		DrawFormatString(300, y, Orange, L"%d:%ls", i, FileNames[i].c_str());
	//	}
	//	ScreenFlip();
	//	WaitKey();

	//	std::wofstream fout;
	//	fout.imbue(std::locale(".65001"));
	//	fout.open(L"filenames.txt");
	//	fout << DirectoryPath << std::endl;
	//	for (int i = 0; i < FileNames.size(); i++) {
	//		fout << FileNames[i] << std::endl;
	//	}
	//	fout.close();
	//}

}


//bool GetFilePath(std::wstring* FileName, std::wstring* FilePath) {
//	OPENFILENAME ofn;
//	wchar_t FilePathBuffer[10000];
//	wchar_t FileNameBuffer[MAX_PATH];
//
//	//OPENFILENAME構造体を設定
//	memset(&ofn, 0, sizeof(OPENFILENAME));
//	memset(FilePathBuffer, 0, sizeof(FilePathBuffer));
//	memset(FileNameBuffer, 0, sizeof(FileNameBuffer));
//	ofn.lStructSize = sizeof(OPENFILENAME);
//	ofn.hwndOwner = GetMainWindowHandle();
//	ofn.lpstrFilter =
//		L"すべてのファイル(*.*)\0*.*"
//		L"\0対応している形式の動画ファイル(*.mp4; *.m4v; *.mkv; *.webm; *.flv; *.mpg; *.avi)\0*.mp4;*.m4v;*.mkv;*.webm;*.flv;*.mpg;*.avi"
//		L"\0MP4ファイル(*.mp4)\0*.mp4"
//		L"\0M4Vファイル(*.m4v)\0*.m4v"
//		L"\0MKVファイル(*.mkv)\0*.mkv"
//		L"\0WEBMファイル(*.webm)\0*.webm"
//		L"\0FLVファイル(*.flv)\0*.flv"
//		L"\0MPEGファイル(*.mpg)\0 *.mpg"
//		L"\0AVIファイル(*.avi)\0 *.avi\0"
//		L"\0";
//	ofn.nFilterIndex = 1;
//	ofn.lpstrFile = FilePathBuffer;
//	ofn.lpstrFileTitle = FileNameBuffer;
//	ofn.nMaxFile = sizeof(FilePathBuffer);
//	ofn.nMaxFileTitle = sizeof(FileNameBuffer);
//	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
//	ofn.lpstrDefExt = L"";
//	ofn.lpstrTitle = L"動画ファイルを開く";
//
//	if (GetOpenFileName(&ofn) == 0)return FALSE;
//	//DrawFormatString(0, 0, GetColor(255, 255, 255), "%s", FullPath);
//	//ScreenFlip();
//	//WaitKey();
//	*FileName = FileNameBuffer;
//	*FilePath = FilePathBuffer;
//	//DrawFormatString(0, 0, GetColor(255, 0, 0), "%s", FullFileName);
//	//ScreenFlip();
//	//WaitKey();
//
//	return TRUE;
//}


//bool GetDirectoryPath(std::wstring* DirectoryPath) {
//	OPENFILENAME ofn;
//	wchar_t DirectoryPathBuffer[MAX_PATH];
//
//	//OPENFILENAME構造体を設定
//	memset(&ofn, 0, sizeof(OPENFILENAME));
//	memset(DirectoryPathBuffer, 0, sizeof(DirectoryPathBuffer));
//
//	ofn.lStructSize = sizeof(OPENFILENAME);
//	ofn.hwndOwner = GetMainWindowHandle();
//	ofn.lpstrFilter =
//		L"フォルダ\0.*.*\0"
//		L"\0";
//	ofn.nFilterIndex = 1;
//	ofn.lpstrFile = DirectoryPathBuffer;
//	ofn.nMaxFile = sizeof(DirectoryPathBuffer);
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
//	ofn.lpstrDefExt = L"";
//	ofn.lpstrTitle = L"動画ファイルを含むフォルダを開く";
//
//	if (GetOpenFileName(&ofn) == 0)return FALSE;
//	*DirectoryPath = DirectoryPathBuffer;
//	return TRUE;
//}


//static
//int CALLBACK SHBrowseProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
//{
//	if (uMsg == BFFM_INITIALIZED && lpData)
//	{
//		//  デフォルトで選択させるパスの指定
//		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
//	}
//	return 0;
//}
//static
//void SHFree(ITEMIDLIST* pidl)
//{
//	IMalloc* pMalloc;
//	SHGetMalloc(&pMalloc);
//	if (pMalloc)
//	{
//		pMalloc->Free(pidl);
//		pMalloc->Release();
//	}
//}
//
//BOOL GetOpenDirectoryPath(std::wstring *DirectoryPath)
//{
//	BROWSEINFO  bi;
//	wchar_t lpszBuffer[MAX_PATH];
//	DWORD dwBufferSize = sizeof(lpszBuffer);
//	const wchar_t lpszInitialDirectory[] = L"D:\\Users\\takum\\Videos";
//
//	ZeroMemory(&bi, sizeof(BROWSEINFO));
//	ZeroMemory(lpszBuffer, sizeof(lpszBuffer));
//
//	bi.hwndOwner = GetMainWindowHandle();
//	bi.lpfn = SHBrowseProc;	//  コールバック関数を指定
//	bi.lParam = (LPARAM)lpszInitialDirectory;	//  デフォルトで選択させておくフォルダを指定
//	bi.lpszTitle = _T("動画ファイルを含むフォルダを開く");	//  タイトルの指定
//
//	ITEMIDLIST* pidl = SHBrowseForFolder(&bi);	//  フォルダダイアログの起動
//	if (pidl)
//	{
//		//  選択されたフォルダ名を取得
//		TCHAR  szSelectedFolder[_MAX_PATH];
//		SHGetPathFromIDList(pidl, szSelectedFolder);
//		SHFree(pidl);
//		if ((DWORD)_tcslen(szSelectedFolder) < dwBufferSize)
//		{
//			_tcsncpy_s(lpszBuffer, dwBufferSize, szSelectedFolder, _TRUNCATE);
//			*DirectoryPath = lpszBuffer;
//			//  フォルダが選択された
//			return TRUE;
//		}
//	}
//	//  フォルダは選択されなかった
//	return FALSE;
//}



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
	int PlayTime_min = PlayTimeMilliseconds / 1000 / 60;
	int PlayTime_sec = PlayTimeMilliseconds / 1000 % 60;
	ColorTones = R_Tones * G_Tones * B_Tones;

	DrawFormatString(0, 0  , White, L"FileName:    %ls", PlayingFileName.c_str());
	DrawFormatString(0, 15 , White, L"PlayTime:    %d\'%02d\" / %d\'%02d\" (%dms)", PlayTime_min, PlayTime_sec, TotalTime_min, TotalTime_sec, PlayTimeMilliseconds);
	DrawFormatString(0, 30 , White, L"PlayFrame:   %d / %d", PlayFrame, TotalFrames);
	DrawFormatString(0, 45 , White, L"SoundVolume: %d%%", (int)((double)SoundVolume/255.0*100));
	DrawFormatString(0, 60 , White, L"ColorDepth:  %d (%dx%dx%d)", ColorTones, R_Tones, G_Tones, B_Tones);
	DrawFormatString(0, 75 , White, L"Gamma:       %.2lf", Gamma);
	DrawFormatString(0, 90 , White, L"Monochrome:  %s", MonochromizeFlag ? L"True" : L"False");
	DrawFormatString(0, 105, White, L"Negative:    %s", NegativizeFlag ? L"True" : L"False");
	DrawFormatString(0, 120, White, L"Fullscreen:  %s", FullscreenFlag ? L"True" : L"False");
	DrawFormatString(0, 135, White, L"RepeatOne:   %s", RepeatOneFlag ? L"True" : L"False");
	DrawFormatString(0, 150, White, L"RepeatAll:   %s", RepeatAllFlag ? L"True" : L"False");
	DrawFormatString(0, 165, White, L"Shuffle:     %s", ShuffleFlag ? L"True" : L"False");
	DrawFormatString(0, 180, White, L"Pause:       %s", PauseFlag ? L"True" : L"False");
	DrawFormatString(1200, 700, White, L"%dFPS", FPS);
}



void DrawKeyGuide() {
	DrawFormatString(0, 300, Yellow, L"%s", KeyGuideForScreening.c_str());
}



void ChangeScreenMode_onScreening() {
	bool WindowMode = !FullscreenFlag;
	ChangeWindowMode(WindowMode);
	hMovie = LoadGraph(PlayingFilePath.c_str());
	ChangeMovieVolumeToGraph(SoundVolume, hMovie);
	SeekMovieToGraph(hMovie, PlayTimeMilliseconds);
	if (PauseFlag == FALSE)PlayMovieToGraph(hMovie);
}


void CheckKeyInput_onScreening() {


	if (PushFlag_Key_F1 == FALSE && CheckHitKey(KEY_INPUT_F1) == TRUE) {
		PushFlag_Key_F1 = TRUE;
		KeyGuideShowFlag = !KeyGuideShowFlag;
	}
	if (PushFlag_Key_F1 == TRUE && CheckHitKey(KEY_INPUT_F1) == FALSE) {
		PushFlag_Key_F1 = FALSE;
	}


	if (PushFlag_Key_F3 == FALSE && CheckHitKey(KEY_INPUT_F3) == TRUE) {
		PushFlag_Key_F3 = TRUE;
		StatusHudShowFlag = !StatusHudShowFlag;
	}
	if (PushFlag_Key_F3 == TRUE && CheckHitKey(KEY_INPUT_F3) == FALSE) {
		PushFlag_Key_F3 = FALSE;
	}


	if (PushFlag_Key_F11 == FALSE && CheckHitKey(KEY_INPUT_F11) == TRUE) {
		PushFlag_Key_F11 = TRUE;
		FullscreenFlag = !FullscreenFlag;
		ChangeScreenMode_onScreening();
	}
	if (PushFlag_Key_F11 == TRUE && CheckHitKey(KEY_INPUT_F11) == FALSE) {
		PushFlag_Key_F11 = FALSE;
	}


	if (PushFlag_Key_W == FALSE && CheckHitKey(KEY_INPUT_W) == TRUE) {
		PushFlag_Key_W = TRUE;
		if (ColorDepthCustomID <= 13) {
			ColorDepthCustomID++;
			R_Tones = ColorDepthCustoms[ColorDepthCustomID][0];
			G_Tones = ColorDepthCustoms[ColorDepthCustomID][1];
			B_Tones = ColorDepthCustoms[ColorDepthCustomID][2];
		}
	}
	if (PushFlag_Key_W == TRUE && CheckHitKey(KEY_INPUT_W) == FALSE) {
		PushFlag_Key_W = FALSE;
	}


	if (PushFlag_Key_S == FALSE && CheckHitKey(KEY_INPUT_S) == TRUE) {
		PushFlag_Key_S = TRUE;
		if (ColorDepthCustomID >= 1) {
			ColorDepthCustomID--;
			R_Tones = ColorDepthCustoms[ColorDepthCustomID][0];
			G_Tones = ColorDepthCustoms[ColorDepthCustomID][1];
			B_Tones = ColorDepthCustoms[ColorDepthCustomID][2];
		}
	}
	if (PushFlag_Key_S == TRUE && CheckHitKey(KEY_INPUT_S) == FALSE) {
		PushFlag_Key_S = FALSE;
	}


	if (PushFlag_Key_A == FALSE && CheckHitKey(KEY_INPUT_A) == TRUE) {
		PushFlag_Key_A = TRUE;
		if (Gamma - GammaAdjustStep > 0.1) {
			Gamma += -GammaAdjustStep;
		}
	}
	if (PushFlag_Key_A == TRUE && CheckHitKey(KEY_INPUT_A) == FALSE) {
		PushFlag_Key_A = FALSE;
	}


	if (PushFlag_Key_D == FALSE && CheckHitKey(KEY_INPUT_D) == TRUE) {
		PushFlag_Key_D = TRUE;
		if (Gamma + GammaAdjustStep < 10.0) {
			Gamma += GammaAdjustStep;
		}
	}
	if (PushFlag_Key_D == TRUE && CheckHitKey(KEY_INPUT_D) == FALSE) {
		PushFlag_Key_D = FALSE;
	}


	if (PushFlag_Key_Space == FALSE && CheckHitKey(KEY_INPUT_SPACE) == TRUE) {
		PushFlag_Key_Space = TRUE;
		if (PauseFlag == FALSE && GetMovieStateToGraph(hMovie) == 1) {
			PauseMovieToGraph(hMovie);
		}
		else if (PauseFlag == TRUE && GetMovieStateToGraph(hMovie) == 0) {
			PlayMovieToGraph(hMovie);
		}
		PauseFlag = !PauseFlag;
	}
	if (PushFlag_Key_Space == TRUE && CheckHitKey(KEY_INPUT_SPACE) == FALSE) {
		PushFlag_Key_Space = FALSE;
	}


	if (PushFlag_Key_Left == FALSE && CheckHitKey(KEY_INPUT_LEFT) == TRUE) {
		PushFlag_Key_Left = TRUE;
		SeekMovieToGraph(hMovie, PlayTimeMilliseconds - 5000);
	}
	if (PushFlag_Key_Left == TRUE && CheckHitKey(KEY_INPUT_LEFT) == FALSE) {
		PushFlag_Key_Left = FALSE;
	}


	if (PushFlag_Key_Right == FALSE && CheckHitKey(KEY_INPUT_RIGHT) == TRUE) {
		PushFlag_Key_Right = TRUE;
		SeekMovieToGraph(hMovie, PlayTimeMilliseconds + 5000);
	}
	if (PushFlag_Key_Right == TRUE && CheckHitKey(KEY_INPUT_RIGHT) == FALSE) {
		PushFlag_Key_Right = FALSE;
	}


	if (PushFlag_Key_Z == FALSE && CheckHitKey(KEY_INPUT_Z) == TRUE) {
		PushFlag_Key_Z = TRUE;
		if (itrMovieList != MovieList.begin()) {
			PlayPreviousMovie();
		}
	}
	if (PushFlag_Key_Z == TRUE && CheckHitKey(KEY_INPUT_Z) == FALSE) {
		PushFlag_Key_Z = FALSE;
	}


	if (PushFlag_Key_C == FALSE && CheckHitKey(KEY_INPUT_C) == TRUE) {
		PushFlag_Key_C = TRUE;
		if (itrMovieList != --MovieList.end()) {
			PlayForwardMovie();
		}
	}
	if (PushFlag_Key_C == TRUE && CheckHitKey(KEY_INPUT_C) == FALSE) {
		PushFlag_Key_C = FALSE;
	}


	if (PushFlag_Key_B == FALSE && CheckHitKey(KEY_INPUT_B) == TRUE) {
		PushFlag_Key_B = TRUE;
		SeekMovieToGraph(hMovie, 0);
	}
	if (PushFlag_Key_B == TRUE && CheckHitKey(KEY_INPUT_B) == FALSE) {
		PushFlag_Key_B = FALSE;
	}


	if (PushFlag_Key_Up == FALSE && CheckHitKey(KEY_INPUT_UP) == TRUE) {
		PushFlag_Key_Up = TRUE;
		if (SoundVolume + 5 <= 255) {
			SoundVolume += 5;
			ChangeMovieVolumeToGraph(SoundVolume, hMovie);
		}
	}
	if (PushFlag_Key_Up == TRUE && CheckHitKey(KEY_INPUT_UP) == FALSE) {
		PushFlag_Key_Up = FALSE;
	}


	if (PushFlag_Key_Down == FALSE && CheckHitKey(KEY_INPUT_DOWN) == TRUE) {
		PushFlag_Key_Down = TRUE;
		if (SoundVolume - 5 >= 0) {
			SoundVolume += -5;
			ChangeMovieVolumeToGraph(SoundVolume, hMovie);
		}
	}
	if (PushFlag_Key_Down == TRUE && CheckHitKey(KEY_INPUT_DOWN) == FALSE) {
		PushFlag_Key_Down = FALSE;
	}


	if (PushFlag_Key_R == FALSE && CheckHitKey(KEY_INPUT_R) == TRUE) {
		PushFlag_Key_R = TRUE;
		RepeatOneFlag = !RepeatOneFlag;
	}
	if (PushFlag_Key_R == TRUE && CheckHitKey(KEY_INPUT_R) == FALSE) {
		PushFlag_Key_R = FALSE;
	}


	if (PushFlag_Key_T == FALSE && CheckHitKey(KEY_INPUT_T) == TRUE) {
		PushFlag_Key_T = TRUE;
		RepeatAllFlag = !RepeatAllFlag;
	}
	if (PushFlag_Key_T == TRUE && CheckHitKey(KEY_INPUT_T) == FALSE) {
		PushFlag_Key_T = FALSE;
	}


	if (PushFlag_Key_F == FALSE && CheckHitKey(KEY_INPUT_F) == TRUE) {
		PushFlag_Key_F = TRUE;
		ShuffleFlag = !ShuffleFlag;
	}
	if (PushFlag_Key_F == TRUE && CheckHitKey(KEY_INPUT_F) == FALSE) {
		PushFlag_Key_F = FALSE;
	}


	if (PushFlag_Key_M == FALSE && CheckHitKey(KEY_INPUT_M) == TRUE) {
		PushFlag_Key_M = TRUE;
		MonochromizeFlag = !MonochromizeFlag;
	}
	if (PushFlag_Key_M == TRUE && CheckHitKey(KEY_INPUT_M) == FALSE) {
		PushFlag_Key_M = FALSE;
	}


	if (PushFlag_Key_N == FALSE && CheckHitKey(KEY_INPUT_N) == TRUE) {
		PushFlag_Key_N = TRUE;
		NegativizeFlag = !NegativizeFlag;
	}
	if (PushFlag_Key_N == TRUE && CheckHitKey(KEY_INPUT_N) == FALSE) {
		PushFlag_Key_N = FALSE;
	}


	if (PushFlag_Key_O == FALSE && CheckHitKey(KEY_INPUT_O) == TRUE) {
		PushFlag_Key_O = TRUE;

		if (PauseFlag == FALSE)PauseMovieToGraph(hMovie);

		OpenFiles();
		//std::wstring FileName;
		//std::wstring FilePath;
		//if (GetFilePath(&FileName, &FilePath) == TRUE) {
		//	AddMovieToMovieList(FileName, FilePath);
		//}

		if (PauseFlag == FALSE)PlayMovieToGraph(hMovie);

	}
	if (PushFlag_Key_O == TRUE && CheckHitKey(KEY_INPUT_O) == FALSE) {
		PushFlag_Key_O = FALSE;
	}


	//if (PushFlag_Key_P == FALSE && CheckHitKey(KEY_INPUT_P) == TRUE) {
	//	PushFlag_Key_P = TRUE;

	//	if (PauseFlag == FALSE)PauseMovieToGraph(hMovie);
	//	std::wstring DirectoryPath;
	//	if (GetDirectoryPath(&DirectoryPath) == TRUE) {
	//		DrawFormatString(0, 0, GetColor(255, 0, 0), L"%ls", DirectoryPath.c_str());
	//		ScreenFlip();
	//		WaitKey();
	//	}
	//	if (PauseFlag == FALSE)PlayMovieToGraph(hMovie);

	//}
	//if (PushFlag_Key_P == TRUE && CheckHitKey(KEY_INPUT_P) == FALSE) {
	//	PushFlag_Key_P = FALSE;
	//}


	if (PushFlag_Key_Esc == FALSE && CheckHitKey(KEY_INPUT_ESCAPE) == TRUE) {
		PushFlag_Key_Esc = TRUE;
		PauseMovieToGraph(hMovie);
		//MovieList.clear();
		DeleteGraph(hMovie);
		Scene = INITIAL;
	}
	if (PushFlag_Key_Esc == TRUE && CheckHitKey(KEY_INPUT_ESCAPE) == FALSE) {
		PushFlag_Key_Esc = FALSE;
	}


}