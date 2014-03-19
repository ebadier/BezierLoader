#ifndef BezierCurveRenderer_H
#define BezierCurveRenderer_H

#include "BezierCurveSolver.h"

namespace Bezier
{
	class CurveData;

	///\class CurveRenderer
	/// inherit from this to implement the rendering part of a curve with your graphic engine.
	class CurveRenderer
	{
	private:
		CurveRenderer(const CurveRenderer&);
		CurveRenderer& operator=(const CurveRenderer&);

	public:
		///\brief Constructor.
		///\param pMaxDegree Allows this to be able to manage degree <= pMaxDegree.
		CurveRenderer(unsigned int pMaxDegree);
		virtual ~CurveRenderer();
		///\brief render the curve pCurveData.
		virtual void renderCurve(const CurveData& pCurveData) = 0;

	protected:
		CurveSolver _CurveSolver; ///< used to compute the points on curve.
	};
}

#endif