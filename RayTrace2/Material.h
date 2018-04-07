#pragma once
#include "ParameterTypes.h"
#include "AllTextures.h"
#include "UVMapped.h"
#include <gmtl/Generate.h>
#define DEFAULT_RTMAT_DIFFUSE ColorRGB(1.0,1.0,1.0)
#define DEFAULT_RTMAT_ABSORBTION ColorRGB(0.0,0.0,0.0)
#define DEFAULT_RTMAT_REFLECTIVITY 0.5
#define DEFAULT_RTMAT_REFRACTIVE_INDEX 1.52
#define DEFAULT_RTMAT_LIGHT_VALUE 0.
#define DEFAULT_SHININESS 3
#define NORMAL_WEIGHT_MIN 0.
#define NORMAL_WEIGHT_MAX 1.

/*
	This class holds the various values of a Material at a given point
*/
class MatSample
{
private:
	ColorRGB Ambient;
	ColorRGB Diffuse;
	ColorRGB Specular;
	ColorRGB Absorbtion;
	gmtl::Vec3d Normal;
	double refractiveIndex;
	double LightValue;
	double reflectivity;
	double shininess;
	double NormalWeight;
	bool NormalActive;
	//double refractiveIndex;
public:
	MatSample() : Diffuse(DEFAULT_RTMAT_DIFFUSE), refractiveIndex(DEFAULT_RTMAT_REFRACTIVE_INDEX), Absorbtion(DEFAULT_RTMAT_ABSORBTION), LightValue(DEFAULT_RTMAT_LIGHT_VALUE), shininess(DEFAULT_SHININESS), NormalWeight(NORMAL_WEIGHT_MIN) , NormalActive(false){}
	MatSample(ColorRGB diffuse, double refract) : Diffuse(diffuse), refractiveIndex(refract), Absorbtion(DEFAULT_RTMAT_ABSORBTION), LightValue(DEFAULT_RTMAT_LIGHT_VALUE), shininess(DEFAULT_SHININESS), NormalWeight(NORMAL_WEIGHT_MIN), NormalActive(false) {}
	MatSample(ColorRGB diffuse, ColorRGB abs, double refract) : Diffuse(diffuse), refractiveIndex(refract), Absorbtion(abs), LightValue(DEFAULT_RTMAT_LIGHT_VALUE), shininess(DEFAULT_SHININESS), NormalWeight(NORMAL_WEIGHT_MIN), NormalActive(false) {}
	MatSample(ColorRGB diffuse, ColorRGB abs, double refract, double reflect) : Diffuse(diffuse), refractiveIndex(refract), Absorbtion(abs), reflectivity(reflect), shininess(DEFAULT_SHININESS), NormalWeight(NORMAL_WEIGHT_MIN), NormalActive(false) {}

	void setRefractiveIndex(double ref) { refractiveIndex = ref; }
	double getRefractiveIndex() { return refractiveIndex; }

	void setNormal(gmtl::Vec3d normal) { Normal = normal; NormalActive = true; }
	gmtl::Vec3d getNormal() { return Normal; }
	/*
		Get/Set Normal Weight

		Values are from 1 - 0 (inclussive)
		Determines the degree to which the material normal will take
		precedence over the geometry normal when shading.
	*/
	void setNormalWeight(double normalWeight)
	{ 
		NormalWeight = normalWeight;
		if (NormalWeight > NORMAL_WEIGHT_MAX)
			NormalWeight = NORMAL_WEIGHT_MAX;
		if (NormalWeight < NORMAL_WEIGHT_MIN)
			NormalWeight = NORMAL_WEIGHT_MIN;
	}
	double getNormalWeight() { return NormalWeight; }

	bool normalIsActive() { return NormalActive; }

	void setAbsorbtion(ColorRGB abs) { Absorbtion = abs; }
	ColorRGB getAbsorbtion() { return Absorbtion; }

	void setAmbient(ColorRGB ambient) { Ambient = ambient; }
	ColorRGB getAmbient() { return Ambient; }

	void setDiffuse(ColorRGB dif) { Diffuse = dif; }
	ColorRGB getDiffuse() { return Diffuse; }

	void setSpecular(ColorRGB spec) { Specular = spec; }
	ColorRGB getSpecular() { return Specular; }

	void setReflectivity(double ref) { reflectivity = ref; }
	double getReflectivity() { return reflectivity; }

	void setShininess(double shiny) { shininess = shiny; }
	double getShininess() { return shininess; }
};

/*
	Read-Only Interface for Materials
*/
class Material
{
public:
	virtual MatSample getSample(UVCoord coord) = 0;
};

/*
	Basic Material implementation
*/
class BasicMaterial : public Material
{
private:
	MatSample MatVal;
public:
	BasicMaterial() {}
	~BasicMaterial() {}

	void setDiffuse(ColorRGB dif) { MatVal.setDiffuse(dif); }
	void setSpecular(ColorRGB spec) { MatVal.setSpecular(spec); }
	void setRefractiveIndex(double ref) { MatVal.setRefractiveIndex(ref); }
	void setReflectivity(double ref) { MatVal.setReflectivity(ref); }
	void setShininess(double shiny) { MatVal.setShininess(shiny); }
	void setAbsorbtion(ColorRGB abs) { MatVal.setAbsorbtion(abs); }

	MatSample getSample(UVCoord coord) override
	{
		return MatVal;
	}
};

/*
	Texture Material

	This Material works with Textures for its fields
*/
class TextureMaterial : public Material
{
private:
	Texture * AmbientMap;
	Texture * DiffuseMap;
	Texture * SpecularMap;
	Texture * NormalMap;
	ColorRGB Ambient;
	ColorRGB Diffuse;
	ColorRGB Specular;
	ColorRGB Absorbtion;
	double RefractiveIndex;
	double Reflectivity;
	double Shininess;
	double NormalWeight;
	bool NormalActive;

public:
	TextureMaterial() : NormalActive(false) {}
	~TextureMaterial() {}

	void setAmbientMap(Texture * ambient) { AmbientMap = ambient; }
	void setDiffuseMap(Texture * diffuse) { DiffuseMap = diffuse; }
	void setSpecularMap(Texture * specular) { SpecularMap = specular; }
	void setNormalMap(Texture * normal) { NormalMap = normal; NormalActive = (NormalMap != nullptr); }
	void setAmbient(ColorRGB ambient) { Ambient = ambient; }
	void setDiffuse(ColorRGB diffuse) { Diffuse = diffuse; }
	void setSpecular(ColorRGB specular) { Specular = specular; }
	void setAbsorbtion(ColorRGB absorbtion) { Absorbtion = absorbtion; }
	void setRefractiveIndex(double refractiveIndex) { RefractiveIndex = refractiveIndex; }
	void setReflectivity(double reflectivity) { Reflectivity = reflectivity; }
	void setShininess(double shininess) { Shininess = shininess; }
	void setNormalWeight(double normalWeight) { NormalWeight = normalWeight; }

	MatSample getSample(UVCoord coord) override
	{
		//std::cout << "\nAuv: " << coord.U << " " << coord.V;
		MatSample matSample = MatSample();
		if (NormalActive)
		{
			ColorRGB n = NormalMap->getValueAt(coord);
			gmtl::Vec3d normalVec = gmtl::Vec3d((0.5 - n.R)*2., n.G, (0.5 - n.B) * 2.);
			normalVec = gmtl::makeNormal(normalVec);

			matSample.setNormalWeight(NormalWeight);
			matSample.setNormal(normalVec);
		}
		if (AmbientMap != nullptr)
		{
			ColorRGB a = AmbientMap->getValueAt(coord);
			a *= Ambient;

			matSample.setAmbient(a);
		}
		else
		{
			matSample.setAmbient(Ambient);
		}

		if (DiffuseMap != nullptr)
		{
			ColorRGB d = DiffuseMap->getValueAt(coord);
			d *= Diffuse;

			//std::cout << "\ndiff: " << d.R << " " << d.G << " " << d.B;
			matSample.setDiffuse(d);
		}
		else
		{
			matSample.setDiffuse(Diffuse);
		}
		if (SpecularMap != nullptr)
		{
			ColorRGB s = SpecularMap->getValueAt(coord);
			s *= Specular;
			matSample.setSpecular(s);
		}
		else
		{
			matSample.setSpecular(Specular);
		}
		matSample.setAbsorbtion(Absorbtion);
		matSample.setRefractiveIndex(RefractiveIndex);
		matSample.setReflectivity(Reflectivity);
		matSample.setShininess(Shininess);
		return matSample;
	}
};