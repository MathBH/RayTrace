#pragma once
#define ZERO_VAL 0.0
#ifndef DEFAULT_RESOLUTION_WIDTH
#define DEFAULT_RESOLUTION_WIDTH 640
#endif
#ifndef DEFAULT_RESOLUTION_HEIGHT
#define DEFAULT_RESOLUTION_HEIGHT 480
#endif

class ColorRGB {
public:
	double R, G, B;
	ColorRGB() : R(ZERO_VAL), G(ZERO_VAL), B(ZERO_VAL) {}
	ColorRGB(double r, double g, double b) : R(r), G(g), B(b) {}
	~ColorRGB() {}

	ColorRGB& operator+(const ColorRGB& otherColor) {
		ColorRGB color(R + otherColor.R, G + otherColor.G, B + otherColor.B);
		return color;
	}

	ColorRGB& operator+=(const ColorRGB& otherColor) {
		this->R += otherColor.R;
		this->G += otherColor.G;
		this->B += otherColor.B;
		return *this;
	}

	ColorRGB& operator=(const ColorRGB& otherColor) {
		R = otherColor.R;
		G = otherColor.G;
		B = otherColor.B;
		return *this;
	}

	ColorRGB& operator/(double value) {
		R = R/value;
		G = G/value;
		B = B/value;
		return *this;
	}

	ColorRGB& operator*(double value) {
		//R = R * value;
		//G = G * value;
		//B = B * value
		ColorRGB color(R * value, G * value, B * value);
		return color;
	}
	//TODO: + operator
	//		check if vector of these will initialize them all at 0 or if u need to do that urself
};

class Dimensions2D {
public:
	double X;
	double Y;

	Dimensions2D() : X(DEFAULT_RESOLUTION_WIDTH), Y(DEFAULT_RESOLUTION_HEIGHT) {}
	Dimensions2D(int x, int y) : X(x), Y(y) {}
};

class ResolutionSettings {
public:
	double width;
	double height;

	ResolutionSettings() : width(DEFAULT_RESOLUTION_WIDTH), height(DEFAULT_RESOLUTION_HEIGHT) {}
	ResolutionSettings(int w, int h) : width(w), height(h) {}
	ResolutionSettings(Dimensions2D dim) : width(dim.X), height(dim.Y) {}
	~ResolutionSettings(){}
};