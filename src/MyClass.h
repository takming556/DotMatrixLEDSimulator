#pragma once

using std::wstring;


class Movie {
public:
	Movie(wstring FileName, wstring FilePath, int PlayResumeFrame, int TotalFrames, double FPS);
	Movie();
	wstring FileName;
	wstring FilePath;
	int PlayResumeFrame;
	int TotalFrames;
	double FPS;
	int TotalSeconds;
};
inline Movie::Movie(wstring FileName, wstring FilePath, int PlayResumeFrame, int TotalFrames, double FPS) {
	this->FileName = FileName;
	this->FilePath = FilePath;
	this->PlayResumeFrame = PlayResumeFrame;
	this->TotalFrames = TotalFrames;
	this->FPS = FPS;
	this->TotalSeconds = TotalFrames / FPS;
}
inline Movie::Movie() {}


class Pixel {
public:
	Pixel(unsigned char Ir, unsigned char Ig, unsigned char Ib, int Ia); //当クラスのインスタンスが作成されたときに引数が与えられた場合に実行されるコンストラクタ
	Pixel() { r = 127; g = 127; b = 127; a = 0; }                    //当クラスのインスタンスが作成されたときに引数が与えられなかった場合に実行されるコンストラクタ
	unsigned char r;
	unsigned char g;
	unsigned char b;
	int a;
	void reduct_color(int r_tones, int g_tones, int b_tones);
	void correct_gamma(double gamma);
	void monochromize();
	void negativize();
};
inline Pixel::Pixel(unsigned char Ir, unsigned char Ig, unsigned char Ib, int Ia) {
	r = Ir;
	g = Ig;
	b = Ib;
	a = Ia;
}
inline void Pixel::reduct_color(int r_tones, int g_tones, int b_tones) {
	r = r / (256 / r_tones) * (256 / r_tones);
	g = g / (256 / g_tones) * (256 / g_tones);
	b = b / (256 / b_tones) * (256 / b_tones);
}
inline void Pixel::correct_gamma(double gamma) {
	r = 255 * pow((double)r / 255.0, 1.0 / gamma);
	g = 255 * pow((double)g / 255.0, 1.0 / gamma);
	b = 255 * pow((double)b / 255.0, 1.0 / gamma);
}
inline void Pixel::monochromize(void) {
	int brightness_average = (r + g + b) / 3;
	r = brightness_average;
	g = brightness_average;
	b = brightness_average;
}
inline void Pixel::negativize(void) {
	r = 255 - r;
	g = 255 - g;
	b = 255 - b;
}