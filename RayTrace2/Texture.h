#pragma once
#include "ParameterTypes.h"
#include <string>

class Texture {
public:
	virtual ColorRGB getValueAt(int x, int y) = 0;
};