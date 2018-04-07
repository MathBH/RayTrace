#pragma once
#include "Material.h"

class HasMaterial {
	virtual void setMaterial(Material * material) = 0;
};