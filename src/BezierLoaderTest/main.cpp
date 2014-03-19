#include "Bezier.h"
#include "BezierMacros.h"
#include "BezierLineOfCommand.h"
#include "BezierBersteinPolynomial.h"
#include "BezierObjLoader.h"
#include "BezierCurveSolver.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreBezierViewer.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#if SUBSYSTEM != CONSOLE
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
#endif
{
	try
	{
		std::string objFilePath = Bezier::LineOfCommand::lookNowForString("file");
		Bezier::ObjLoader bezierObjLoader;
		std::string errorMsg = "";
		// load curve data.
		std::vector<Bezier::CurveData> curves;
		if(bezierObjLoader.loadCurves(objFilePath, curves))
		{
			//for (unsigned int i = 0; i < curves.size(); ++i)
			//{
			//	MOut(curves[i]);
			//}

			OgreBezierViewer demo(Bezier::ObjLoader::maxDegreeForCurves());
			demo.startDemo(curves);
		}
	}
	catch(std::exception& e)
	{
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		fprintf(stderr, "An exception has occurred: %s\n", e.what());
#endif
	}

	return 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||
/// Tests.
//int main()
//{	
	///\brief Test Obj Loading.
	//try
	//{
	//	std::string objFilePath = Bezier::LineOfCommand::lookNowForString("file");
	//	Bezier::ObjLoader bezierObjLoader;
	//	std::string errorMsg = "";
	//	// load curve data.
	//	std::vector<Bezier::CurveData> curves;
	//	if(bezierObjLoader.loadCurves(objFilePath, curves))
	//	{
	//		// compute curves.
	//		for (unsigned int i = 0; i < curves.size(); ++i)
	//		{
	//			MOut(curves[i]);
	//		}

	//		// draw curves.
	//		Bezier::Real t(0), end(4);
	//		Bezier::CurveSolver curveSolver(Bezier::ObjLoader::maxDegreeForCurves());
	//		while(t < end)
	//		{
	//			std::ostringstream ostrm;
	//			ostrm << "t = " << t << " : ";
	//			for (unsigned int i = 0; i < curves.size(); ++i)
	//			{
	//				ostrm << curveSolver.getPointOnCurve(t, curves[i]) << "\t\t";
	//			}
	//			MOut(ostrm.str());
	//			t += 0.1;
	//		}
	//	}
	//}
	//catch(std::exception& e)
	//{
	//	MWarning(e.what());
	//}
	//MPause;

	///\brief Test normalization of Ranges.
	//try
	//{
	//	for (unsigned int i = 0; i < 10; ++i)
	//	{
	//		Bezier::Real lowerBound = i;
	//		Bezier::Real upperBound = i + Bezier::Real(0.2) * i;
	//		Bezier::Range range(lowerBound, upperBound);
	//		MOut(range);

	//		Bezier::Real t = range.getLowerBound() + (range.getLength() * Bezier::Real(0.6));
	//		Bezier::Real normT = range.normalize(t);
	//		MOut("t : " << t << " , normalized t : " << normT << "\n");
	//	}
	//}
	//catch (std::exception& e)
	//{
	//	MWarning(e.what());
	//}
	//MPause;
	
	
	///\brief Test computation of binomialCoeff. 
	//try
	//{
	//	unsigned int maxDegree = 20;
	//	Bezier::BernsteinPolynomial poly(maxDegree); 
	//	MOut(poly);

	//	///\brief Test computation of BersteinCoeff(t).
	//	for (Bezier::Real t = 0.0; t <= 1.0f; t+= Bezier::Real(0.1))
	//	{
	//		MOut(poly.bernsteinCoeff(20, 0, t));
	//	}

	//}
	//catch (std::exception& e)
	//{
	//	MWarning(e.what());
	//}
	//MPause;

	//return EXIT_SUCCESS;
//}