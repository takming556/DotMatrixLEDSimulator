#pragma once
#include <vector>
#include <list>
#include <chrono>
#include <utility>
#include "MediaInfo.h"
#include "Dot.h"

using std::vector;
using std::list;
using std::pair;
using std::chrono::milliseconds;
using namespace std::literals::chrono_literals;

class Player {
private:
	static vector<vector<vector<Dot>>> Movie;
	static list<MediaInfo> Queue;
	static list<MediaInfo>::iterator NowPlaying;
	static enum PlayStatus Status;
	static milliseconds PlayPosition;
	static pair<unsigned char, unsigned char> MatrixSize;
	static unsigned char SoundVolume;
	static enum ColorDepthID ColorDepth;
	static float Gamma;
	static unsigned int ActualFPS;
	static float InstantFPS;
	static bool isMonochromized;
	static bool isNegativized;
	static bool isFullscreen;
	static bool isRepeatOne;
	static bool isRepeatAll;
	static bool isPaused;
	static bool isStatusDisplayed;
	static bool isKeyGuideDisplayed;
	static bool isQueueDisplayed;
	Player() {}
	static void ReactToKeyInput();
	static void DrawDots();
	static void DrawStatus();
	static void DrawKeyGuide();
	static void DrawQueue();
	static void SwitchWindowMode();
	static void MeasureFPS();
	static void ShuffleQueue();
public:
	static void Initialize();
	static void Update();
	static void Enqueue();
};