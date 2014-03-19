#ifndef BezierCurveSolver_H
#define BezierCurveSolver_H

#include <vector>
#include "BezierBersteinPolynomial.h"

namespace Bezier
{
	class CurveData;

	///\class CurveSolver computes points on a Bezier curve.
	/// This class could be used to build both rational and non-rational Bezier Curves.
	class CurveSolver
	{
	private:
		// copy is not allowed.
		CurveSolver(const CurveSolver&);
		CurveSolver& operator=(const CurveSolver&);
		///\brief get the Position of a point at t knowing the subRange involved by t value.
		///\param pT : t C [a;b] interval.
		///\param pSubCurveInd : the index of the subCurve involved by t value.
		///\param pCurveData : curve infos.
		Vector3 getPointOnCurveFast(Real pT, int pSubCurveInd, const CurveData& pCurveData)const;
		///\brief return the angle between pVec1 & pVec2 in degrees.
		Real angleBetweenDeg(const Vector3& pVec1, const Vector3& pVec2)const;

		//int FindDrawingPoints(Real t0, Real t1, int insertionIndex, std::list<Vector3>& pList, const CurveData& pCurvData)const;

	public:
		///\brief Constructor.
		///\param pMaxDegree Allows this to be able to manage degree <= pMaxDegree.
		CurveSolver(unsigned int pMaxDegree);
		~CurveSolver();
		///\brief get the Position of a point at t.
		///\param pT : t C [a;b] interval.
		///\param pCurveData : curve infos.
		//Vector3 getPointOnCurve(Real pT, const CurveData& pCurveData)const;
		
		///\brief get the Position of a point at t.
		///\param pT : t C [a;b] interval.
		///\param pCurveData : curve infos.
		Vector3 getPointOnCurve(Real pT, const CurveData& pCurveData)const;

		///\brief Approximate a curve with constantParametricSubdivision algorithm.
		///\param pCurveData : curve infos.
		///\param pOutPoints : the points computed which approximate the curve.
		void constantParametricSubdivision(const CurveData& pCurveData, std::vector<Vector3>& pOutPoints)const;
		///\brief Approximate a curve with constantSpaceSubdivision algorithm.
		///\param pCurveData : curve infos.
		///\param pOutPoints : the points computed which approximate the curve.
		void constantSpaceSubdivision(const CurveData& pCurveData, std::vector<Vector3>& pOutPoints)const;
		///\brief Approximate a curve with curvatureDependentSubdivision algorithm.
		///\param pCurveData : curve infos.
		///\param pOutPoints : the points computed which approximate the curve.
		void curvatureDependentSubdivision(const CurveData& pCurveData, std::vector<Vector3>& pOutPoints)const;

	protected:
		BernsteinPolynomial			_BernsteinPolynomial; ///< to get B[n,k] in a fast way.
	};
}

#endif