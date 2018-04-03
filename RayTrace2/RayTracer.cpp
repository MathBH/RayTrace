#include "RayTracer.h"
#include "RayCam.h"
#include <vector>
#include <cmath>
#include <stdio.h>
//#include <boost/math/distributions/normal.hpp>
#define AIR_REFRACTIVE_INDEX 1.00029
#define EPSILON 0.000000001 // custom defined for now for the sake of control 

using namespace gmtl;
/*
Set target output
return:
	0 on success
	negative values for invalid output
*/
int RayTracer::setOutput(RTOutput * rndrOut)
{
	renderOutput = rndrOut;
	outputSet = true; //TODO: check for validity
	return 0;
}

int RayTracer::setScene(RTScene * scn)
{
	scene = scn;
	sceneSet = true; //TODO: check for validity
	return 0;
}

//Helper - TODO: refactor
void RayTracer::insertBufferLine(vector<ColorRGB> pixelBuffer, int yIndex) {
	for (int xIndex = 0; xIndex < renderSettings.resolution.width; xIndex++) {
		renderOutput->setValueAt(xIndex, yIndex, pixelBuffer[xIndex]);
	}
}

/*
	For an incidence ray and a collision point, return a set of reflected rays
*/
std::vector<Rayd> reflect(Rayd ray, CollisionPoint colPoint) {
	std::vector<Rayd> reflections = std::vector<Rayd>();
	Vec3d incidenceVec = ray.getDir();
	Vec3d normalVec = colPoint.getNormal();

	if (gmtl::dot(incidenceVec, normalVec) < 0.) // Only reflect if you are on the outside (internal reflection later)
	{
		Vec3d newRayDir = incidenceVec - 2.*dot(incidenceVec, normalVec) * normalVec;
		newRayDir = makeNormal(newRayDir);

		Rayd newRay = Rayd(colPoint.getPosition() + newRayDir * EPSILON, newRayDir);

		reflections.push_back(newRay);
	}

	//Normally distribute rays around the normal

	return reflections;
}

//TODO: refactor and decide if to keep and move in header or find alternative
class RefractionResult {
public:
	bool Exiting;
	std::vector<Rayd> RefractedRays;
	RefractionResult(std::vector<Rayd> refractedRays, bool exiting) : RefractedRays(refractedRays), Exiting(exiting) {}
	~RefractionResult() {}
};

/*
	For an incidence ray and a collision point, return a set of refracted rays
*/
RefractionResult refract(Rayd ray, CollisionPoint colPoint, CollisionPoint * lastCollision) {
	std::vector<Rayd> refractions = std::vector<Rayd>();

	RTMaterial material = colPoint.getMaterial();
	double refractiveIndex = material.getRefractiveIndex();

	Vec3d incidenceVec = ray.getDir();
	Vec3d normalVec = colPoint.getNormal();

	double refractiveRatio;
	double cosIAng;
	double incAng;

	//std::cout << "\nCOL NORMAL: <" << normalVec[0] << " , " << normalVec[1] << " , " << normalVec[2] << ">";

	bool exiting = gmtl::dot(incidenceVec, normalVec) > 0.;// if the dot product of incidence and normal is negative, we are entering the medium, otherwise we are exiting
	if (exiting)
	{
		// Take note of:	n1/n2 ratio
		//					cos(incidenceAngle)
		//					incidenceAngle
		refractiveRatio = refractiveIndex / AIR_REFRACTIVE_INDEX ;
		cosIAng = gmtl::dot(incidenceVec, normalVec);
		incAng = acos(cosIAng);
		//std::cout << "\nEXIT POS:<" << colPoint.getPosition()[0] << " , " << colPoint.getPosition()[1] << " , " << colPoint.getPosition()[2] << ">";
		//std::cout << "\nEXIT DIR:<" << incidenceVec[0] << " , " << incidenceVec[1] << " , " << incidenceVec[2] << ">";
	}
	else
	{
		// Take note of:	n1/n2 ratio
		//					cos(incidenceAngle)
		//					incidenceAngle
		refractiveRatio = AIR_REFRACTIVE_INDEX / refractiveIndex;
		cosIAng = gmtl::dot(incidenceVec, -normalVec);
		incAng = acos(cosIAng);
		//std::cout << "\nENTER POS: <" << colPoint.getPosition()[0] << " , " << colPoint.getPosition()[1] << " , " << colPoint.getPosition()[2] << ">";
		//std::cout << "\nENTER DIR: <" << incidenceVec[0] << " , " << incidenceVec[1] << " , " << incidenceVec[2] << ">";
	}

	// Let RefractedRay = ForwardRay + DownwardRay
	Vec3d forwardRay = refractiveRatio * (incidenceVec + cosIAng*normalVec);
	Vec3d downwardRay = -normalVec * sqrt(1. - pow(refractiveRatio*sin(incAng),2.));

	Vec3d refractionDir = forwardRay + downwardRay;
	refractionDir = makeNormal(refractionDir);
	Point3d refractionOrig = colPoint.getPosition() + refractionDir * EPSILON;
	Rayd refractedRay = Rayd(refractionOrig, refractionDir);

	refractions.push_back(refractedRay);
	//std::cout << "\nREFRACT POS: <" << refractionOrig[0] << " , " << refractionOrig[1] << " , " << refractionOrig[2] << ">";
	//std::cout << "\nREFRACT DIR: <" << refractionDir[0] << " , " << refractionDir[1] << " , " << refractionDir[2] << ">";
	return RefractionResult(refractions,exiting);
}

/*
	Helper that calculates the reflection amount out of 0-1.
	( and so refraction amount = (1. - reflection amount) )
	Using Shlick's approximation : https://en.wikipedia.org/wiki/Schlick%27s_approximation
*/
double calcFresnelValue(double n1, double n2, gmtl::Vec3d normal, gmtl::Vec3d incidence, bool exiting, double reflectivity) {
	double cosTheta;

	if (exiting)
	{
		cosTheta = gmtl::dot(incidence, normal);
	}
	else
	{
		cosTheta = gmtl::dot(incidence, -normal);
	}

	// Special case: if we are moving from a denser medium to a less dense one
	if (n1 > n2) {
		// This is the value that must be quare rooted for the refraction angle.
		// As such, if it is negative there is no refraction and thus we have total
		// internal reflection.
		if (0 > (1. - pow((n1 / n2), 2.)*(1. - pow(cosTheta, 2.)))) {
			return 1.;
		}
	}

	double r0 = pow(((n1 - n2) / (n1 + n2)), 2.);
	double rTheta = r0 + (1. - r0)*pow((1. - cosTheta), 5.);
	return (reflectivity + (1 - reflectivity)*rTheta);
}

//Debug purpose only value
#define DOF 16.
#define RAY_LIFE 200
ColorRGB RayTracer::trace(RTScene * scene, Rayd ray, int life, CollisionPoint * lastCollision)
			//TODO: add parameter int life -	also first try collision with lights and then compare if closest object is closer than closest light
			//									if it hits a light, return the light's diffuse color
			//
			//		get material value for reflectivity
			//		get lights in line of sight
			//		calculate val = dot(normalize(lightPos - colPos), f_normal)*matColor*(1-reflectivity)
			//
			//		if life == 0
			//			return val
			//
			//		get reflectedray
			//		I - 2 * dotProduct(I, N) * N;
			//
			//		return val + reflectivity*trace(reflectedray, scene, life-1)
			//
			//		shading(light)
			//			take angle between normal and incidence of light
			//			use that thing from class and yeah return modified material color

			//		refraction:
			//
			//		get refraction index for material
			//		calculate the new refractedray
			//
			//		get transparency
			//		multiply the last value^ by (1-transparency)
			//		return that^ + transparency*trace(refractedray,scene)
			//
			//
			//		refractedray
			//		todo:
			//
			//
			//
			//		
			//		Beer’s Law https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
			//		
			//		vec3 color = RayTrace(rayPos, rayDir);
			//		vec3 absorb = exp(-OBJECT_ABSORB * absorbDistance);
			//		color *= absorb;
			//		OBJECT_ABSORB is an RGB value that describes how much of each color channel absorbs over distance.For example, a value of(8.0, 2.0, 0.1) would make red get absorbed the fastest, then green, then blue, so would result in a blueish, slightly green color object.
{
	RayCollisionResult collisionResult = scene->tryCollision(ray);
	if (life <= 0) {
		return ambientColor;
	}
	if (collisionResult.getCollided())
	{
		CollisionPoint collisionPoint = collisionResult.getCollisionPoint();
		//Vec3d colToRay = collisionPoint.getPosition() - ray.getOrigin();
		//double distanceToPoint = gmtl::length(colToRay);
		//double colorValue = 1. - (distanceToPoint / DOF);

		RTMaterial material = collisionPoint.getMaterial();
		ColorRGB materialAbsorbtion = collisionPoint.getMaterial().getAbsorbtion();
		ColorRGB reflectionColor = ColorRGB();
		ColorRGB refractionColor = ColorRGB();

		ColorRGB reflectionLight = ColorRGB();
		ColorRGB refractionLight = ColorRGB();

		for (Rayd reflectedRay : reflect(ray, collisionPoint))
		{
			// TODO
			// setup probability distribution and multiply probability by ray values and add them up and average them out
			// (adding the odds as denominator and resultant color as nominator)
			// afterwards, clamp
		
			RayCollisionResult reflectResult = scene->tryCollision(reflectedRay);
			if (reflectResult.getCollided()) {
				reflectionColor = trace(scene, reflectedRay, life - 1); //TODO: adjust to make it take an average for if multy reflect
				//std::cout << "\nReflected ray collided\n";
			}
			else
			{
				//std::cout << "\nUSING SKY DATA\n";
				SkyMapHit skyData = scene->Sky.hitSkyMap(reflectedRay); // restore that thing where you have a light and color component and apply light component here
				reflectionColor = skyData.getColorData();
				reflectionLight = skyData.getLightData(); //TODO: adjust to make it take an average for if multy reflect
			}
		}

		RefractionResult refractionResult = refract(ray, collisionPoint, lastCollision);
		
		CollisionPoint * passOnCol = nullptr;

		double reflectValue;
		double refractiveIndex = material.getRefractiveIndex();
		double reflectivity = material.getReflectivity();
		gmtl::Vec3d colNormal = collisionPoint.getNormal();
		gmtl::Vec3d incidence = ray.getDir();

		bool exiting = refractionResult.Exiting;
		if (exiting) {
			reflectValue = calcFresnelValue(refractiveIndex, AIR_REFRACTIVE_INDEX, colNormal, incidence, exiting, reflectivity);
		}
		else
		{
			reflectValue = calcFresnelValue(AIR_REFRACTIVE_INDEX, refractiveIndex, colNormal, incidence, exiting, reflectivity);
			passOnCol = &collisionPoint;
		}

		for (Rayd refractedRay : refractionResult.RefractedRays) {

			RayCollisionResult refractColResult = scene->tryCollision(refractedRay);
			if (refractColResult.getCollided()) {
				refractionColor = trace(scene, refractedRay, life - 1, passOnCol); //TODO: adjust to make it take an average for if multy refract
				//std::cout << "\nReflected ray collided\n";
			}
			else
			{
				//std::cout << "\nUSING SKY DATA\n";
				SkyMapHit skyData = scene->Sky.hitSkyMap(refractedRay);
				refractionColor = skyData.getColorData();
				refractionLight = skyData.getLightData();//TODO: adjust to make it take an average for if multy refract
			}

			//Vec3d direction = refractedRay.getDir();
			//refractionColor *= trace(scene, refractedRay, life - 1);
			//materialColor = ColorRGB(direction[0]*0.5 + 0.5, direction[1] * 0.5 + 0.5, direction[2] * 0.5 + 0.5);
			//std::cout << "\nColor: <"<< materialColor.R << " , " << materialColor.G << " , " << materialColor.B << ">";
		}

		if (refractionResult.Exiting && lastCollision != nullptr) {
			Vec3d transmissionVector = collisionPoint.getPosition() - lastCollision->getPosition();
			double transmissionLength = gmtl::length(transmissionVector);
			//std::cout << "\nLEN: " << transmissionLength;
			//materialAbsorbtion *= -transmissionLength;
			materialAbsorbtion = ColorRGB(exp(-materialAbsorbtion.R*transmissionLength), exp(-materialAbsorbtion.G*transmissionLength), exp(-materialAbsorbtion.B*transmissionLength));
			//refractionColor *= refractionLight;
			refractionColor *= materialAbsorbtion;
			//lightValue *= exp(-LIGHT_ABSORBSION * transmissionLength);
		}
		//refractionColor *= refractionLight;
		//reflectionColor *= reflectionLight;
		ColorRGB ColorMix = refractionColor*(1. - reflectValue);
		ColorMix += reflectionColor*reflectValue;
		ColorMix *= material.getColor();
		return ColorMix;
	}
	else
	{
		SkyMapHit skyData = scene->Sky.hitSkyMap(ray);
		return skyData.getColorData();
	}
}

int RayTracer::render() //TODO add filepath to render to
{

	if (!outputSet) {
		return -1;
	}
	if (!sceneSet) {
		return -2;
	}
	const int outputWidth = renderSettings.resolution.width;
	const int outputHeight = renderSettings.resolution.height;

	RayCam rayCam = RayCam(scene->camera, renderSettings.camSettings.zNear, renderSettings.camSettings.fov,
		renderSettings.antiAlias, renderSettings.resolution);

	RayIterator rayIterator = rayCam.iterator();
	renderOutput->initialize(outputWidth, outputHeight);

	std::cout << "\nRENDERING\n---------";
	for (int yIndex = 0; yIndex < outputHeight; yIndex++)
	{
		for (int xIndex = 0; xIndex < outputWidth; xIndex++)
		{
			//std::cout << "\nposition: [" << xIndex << " , " << yIndex << "]";
			RayPacket pixelRays = rayIterator.getAt(xIndex, yIndex);
			ColorRGB colorOut = ColorRGB(0.0,0.0,0.0);
			for (Rayd ray : pixelRays) {
				colorOut += trace(scene, ray, RAY_LIFE);
			}
			colorOut = colorOut/(pow((double)renderSettings.antiAlias, 2.));
			//std::cout << "\n[" << colorOut.R << ", " << colorOut.G << ", " << colorOut.B << "]";
			renderOutput->setValueAt(xIndex, yIndex, colorOut);
		}
		renderOutput->commit();
		std::cout << "\nprogress: " << (yIndex/(double)outputHeight)*100. << "%";
	}
	return 0;
}