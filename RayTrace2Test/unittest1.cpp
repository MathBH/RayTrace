#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\RayTrace2\RayCam.h"
#include <gmtl\Point.h>
#include <string>
#include <sstream>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "..\RayTrace2\TraceableObject.h"
#include "..\RayTrace2\SphereTraceable.h"
#include "..\RayTrace2\Scene.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// DEBUG HELPERS //------------------------------------------------------------------------------------
bool ResolutionEquals(ResolutionSettings& r0, ResolutionSettings& r1) {
	return (r0.width == r1.width) && (r0.height == r1.height);
}

// DEBUG WRAPPERS //-----------------------------------------------------------------------------------
// Wraper class for accessing RayIterator protected data
	class TestRIT : RayIterator {
	public:
		TestRIT() : RayIterator() {}
		TestRIT(gmtl::Point3d camPos, ProjectionPlane projPlane,
			ResolutionSettings resSet, double antiAlias) 
		: RayIterator(camPos, projPlane, resSet, antiAlias){}

		gmtl::Point3d getCamPos() {
			return cameraPos;
		}

		ProjectionPlane getProjectionPlane() {
			return projectionPlane;
		}
	};

	// Wraper class for accessing RayCam protected data
	class TestRayCam : RayCam {

	};
// DEBUG WRAPPERS //------------------------------------------------------------------------------------------

namespace SceneObjectsTestSpace
{
	TEST_CLASS(SceneObjectsTest)
	{
		// HELPERS//------------------------------------------------------------------------------------------
		Scene generateScene(int numTraceables) {
			Scene myScene = Scene();
			if (numTraceables > 0) {
				for (int i = 0; i < numTraceables; i++) {
					SphereTraceable sphere = SphereTraceable();
					myScene.objects.traceable.push_front(&sphere);
				}
			}
			return myScene;
		}
		TEST_METHOD(sceneGenHelperTest) {
			int numTraceables = 50;
			Scene s1 = generateScene(numTraceables);
			int counter = 0;
			for (TraceableObject * obj : s1.objects.traceable) {
				counter++;
			}

			std::wstringstream sstream = std::wstringstream();
			sstream << "expected counter: " << numTraceables << "\n counter:" << counter << '\n';
			std::wstring fail_message = sstream.str();

			Assert::IsTrue(counter == numTraceables, fail_message.c_str());
		}
		// HELPERS//------------------------------------------------------------------------------------------

		TEST_METHOD(test1) {
		}
	};
}
namespace RayIteratorTestSpace
{
	TEST_CLASS(RayIteratorTest)
	{
	private:

		void projectionPlaneValidationTests(ProjectionPlane pp, double fov, double d, double aspectRatio) {
			double expected_d = d;
			double expected_z_near = -expected_d;
			double expected_fov = fov;
			double expected_aspect_ratio = aspectRatio;

			double expectedX0 = (expected_z_near * tan(expected_fov));

			std::wstringstream sstreamX0 = std::wstringstream();
			sstreamX0 << "expected x0: " << expectedX0 << "\n x0:" << pp.x0 << '\n';
			std::wstring fail_messageX0 = sstreamX0.str();

			Assert::IsTrue(pp.x0 == expectedX0, fail_messageX0.c_str());

			double expectedY0 = expectedX0 / expected_aspect_ratio;

			std::wstringstream sstreamY0 = std::wstringstream();
			sstreamY0 << "expected y0: " << expectedY0 << "\n y0:" << pp.y0 << '\n';
			std::wstring fail_messageY0 = sstreamY0.str();

			Assert::IsTrue(pp.y0 == expectedY0, fail_messageY0.c_str());

			double expectedAspectRatio = expected_aspect_ratio;
			double aspectRatioOut = pp.width / pp.height;

			std::wstringstream sstreamAR = std::wstringstream();
			sstreamAR << "expected AR: " << expectedAspectRatio << "\n AR:" << aspectRatioOut << '\n';
			std::wstring fail_messageAR = sstreamAR.str();

			Assert::IsTrue(expectedAspectRatio == aspectRatioOut, fail_messageAR.c_str());

			double expectedWidth = abs(expectedX0) * 2;
			double expectedHeight = abs(expectedY0) * 2;

			std::wstringstream sstreamWH = std::wstringstream();
			sstreamWH << "expected width: " << expectedWidth << "\n width:" << pp.width << '\n';
			sstreamWH << "expected height: " << expectedHeight << "\n height:" << pp.height << '\n';
			std::wstring fail_messageWH = sstreamWH.str();

			Assert::IsTrue((expectedWidth == pp.width) &&
				(expectedHeight == pp.height), fail_messageWH.c_str());

			std::wstringstream sstreamZ = std::wstringstream();
			sstreamZ << "expected z: " << expected_z_near << "\n z:" << pp.z << '\n';
			std::wstring fail_messageZ = sstreamZ.str();

			Assert::IsTrue(pp.z == expected_z_near, fail_messageZ.c_str());
		}

	public:

		TEST_METHOD(ResolutionSettingsConstructor) {
			ResolutionSettings rs = ResolutionSettings();
			Assert::IsTrue(rs.height == DEFAULT_RESOLUTION_HEIGHT);
			Assert::IsTrue(rs.width == DEFAULT_RESOLUTION_WIDTH);
		}

		TEST_METHOD(ProjectionPlaneConstructorDefault) {
			ProjectionPlane ppDefault = ProjectionPlane();
			projectionPlaneValidationTests(ppDefault, DEFAULT_FOV, DEFAULT_D_NEAR, DEFAULT_ASPECT_RATIO);
		}

		TEST_METHOD(ProjectionPlaneConstructor) {
			ProjectionPlane pp = ProjectionPlane(M_PI/4,2.0,1.);
			projectionPlaneValidationTests(pp, M_PI/4,2.0, 1.);
		}

		TEST_METHOD(ProjectionPlaneConstructor0) {
			ProjectionPlane pp = ProjectionPlane(0.0, 2.0, 1.);

			std::wstringstream sstream = std::wstringstream();
			sstream << "expected width: " << 0.0 << "\n width:" << pp.width << '\n';
			sstream << "expected height: " << 0.0 << "\n height:" << pp.height << '\n';
			std::wstring fail_message = sstream.str();

			Assert::IsTrue(pp.width == 0.0 && pp.height == 0.0, fail_message.c_str());
		}

		TEST_METHOD(ProjectionPlaneDistanceScale) {
			ProjectionPlane pp0 = ProjectionPlane(DEFAULT_FOV, 0.1, DEFAULT_ASPECT_RATIO);
			ProjectionPlane pp1 = ProjectionPlane(DEFAULT_FOV, 0.101, DEFAULT_ASPECT_RATIO);
			ProjectionPlane pp2 = ProjectionPlane(DEFAULT_FOV, 500., DEFAULT_ASPECT_RATIO);

			Assert::IsTrue(pp1.width > pp0.width);
			Assert::IsTrue(pp1.width < pp2.width);
		}

		TEST_METHOD(ResolutionSettingsEquals) {
			ResolutionSettings rs0 = ResolutionSettings(4.0, 1.0);
			ResolutionSettings rs1 = ResolutionSettings(408.0, 101.0);
			ResolutionSettings rs2 = ResolutionSettings(408.000001, 101.00001);
			ResolutionSettings rs4 = ResolutionSettings(408.0, 101.0);

			Assert::IsFalse(ResolutionEquals(rs1, rs0));
			Assert::IsTrue(ResolutionEquals(rs1, rs4));
			Assert::IsFalse(ResolutionEquals(rs1, rs2));
		}

		TEST_METHOD(RayIteratorConstructorEmpty) {
			TestRIT iter = TestRIT();
			std::wstringstream sstream = std::wstringstream();
			sstream << "cam pos: " << iter.getCamPos()[0] << " , " << iter.getCamPos()[1] << " , " << iter.getCamPos()[2] << '\n';
			std::wstring fail_message = sstream.str();

			Assert::IsTrue(iter.getCamPos() == DEFAULT_CAM_POS, fail_message.c_str());
		}

		TEST_METHOD(RayIteratorConstructorBasic)
		{
			ResolutionSettings resSet = ResolutionSettings(480., 640.);
			ProjectionPlane projPlane = ProjectionPlane(33., 0.1, 1.33);
			TestRIT iter = TestRIT(gmtl::Point3d(0.2,0.4,0.6), projPlane, resSet, 16.);
			Assert::IsNotNull(&iter);

			std::wstringstream sstream = std::wstringstream();
			sstream << "cam pos: " << iter.getCamPos()[0] << " , " << iter.getCamPos()[1] << " , "<< iter.getCamPos()[2] <<'\n';
			std::wstring fail_message = sstream.str();

			Assert::IsTrue(iter.getCamPos() == gmtl::Point3d(0.2, 0.4, 0.6), fail_message.c_str());
		}

		// Check that for a given xValue and yValue between 0-1, we get a corresponding point on the projection plane
		// going left to right and bottom to top (comparing vals each time to ensure proper progression and that
		// final x-y yields plane width - xDrift and plane height - yDrift
		// Also that 0,0, gives x0, y0
		// will have to set a resolution setting for testing
		// TODO: factor into a helper function for testing with multiple parameters
		TEST_METHOD(GetPointOnProjectionPlane) {
			Assert::Fail(L"Unimplemented");
		}

		// Check firing the right number of rays, that they are all normalized, and progressing from left to right, bottom to top (using angles to check)
		TEST_METHOD(TestRays) {
			Assert::Fail(L"Unimplemented");
		}
	};
}

// reminders
// use 3 for vectors and points and 44 for matrices all in doubles

//gmtl::AxisAnglef angle = gmtl::AxisAnglef();
//gmtl::Matrix44f transMatrix = gmtl::Matrix44f();
//gmtl::Vec3f transposeVec = gmtl::Vec3f(1.0, 0.0, 0.0);
//angle.setAngle(gmtl::Math::PI / 4);
//gmtl::setTrans(transMatrix, transposeVec);
//gmtl::setRot(transMatrix, angle);
//gmtl::Point3f point = gmtl::Point3f(1.0, 1.0, 1.0);
//gmtl::Vec3f vector = gmtl::Vec3f(1.0, 1.0, 1.0);
//
//std::cout << "v: " << vector << '\n';
//std::cout << "p: " << point << '\n';
//
//vector = transMatrix * vector;
//
//point = transMatrix * point;
//
//std::cout << "v: " << vector << '\n';
//std::cout << "p: " << point << '\n';