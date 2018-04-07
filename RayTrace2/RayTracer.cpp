#include "RayTracer.h"
#include "RayCam.h"
#include <vector>
#include <cmath>
#include <stdio.h>
#define AIR_REFRACTIVE_INDEX 1.00029
#define EPSILON 0.000000001 // custom defined for now for the sake of control 
#define RED_LUMINANCE 0.299
#define GREEN_LUMINANCE 0.587
#define BLUE_LUMINANCE 0.114

using namespace gmtl;

/*
	Helper that calculates the Reflection Coefficient

	Equations from "Reflections and Refractions in Ray Tracing"
	(Bram de Greve 2006).
*/
double R(Rayd ray, CollisionPoint collisionPoint) {

	//double n1, double n2, gmtl::Vec3d normal, gmtl::Vec3d incidence, bool exiting, double reflectivity
	MatSample material = collisionPoint.getMaterial();
	gmtl::Vec3d normal = collisionPoint.getNormal();
	gmtl::Vec3d incidence = ray.getDir();
	incidence = gmtl::makeNormal(incidence);
	double reflectivity = material.getReflectivity();
	double n1;
	double n2;

	double cosTheta;
	n1 = AIR_REFRACTIVE_INDEX;
	n2 = material.getRefractiveIndex();
	cosTheta = gmtl::dot(incidence, -normal);

	// Special case: if we are moving from a denser medium to a less dense one
	if (n1 > n2) {
		// We need to use the cos of the angle of transmission in this case
		// Solving for it gives us the quare root of the following equation
		double cosThetaSqrd = 1. - pow((n1 / n2), 2.)*(1. - pow(cosTheta, 2.));

		// A short hand for total internal reflection is checking if this
		// value is negative
		if (0. > cosThetaSqrd) {
			return 1.;
		}

		// If we do not have total internal reflection, use the new cos value
		cosTheta = sqrt(cosThetaSqrd);
	}

	double r0 = pow(((n1 - n2) / (n1 + n2)), 2.);
	double rTheta = r0 + (1. - r0)*pow((1. - cosTheta), 5.);
	return (reflectivity + (1. - reflectivity)*rTheta);
}

/*
	Helper that calculates the Transmission Coefficient

	Equations from "Reflections and Refractions in Ray Tracing"
	(Bram de Greve 2006).
*/
double T(Rayd ray, CollisionPoint collisionPoint) {
	return 1. - R(ray, collisionPoint);
}

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

	TODO: remove the multi thing
*/
std::vector<Rayd> reflect(Rayd ray, CollisionPoint colPoint) {
	std::vector<Rayd> reflections = std::vector<Rayd>();
	Vec3d incidenceVec = ray.getDir();
	Vec3d normalVec = colPoint.getNormal();

	if (gmtl::dot(incidenceVec, normalVec) < 0.) // If on outside
	{
		Vec3d newRayDir = incidenceVec - 2.*dot(incidenceVec, normalVec) * normalVec;
		newRayDir = makeNormal(newRayDir);

		Rayd newRay = Rayd(colPoint.getPosition() + newRayDir * EPSILON, newRayDir);

		reflections.push_back(newRay);
	}

	return reflections;
}

/*
	For an incidence ray and a collision point, return the refracted ray at this point
	on the object.
*/
Rayd _refract(Rayd ray, CollisionPoint colPoint) {
	MatSample material = colPoint.getMaterial();
	double refractiveIndex = material.getRefractiveIndex();

	Vec3d incidenceVec = ray.getDir();
	Vec3d normalVec = colPoint.getNormal();

	double refractiveRatio;
	double cosIAng;
	double incAng;

	bool exiting = gmtl::dot(incidenceVec, normalVec) > 0.;// if the dot product of incidence and normal is positive, we are exiting the medium
	if (exiting)
	{
		// Take note of:	n1/n2 ratio
		//					cos(incidenceAngle)
		//					incidenceAngle
		refractiveRatio = refractiveIndex / AIR_REFRACTIVE_INDEX;
		cosIAng = gmtl::dot(incidenceVec, normalVec);
		incAng = acos(cosIAng);
	}
	else // Otherwise, we are entering
	{
		// Take note of:	n1/n2 ratio
		//					cos(incidenceAngle)
		//					incidenceAngle
		refractiveRatio = AIR_REFRACTIVE_INDEX / refractiveIndex;
		cosIAng = gmtl::dot(incidenceVec, -normalVec);
		incAng = acos(cosIAng);
	}

	// Let RefractedRay = ForwardRay + DownwardRay
	Vec3d forwardRay = refractiveRatio * (incidenceVec + cosIAng * normalVec);
	Vec3d downwardRay = -normalVec * sqrt(1. - pow(refractiveRatio*sin(incAng), 2.));

	Vec3d refractionDir = forwardRay + downwardRay;
	refractionDir = makeNormal(refractionDir);
	Point3d refractionOrig = colPoint.getPosition() + refractionDir * EPSILON;
	Rayd refractedRay = Rayd(refractionOrig, refractionDir);

	return refractedRay;
}

/*
	For an incidence ray and a collision point, return the refracted rays from the other side
	of the object.
	
	Assuming no one-sided objects

	TODO: remove the multi thing
*/
std::vector<RefractionResult> RayTracer::refract(Rayd ray, CollisionPoint colPoint, CollisionPoint * lastCollision) {
	std::vector<RefractionResult> refractions = std::vector<RefractionResult>();
	Rayd travelRay = _refract(ray, colPoint);
	RayCollisionResult colResult = scene->tryCollision(travelRay);
	if (colResult.getCollided()) {
		CollisionPoint point2 = colResult.getCollisionPoint();
		Vec3d travelVector = point2.getPosition() - colPoint.getPosition();
		double travelDistance = gmtl::length(travelVector);
		Rayd newRay = _refract(travelRay, colResult.getCollisionPoint());
		RefractionResult result = RefractionResult(newRay, travelDistance);
		refractions.push_back(result);
	}
	return refractions;
}

ColorRGB RayTracer::evaluateDiffuse(gmtl::Point3d objPos, gmtl::Vec3d objNorm)
{
	ColorRGB diffuse = ColorRGB();
	for (RTLight * light : scene->lights)
	{
		gmtl::Vec3d incidence = light->getPosition() - objPos;
		incidence = gmtl::makeNormal(incidence);
		
		Rayd shadowRay = Rayd(objPos + incidence * EPSILON, incidence);
		RayCollisionResult colResult = scene->tryCollision(shadowRay);
		if (!colResult.getCollided()) 
		{
			double cosTheta = gmtl::dot(incidence, objNorm);
			ColorRGB cosDiff = light->getDiffuse()*cosTheta;
			diffuse += cosDiff*light->getFalloff(objPos);
		}
	}
	return diffuse;
}

ColorRGB RayTracer::evaluateSpec(gmtl::Point3d objPos, gmtl::Vec3d objNorm, MatSample material)
{
	ColorRGB spec = ColorRGB();
	for (RTLight * light : scene->lights)
	{
		gmtl::Vec3d incidence = light->getPosition() - objPos;
		incidence = gmtl::makeNormal(incidence);
		
		Rayd shadowRay = Rayd(objPos + incidence*EPSILON, incidence);
		RayCollisionResult colResult = scene->tryCollision(shadowRay);
		if (!colResult.getCollided()) 
		{
			double cosTheta = gmtl::dot(incidence, objNorm);
			//std::cout << "\ncos: " << cosTheta;
			ColorRGB cosSpec = light->getSpecular() * pow(cosTheta, material.getShininess());
			ColorRGB newSpec = cosSpec *light->getFalloff(objPos);
			spec += newSpec;
		}
	}

	return spec;
}

ColorRGB RayTracer::trace(RTScene * scene, Rayd ray, int life, CollisionPoint * lastCollision)
{
	RayCollisionResult collisionResult = scene->tryCollision(ray);
	if (life <= 0) {
		return scene->ambientColor;
	}
	if (collisionResult.getCollided())
	{
		CollisionPoint collisionPoint = collisionResult.getCollisionPoint();
		gmtl::Point3d colPos = collisionPoint.getPosition();
		gmtl::Vec3d colNormal = collisionPoint.getNormal();
		gmtl::Vec3d incidence = ray.getDir();
		incidence = gmtl::makeNormal(incidence);

		MatSample material = collisionPoint.getMaterial();
		ColorRGB materialAbsorbtion = collisionPoint.getMaterial().getAbsorbtion();
		ColorRGB reflectionColor = ColorRGB();
		ColorRGB refractionColor = ColorRGB();

		double r = R(ray, collisionPoint);
		double t = T(ray, collisionPoint);

		ColorRGB outputColor = ColorRGB();

		// evaluate lighting values
		ColorRGB ambientColor = material.getAmbient();
		ambientColor *= scene->ambientColor;
		ColorRGB diffuseColor = evaluateDiffuse(colPos, colNormal)*t;
		diffuseColor *= material.getDiffuse();
		ColorRGB specColor = evaluateSpec(colPos, colNormal, material)*r;
		specColor *= material.getSpecular();

		outputColor += ambientColor;
		outputColor += diffuseColor;
		outputColor += specColor;

		// reflect ray
		for (Rayd reflectedRay : reflect(ray, collisionPoint))
		{
			ColorRGB reflectionBuffer = trace(scene, reflectedRay, life - 1);
			outputColor += reflectionBuffer * r;
		}


		// refract ray
		for (RefractionResult refraction : refract(ray, collisionPoint, nullptr)) // TODO: clean boiler plate
		{
			double travelDistance = refraction.Distance;
			Rayd refractedRay = refraction.RefractedRay;
			ColorRGB color = trace(scene, refractedRay, life - 1, nullptr);
			ColorRGB absorbtion = ColorRGB(exp(-materialAbsorbtion.R*travelDistance), exp(-materialAbsorbtion.G*travelDistance), exp(-materialAbsorbtion.B*travelDistance));
			color *= absorbtion;

			outputColor += color * t;
		}
		return outputColor;
	}
	else
	{
		RayCollisionResult skyColResult = scene->Sky.hitSkyMap(ray);
		if (skyColResult.getCollided())
		{
			CollisionPoint skyCollision = skyColResult.getCollisionPoint();
			MatSample skyMat = skyCollision.getMaterial();
			return skyMat.getDiffuse();
		}

		return scene->ambientColor;
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
			RayPacket pixelRays = rayIterator.getAt(xIndex, yIndex);
			ColorRGB colorOut = ColorRGB(0.0,0.0,0.0);
			for (Rayd ray : pixelRays) {
				colorOut += trace(scene, ray, renderSettings.camSettings.RayLife);
			}
			colorOut = colorOut/(pow((double)renderSettings.antiAlias, 2.));
			renderOutput->setValueAt(xIndex, yIndex, colorOut);
		}
		renderOutput->commit();
		//std::cout << "\nprogress: " << (yIndex/(double)outputHeight)*100. << "%";
	}
	return 0;
}