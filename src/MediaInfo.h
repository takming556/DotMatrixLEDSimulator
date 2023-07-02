#pragma once
#include <filesystem>
#include <string>
#include <chrono>
#include <utility>

namespace fs = std::filesystem;
using std::u8string;
using std::chrono::seconds;
using std::pair;
using namespace std::literals::chrono_literals;


class MediaInfo {
private:
	const fs::path FilePath;
	const u8string FileName;
	const unsigned int TotalFrames;
	const seconds Duration;
	const float FrameRate;
	const enum AudioCodec AudioCodec;
	const unsigned int AudioSamplingRate;
	const unsigned int AudioBitRate;
	const enum VideoCodec VideoCodec;
	const unsigned int VideoBitRate;
	const pair<unsigned int, unsigned int> ImageResolution;
	const float DataSize;
public:
	MediaInfo(u8string);
};