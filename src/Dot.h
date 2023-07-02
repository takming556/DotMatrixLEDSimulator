#pragma once

class Dot {
private:
	unsigned char R;
	unsigned char G;
	unsigned char B;
public:
	Dot(unsigned char R, unsigned char G, unsigned char B);
	void Monochromize();
	void Negativize();
	void Quantize();
	void GammaEqualize();
};