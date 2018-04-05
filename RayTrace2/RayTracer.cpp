#include "RayTracer.h"
#include "RayCam.h"
#include <vector>
#include <cmath>
#include <stdio.h>
//#include <boost/math/distributions/normal.hpp>
#define AIR_REFRACTIVE_INDEX 1.00029
#define EPSILON 0.000000001 // custom defined for now for the sake of control 
#define RED_LUMINANCE 0.299
#define GREEN_LUMINANCE 0.587
#define BLUE_LUMINANCE 0.114

using namespace gmtl;

//enum RAY_STATUS{ EXITING, ENTERING, PARALLEL};

/*
	Helper function that evaluates if a ray is entering at a collision point, exiting or parallel to the face
*/
//RAY_STATUS status(Rayd ray, CollisionPoint collisionPoint) {
//	gmtl::Vec3d normal = collisionPoint.getNormal();
//	gmtl::Vec3d incidence = ray.getDir();
//	incidence = gmtl::makeNormal(incidence);
//	double dot = gmtl::dot(incidence, normal);
//	if (dot < 0.) { return ENTERING; }
//	if (dot > 0.) { return EXITING; }
//	return PARALLEL;
//}

/*
	Helper that calculates the Reflection Coefficient

	Equations from "Reflections and Refractions in Ray Tracing"
	(Bram de Greve 2006).
*/
double R(Rayd ray, CollisionPoint collisionPoint) {

	//double n1, double n2, gmtl::Vec3d normal, gmtl::Vec3d incidence, bool exiting, double reflectivity
	RTMaterial material = collisionPoint.getMaterial();
	gmtl::Vec3d normal = collisionPoint.getNormal();
	gmtl::Vec3d incidence = ray.getDir();
	incidence = gmtl::makeNormal(incidence);
	double reflectivity = material.getReflectivity();
	double n1;
	double n2;

	double cosTheta;

	//double dot = gmtl::dot(incidence, normal);
	//bool exiting = !(dot < 0.);
	//if (exiting)
	//{
	//	n1 = material.getRefractiveIndex();
	//	n2 = AIR_REFRACTIVE_INDEX;
	//	cosTheta = gmtl::dot(incidence, normal);
	//}
	//else
	//{
		n1 = AIR_REFRACTIVE_INDEX;
		n2 = material.getRefractiveIndex();
		cosTheta = gmtl::dot(incidence, -normal);
	//}

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
	else
	{ // Internal reflection
		//Vec3d newRayDir = incidenceVec - 2.*dot(incidenceVec, -normalVec) * -normalVec;
		//newRayDir = makeNormal(newRayDir);
		//
		//Rayd newRay = Rayd(colPoint.getPosition() + newRayDir * EPSILON, newRayDir);
		//
		//reflections.push_back(newRay);
	}

	//Normally distribute rays around the normal

	return reflections;
}

/*
	For an incidence ray and a collision point, return the refracted ray at this point
	on the object.
*/
Rayd _refract(Rayd ray, CollisionPoint colPoint) {
	RTMaterial material = colPoint.getMaterial();
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
		//std::cout << "\nEXIT POS:<" << colPoint.getPosition()[0] << " , " << colPoint.getPosition()[1] << " , " << colPoint.getPosition()[2] << ">";
		//std::cout << "\nEXIT DIR:<" << incidenceVec[0] << " , " << incidenceVec[1] << " , " << incidenceVec[2] << ">";
	}
	else // Otherwise, we are entering
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
*/
std::vector<RefractionResult> RayTracer::refract(Rayd ray, CollisionPoint colPoint, CollisionPoint * lastCollision) {
	std::vector<RefractionResult> refractions = std::vector<RefractionResult>();

	//RTMaterial material = colPoint.getMaterial();
	//double refractiveIndex = material.getRefractiveIndex();
	//
	//Vec3d incidenceVec = ray.getDir();
	//Vec3d normalVec = colPoint.getNormal();
	//
	//double refractiveRatio;
	//double cosIAng;
	//double incAng;

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
	
	//std::cout << "\nCOL NORMAL: <" << normalVec[0] << " , " << normalVec[1] << " , " << normalVec[2] << ">";

	

	//refractions.push_back(refractedRay);
	//std::cout << "\nREFRACT POS: <" << refractionOrig[0] << " , " << refractionOrig[1] << " , " << refractionOrig[2] << ">";
	//std::cout << "\nREFRACT DIR: <" << refractionDir[0] << " , " << refractionDir[1] << " , " << refractionDir[2] << ">";
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
	//ColorRGB eh = diffuse * (1. / (double)counter);
	//std::cout << "\nDiff: <" << eh.R << " , " << eh.G << " , " << eh.B << ">";
	return diffuse;
}

ColorRGB RayTracer::evaluateSpec(gmtl::Point3d objPos, gmtl::Vec3d objNorm, RTMaterial material)
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
	//if (spec.R > 0.) {
	//	std::cout << "\nSpec: <" << spec.R << " , " << spec.G << " , " << spec.B << ">";
	//}
	//ColorRGB eh = spec * (1. / (double)counter);
	//std::cout << "\nSpec: <" << eh.R << " , " << eh.G << " , " << eh.B << ">";
	return spec;
}

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
		return scene->ambientColor;
	}
	if (collisionResult.getCollided())
	{
		CollisionPoint collisionPoint = collisionResult.getCollisionPoint();
		gmtl::Point3d colPos = collisionPoint.getPosition();
		gmtl::Vec3d colNormal = collisionPoint.getNormal();
		gmtl::Vec3d incidence = ray.getDir();
		incidence = gmtl::makeNormal(incidence);
		//double dot = gmtl::dot(incidence, colNormal);
		//bool exiting = !(dot < 0.);
		//Vec3d colToRay = collisionPoint.getPosition() - ray.getOrigin();
		//double distanceToPoint = gmtl::length(colToRay);
		//double colorValue = 1. - (distanceToPoint / DOF);

		RTMaterial material = collisionPoint.getMaterial();
		ColorRGB outputColor = ColorRGB();
		ColorRGB materialAbsorbtion = collisionPoint.getMaterial().getAbsorbtion();
		ColorRGB reflectionColor = ColorRGB();
		ColorRGB refractionColor = ColorRGB();

		double r = R(ray, collisionPoint);
		double t = T(ray, collisionPoint);
		//ColorRGB reflectionLight = ColorRGB();
		//ColorRGB refractionLight = ColorRGB();

		// evaluate lighting values
		ColorRGB diffuseColor = evaluateDiffuse(colPos, colNormal)*t;
		diffuseColor *= material.getDiffuse();
		ColorRGB specColor = evaluateSpec(colPos, colNormal, material)*r;
		specColor *= material.getSpecular();
		//ColorRGB ambientColor = scene->ambientColor;
		outputColor += diffuseColor;
		outputColor += specColor;

		// reflect ray
		for (Rayd reflectedRay : reflect(ray, collisionPoint))
		{
			ColorRGB reflectionBuffer = trace(scene, reflectedRay, life - 1); //TODO: adjust to make it take an average for if multi reflect
			outputColor += reflectionBuffer * r;

			// TODO
			// setup probability distribution and multiply probability by ray values and add them up and average them out
			// (adding the odds as denominator and resultant color as nominator)
			// afterwards, clamp
		
			//RayCollisionResult reflectResult = scene->tryCollision(reflectedRay);
			//if (reflectResult.getCollided()) {
			//	CollisionPoint collisionPoint = reflectResult.getCollisionPoint();
			//	r = R(reflectedRay, collisionPoint);
			//	reflectionSubBuffer = trace(scene, reflectedRay, life - 1); //TODO: adjust to make it take an average for if multy reflect
			//	//std::cout << "\nReflected ray collided\n";
			//}
			//else
			//{
			//	//std::cout << "\nUSING SKY DATA\n";
			//	RayCollisionResult skyData = scene->Sky.hitSkyMap(reflectedRay); // restore that thing where you have a light and color component and apply light component here
			//	CollisionPoint collisionPoint = skyData.getCollisionPoint();
			//	r = R(reflectedRay, collisionPoint);
			//	reflectionSubBuffer = collisionPoint.getMaterial().getColor();
			//	//reflectionColor *= skyData.getLightData(); //TODO: adjust to make it take an average for if multy reflect
			//}
			//reflectionColor += reflectionSubBuffer * r;
		}


		// refract ray
		//CollisionPoint * passOnCol = nullptr;
		//RefractionResult refractionResult = refract(ray, collisionPoint);
		for (RefractionResult refraction : refract(ray, collisionPoint, nullptr)) // TODO: clean boiler plate
		{
			double travelDistance = refraction.Distance;
			//std::cout << "\ntravelDistance: " << travelDistance;
			Rayd refractedRay = refraction.RefractedRay;
			ColorRGB color = trace(scene, refractedRay, life - 1, nullptr); //TODO: adjust to make it take an average for if multy refract
			ColorRGB absorbtion = ColorRGB(exp(-materialAbsorbtion.R*travelDistance), exp(-materialAbsorbtion.G*travelDistance), exp(-materialAbsorbtion.B*travelDistance));
			color *= absorbtion;

			outputColor += color * t;

			//
			//
			//// Check if there are objects to hit and return SkyMap data if not
			//RayCollisionResult refractColResult = scene->tryCollision(refractedRay);
			//if (refractColResult.getCollided()) {
			//	CollisionPoint refCol = refractColResult.getCollisionPoint();
			//	t = T(refractedRay, refCol);
			//	//std::cout << "\nReflected ray collided\n";calcFresnelValue
			//}
			//else
			//{
			//	//std::cout << "\nUSING SKY DATA\n";
			//	RayCollisionResult skyData = scene->Sky.hitSkyMap(refractedRay);
			//	CollisionPoint skyCol = skyData.getCollisionPoint();
			//	t = T(refractedRay, skyCol);
			//	refractionBuffer = skyCol.getMaterial().getColor()*t;
			//	//refractionColor *= skyData.getLightData();//TODO: adjust to make it take an average for if multy refract
			//}
			//
			//
			//refractionColor += refractionBuffer * t;
			//Vec3d direction = refractedRay.getDir();
			//refractionColor *= trace(scene, refractedRay, life - 1);
			//materialColor = ColorRGB(direction[0]*0.5 + 0.5, direction[1] * 0.5 + 0.5, direction[2] * 0.5 + 0.5);
			//std::cout << "\nColor: <"<< materialColor.R << " , " << materialColor.G << " , " << materialColor.B << ">";
		}

		//if (!refractionResult.Exiting && lastCollision != nullptr) {
		//RAY_STATUS st = status(ray, collisionPoint);
		//switch (st) {
		//case ENTERING:
		//	break;
		//case EXITING: // Assuming no one-sided polygons
		//	double transmissionLength = 0.;
		//	if (lastCollision != nullptr) {
		//		Vec3d transmissionVector = collisionPoint.getPosition() - lastCollision->getPosition();
		//		double transmissionLength = gmtl::length(transmissionVector);
		//	}
		//	materialAbsorbtion = ColorRGB(exp(-materialAbsorbtion.R*transmissionLength), exp(-materialAbsorbtion.G*transmissionLength), exp(-materialAbsorbtion.B*transmissionLength));
		//	refractionColor *= materialAbsorbtion;
		//	break;
		//case PARALLEL:
		//	break;
		//}

		//if () {
		//	Vec3d transmissionVector = collisionPoint.getPosition() - lastCollision->getPosition();
		//	double transmissionLength = gmtl::length(transmissionVector);
		//	//std::cout << "\nLEN: " << transmissionLength;
		//	//materialAbsorbtion *= -transmissionLength;
		//	materialAbsorbtion = ColorRGB(exp(-materialAbsorbtion.R*transmissionLength), exp(-materialAbsorbtion.G*transmissionLength), exp(-materialAbsorbtion.B*transmissionLength));
		//	//refractionColor *= refractionLight;
		//	refractionColor *= materialAbsorbtion;
		//
		//	//mateColor *= ColorRGB(1. - materialAbsorbtion.R, 1. - materialAbsorbtion.G, 1. - materialAbsorbtion.B);
		//	//lightValue *= exp(-LIGHT_ABSORBSION * transmissionLength);
		//}
		//
		//if (!exiting) {
		//	passOnCol = &collisionPoint;
		//}

		//refractionColor *= refractionLight;
		//reflectionColor *= reflectionLight;
		//refractionColor += mateColor;
		//std::cout << "\nColor: <"<< mateColor.R << " , " << mateColor.G << " , " << mateColor.B << ">";
		//ColorRGB ColorMix = ColorRGB(0,0,0);
		//ColorMix += refractionColor;
		//ColorMix += diffuseColor;
		//ColorMix += reflectionColor;
		//ColorMix += specColor;
		//ColorMix *= material.getColor();
		return outputColor;
	}
	else
	{
		RayCollisionResult skyColResult = scene->Sky.hitSkyMap(ray);
		if (skyColResult.getCollided())
		{
			CollisionPoint skyCollision = skyColResult.getCollisionPoint();
			RTMaterial skyMat = skyCollision.getMaterial();
			return skyMat.getColor();
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
			//std::cout << "\nposition: [" << xIndex << " , " << yIndex << "]";
			RayPacket pixelRays = rayIterator.getAt(xIndex, yIndex);
			ColorRGB colorOut = ColorRGB(0.0,0.0,0.0);
			for (Rayd ray : pixelRays) {
				colorOut += trace(scene, ray, renderSettings.camSettings.RayLife);
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