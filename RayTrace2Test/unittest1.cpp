#include "stdafx.h"
#include "CppUnitTest.h"
#include "RayCam.h"
#include <gmtl\Point.h>
#include <string>
#include <sstream>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "TraceableObject.h"
#include "SphereTraceable.h"
#include "Scene.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// DEBUG WRAPPERS //----------------------------------------------------------------------------------------
// Wraper class for accessing RayIterator protected data
	class TestRIT : RayIterator
	{
	public:
		TestRIT() : RayIterator(DEFAULT_CAM_POS, ProjectionPlane(), ResolutionSettings(), DEFAULT_ANTIALIAS) {}
		TestRIT(gmtl::Point3d camPos, ProjectionPlane projPlane,
			ResolutionSettings resSet, int antiAlias) 
		: RayIterator(camPos, projPlane, resSet, antiAlias){}

		gmtl::Point3d getCamPos() {
			return cameraPos;
		}

		ProjectionPlane getProjectionPlane() {
			return projectionPlane;
		}

		double getProjPlaneWidth() {
			return projectionPlane.width;
		}

		double getProjPlaneHeight() {
			return projectionPlane.height;
		}

		int getXFullScale() {
			return xFullScale;
		}

		int getYFullScale() {
			return yFullScale;
		}

		int getXIndex() {
			return xCursor;
		}

		int getYIndex() {
			return yCursor;
		}

		using RayIterator::next;
		using RayIterator::hasNext;

		gmtl::Point3d accessGetPointOnPlane(double xVal, double yVal) {
			return getPointOnPlane(xVal, yVal);
		}
	};

// DEBUG HELPERS //-----------------------------------------------------------------------------------------
bool ResolutionEquals(ResolutionSettings& r0, ResolutionSettings& r1)
{
	return (r0.width == r1.width) && (r0.height == r1.height);
}

// TEST SPACES //-------------------------------------------------------------------------------------------
namespace SceneObjectsTestSpace
{
	TEST_CLASS(SceneObjectsTest)
	{
		// DEBUG HELPERS //---------------------------------------------------------------------------------
		void sceneFillTraceablesTest(int numTraceables, int expectedNum)
		{
			Scene s1 = Scene();
			if (numTraceables > 0) {
				for (int i = 0; i < numTraceables; i++) {
					SphereTraceable sphere = SphereTraceable();
					s1.objects.traceable.push_front(&sphere);
				}
			}
			int counter = 0;
			for (TraceableObject * obj : s1.objects.traceable) {
				counter++;
			}

			std::wstringstream sstream = std::wstringstream();
			sstream << "expected counter: " << expectedNum << "\n counter:" << counter << '\n';
			std::wstring fail_message = sstream.str();

			Assert::IsTrue(counter == expectedNum, fail_message.c_str());
		}

		// TESTS //-----------------------------------------------------------------------------------------
		TEST_METHOD(TraceableInterfaceTest) // Test that subclasses can be made, individually overriding the tryCollision function
		{
			static bool subFunction1 = false;
			static bool subFunction2 = false;
			class TraceableTest1 : TraceableObject {
			public:
				RayCollisionResult tryCollision(gmtl::Rayd ray) override {
					subFunction1 = true;
					return RayCollisionResult();
				}
			};
			class TraceableTest2 : TraceableObject {
			public:
				RayCollisionResult tryCollision(gmtl::Rayd ray) override {
					subFunction2 = true;
					return RayCollisionResult();
				}
			};
			TraceableTest1 t1 = TraceableTest1();
			TraceableTest2 t2 = TraceableTest2();
			gmtl::Rayd r = gmtl::Rayd();

			Assert::IsFalse(subFunction1, L"untested1");
			Assert::IsFalse(subFunction2, L"untested2");
			t1.tryCollision(r);
			Assert::IsTrue(subFunction1, L"untested1");
			Assert::IsFalse(subFunction2, L"untested2");
			t2.tryCollision(r);
			Assert::IsTrue(subFunction1, L"untested1");
			Assert::IsTrue(subFunction2, L"untested2");
		}

		TEST_METHOD(SceneAddTraceablesTest) {
			int numTraceables0 = 0;
			int numTraceables1 = 7;
			int numTraceables2 = 617;
			int numTraceables3In = -107;
			int numTraceables3Out = 0;
			sceneFillTraceablesTest(numTraceables0, numTraceables0);
			sceneFillTraceablesTest(numTraceables1, numTraceables1);
			sceneFillTraceablesTest(numTraceables2, numTraceables2);
			sceneFillTraceablesTest(numTraceables3In, numTraceables3Out);
		}

		TEST_METHOD(SceneRemoveTraceablesTest) {
			Scene s = Scene();
			SphereTraceable sphere1 = SphereTraceable();
			SphereTraceable sphere2 = SphereTraceable();

			//Iteration 1

				// CONTROL: Making sure the base structure is built properly for test
				Assert::IsTrue(s.objects.traceable.empty());
				s.objects.traceable.push_front(&sphere1);
				Assert::IsTrue(s.objects.traceable.front() == &sphere1);
				s.objects.traceable.push_front(&sphere2);
				Assert::IsTrue(s.objects.traceable.front() == &sphere2);

				//Remove test
				s.objects.traceable.remove(&sphere2);
				Assert::IsTrue(s.objects.traceable.front() == &sphere1);
				s.objects.traceable.remove(&sphere1);
				Assert::IsTrue(s.objects.traceable.empty());

			//Iteration 2

				// CONTROL: Making sure the base structure is built properly for test
				s.objects.traceable.push_front(&sphere1);
				Assert::IsTrue(s.objects.traceable.front() == &sphere1);
				s.objects.traceable.push_front(&sphere2);
				Assert::IsTrue(s.objects.traceable.front() == &sphere2);

				//Remove test
				s.objects.traceable.remove(&sphere1);
				Assert::IsTrue(s.objects.traceable.front() == &sphere2);
				s.objects.traceable.remove(&sphere2);
				Assert::IsTrue(s.objects.traceable.empty());
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

		TEST_METHOD(HELPERResolutionSettingsEquals) {
			ResolutionSettings rs0 = ResolutionSettings(4, 1);
			ResolutionSettings rs1 = ResolutionSettings(408, 101);
			ResolutionSettings rs2 = ResolutionSettings(409, 100);
			ResolutionSettings rs4 = ResolutionSettings(408, 101);

			Assert::IsFalse(ResolutionEquals(rs1, rs0));
			Assert::IsTrue(ResolutionEquals(rs1, rs4));
			Assert::IsFalse(ResolutionEquals(rs1, rs2));
		}

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
			ProjectionPlane pp = ProjectionPlane(M_PI / 4, 2.0, 1.);
			projectionPlaneValidationTests(pp, M_PI / 4, 2.0, 1.);
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

		TEST_METHOD(RayIteratorConstructorEmpty) {
			TestRIT iter = TestRIT();
			std::wstringstream sstream = std::wstringstream();
			sstream << "cam pos: " << iter.getCamPos()[0] << " , " << iter.getCamPos()[1] << " , " << iter.getCamPos()[2] << '\n';
			std::wstring fail_message = sstream.str();

			Assert::IsTrue(iter.getCamPos() == DEFAULT_CAM_POS, fail_message.c_str());
		}

		TEST_METHOD(RayIteratorConstructorBasic)
		{
			ResolutionSettings resSet = ResolutionSettings(480, 640);
			ProjectionPlane projPlane = ProjectionPlane(33., 0.1, 1.33);
			TestRIT iter = TestRIT(gmtl::Point3d(0.2, 0.4, 0.6), projPlane, resSet, 16);
			Assert::IsNotNull(&iter);

			std::wstringstream sstream = std::wstringstream();
			sstream << "cam pos: " << iter.getCamPos()[0] << " , " << iter.getCamPos()[1] << " , " << iter.getCamPos()[2] << '\n';
			std::wstring fail_message = sstream.str();

			Assert::IsTrue(iter.getCamPos() == gmtl::Point3d(0.2, 0.4, 0.6), fail_message.c_str());
		}

		// Check that for a given xValue and yValue between 0-1, we get a corresponding point on the projection plane
		TEST_METHOD(GetPointOnProjectionPlane) {
			bool finished = false;

			TestRIT iter = TestRIT();
			double width = iter.getProjPlaneWidth();
			double height = iter.getProjPlaneHeight();
			double wRes = 640.;
			double hRes = 480.;
			double xDrift = 1 / wRes;
			double yDrift = 1 / hRes;
			double x0 = -(width / 2.);
			double y0 = -(height / 2.);

			double x_last = x0 - xDrift;
			for (double i = 0.; i < wRes; i += 1.) {
				double y_last = y0 - yDrift;
				double x_expected = x0 + (i / wRes)*width;
				std::wstringstream sstreamX = std::wstringstream();
				sstreamX << "\n[" << i << "] last-x: " << x_last << "\nx: " << x_expected << '\n';
				std::wstring fail_messageX = sstreamX.str();

				Assert::IsTrue(x_expected > x_last, fail_messageX.c_str());

				x_last = x_expected;
				for (double j = 0.; j < hRes; j += 1.) {
					double y_expected = y0 + (j / hRes)*height;
					std::wstringstream sstreamY = std::wstringstream();
					sstreamY << "\n[" << j << "] last-y: " << y_last << "\ny: " << y_expected << '\n';
					std::wstring fail_messageY = sstreamY.str();

					Assert::IsTrue(y_expected > y_last, fail_messageY.c_str());

					y_last = y_expected;

					gmtl::Point3d point = iter.accessGetPointOnPlane(i / wRes, j / hRes);

					std::wstringstream sstream = std::wstringstream();
					sstream << "\nexpected-x: " << x_expected << "\nx-out: " << point[0] << '\n' <<
						"\nexpected-y: " << y_expected << "\ny-out: " << point[1] << "\n";
					std::wstring fail_message = sstream.str();

					Assert::IsTrue(x_expected == point[0] && y_expected == point[1], fail_message.c_str());

					if ((i + 1.) == wRes && (j + 1.) == hRes) {
						double expectedEndPointx = ((width - (xDrift*width)) - (width / 2));
						double expectedEndPointy = ((height - (yDrift*height)) - (height / 2));
						std::wstringstream sstreamEndPoint = std::wstringstream();
						sstreamEndPoint << "\nEND POINT\nexpected-x: " << expectedEndPointx <<
							"\nexpected-y: " << expectedEndPointy << '\n' <<
							"\nout-x: " << point[0] <<
							"\nout-y: " << point[1] << '\n';
						std::wstring fail_messageEndPoint = sstreamEndPoint.str();

						Assert::IsTrue(expectedEndPointx == point[0], fail_messageEndPoint.c_str());
						Assert::IsTrue(expectedEndPointy == point[1], fail_messageEndPoint.c_str());
						finished = true;
					}
				}
			}

			Assert::IsTrue(finished, L"\nEnd was not reached\n");
		}

		// Check that the iterator is progressing through the projection plane in the right order and
		// accross all points
		TEST_METHOD(IterationTest)
		{
			TestRIT iter = TestRIT();
			int xFullScale = iter.getXFullScale();
			int yFullScale = iter.getYFullScale();

			//std::wstringstream progressionSstream = std::wstringstream();
			//progressionSstream << "\nLOG:\n";

			Assert::IsTrue(iter.hasNext(), L"hasNext() returned false at start of iteration");

			for (int yIdxExp = 0; yIdxExp < yFullScale; yIdxExp++) {
				for (int xIdxExp = 0; xIdxExp < xFullScale; xIdxExp++) {
					//progressionSstream << "<"<< xIdxExp << "-"<< yIdxExp << ">[" << iter.getXIndex() << ", " << iter.getYIndex() << "], ";
					//std::wstring progressLog = progressionSstream.str();

					Assert::IsTrue(iter.hasNext());
					Assert::IsTrue(iter.getXIndex() == xIdxExp && iter.getYIndex() == yIdxExp/*, progressLog.c_str()*/);

					iter.next();
				}
			}
			Assert::IsFalse(iter.hasNext(), L"\nHasNext() returns true when indices are at full scale.\n");
			//Assert::Fail(L"\nEnd was not reached\n");
		}

		// Check that the right amount of rays are produced 
		TEST_METHOD(RayCountTest) {
			TestRIT iter = TestRIT();
			int numRays = DEFAULT_RESOLUTION_WIDTH * DEFAULT_RESOLUTION_HEIGHT * DEFAULT_ANTIALIAS * DEFAULT_ANTIALIAS;
			int counter = 0;
			while (iter.hasNext()) {
				iter.next();
				counter++;
			}
			std::wstringstream sstream = std::wstringstream();
			sstream << "\nexpected count: " << numRays << "\ncount out: " << counter << '\n';
			std::wstring fail_message = sstream.str();
			Assert::IsTrue(counter == numRays, fail_message.c_str());
		}

		// Check that rays are of the correct format
		TEST_METHOD(RayTest) {
			TestRIT iter = TestRIT();
			while (iter.hasNext()) {
				gmtl::Rayd ray = iter.next();
				gmtl::Point3d orig = ray.getOrigin();
				gmtl::Vec3d dir = ray.getDir();
				double dlength = gmtl::length(dir);

				std::wstringstream sstream = std::wstringstream();
				sstream << "\nlength: " << dlength << "\norig: [" <<
					orig[0] << ", " << orig[1] << ", " << orig[2] << "] ";
				std::wstring fail_message = sstream.str();
				Assert::IsTrue(0.999999999999 < dlength && dlength < 1.000000000001, fail_message.c_str());
				Assert::IsTrue(ray.getOrigin() == DEFAULT_CAM_POS, fail_message.c_str());
			}
		}
	};
}
namespace DataSpitters
{
	TEST_CLASS(Data)
	{
		TEST_METHOD(RayIteratorData) {
			std::ofstream outputFile;
			outputFile.open("RayItData.txt");
			TestRIT iter = TestRIT();
			int yIndex = iter.getYIndex();
			while (iter.hasNext()) {
				gmtl::Rayd ray = iter.next();
				gmtl::Point3d orig = ray.getOrigin();
				gmtl::Vec3d dir = ray.getDir();
				outputFile << "[" /*<< " <" << orig[0] << ", " << orig[1] << ", " << orig[2] << ">: "*/ <<
					dir[0] << ", " << dir[1] << ", " << dir[2] << "] ";
				if (yIndex != iter.getYIndex()) {
					yIndex = iter.getYIndex();
					outputFile << '\n';
				}
			}
			outputFile.close();
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