#include "BezierMacros.h"
#include "BezierCurveData.h"
#include "BezierCurveSolver.h"

namespace Bezier
{
	//--------------------------------------------------------------------------------------------------------------------//
	CurveSolver::CurveSolver( unsigned int pMaxDegree): _BernsteinPolynomial(pMaxDegree)
	{
		
	}

	//--------------------------------------------------------------------------------------------------------------------//
	CurveSolver::~CurveSolver()
	{
		
	}

	//--------------------------------------------------------------------------------------------------------------------//
	Vector3 CurveSolver::getPointOnCurve( Real pT, const CurveData& pCurveData ) const
	{
		Vector3 curvePoint = Vector3::ZERO;
		if(0 == pCurveData._Degree)
		{
			return curvePoint;
		}

		int subRangeInd = pCurveData.getSubCurveIndex(pT);
		MTest_E_MSG(-1 == subRangeInd, "Can't compute the point for a t value " << pT << " which is out of curve range : " << pCurveData._Range);
		if(-1 != subRangeInd)
		{
			Real rational(0);
			const Range& subRange = pCurveData._SubCurvesRanges[subRangeInd];
			unsigned int baseInd = subRangeInd*pCurveData._Degree;
			Real normalizedT = subRange.normalize(pT);
			unsigned int ptsPerRange = pCurveData.order();
			for(unsigned int p = 0; p < ptsPerRange; ++p)
			{
				const ControlPoint& ctrlPoint = pCurveData._ControlPoints[baseInd+p];
				const Vector3& point = ctrlPoint.getVector3();
				Real Berncoeff = ctrlPoint.getWeight() * _BernsteinPolynomial.bernsteinCoeff(pCurveData._Degree, p, normalizedT);
				rational += Berncoeff;
				curvePoint += point * Berncoeff;
			}
			curvePoint /= rational;
		}
		return curvePoint;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	Vector3 CurveSolver::getPointOnCurveFast( Real pT, int pSubCurveInd, const CurveData& pCurveData ) const
	{
		Vector3 curvePoint = Vector3::ZERO;
		if(0 == pCurveData._Degree)
		{
			return curvePoint;
		}
		
		const Range& subRange = pCurveData._SubCurvesRanges[pSubCurveInd];
		unsigned int baseInd = pSubCurveInd*pCurveData._Degree;
		Real normalizedT = subRange.normalize(pT);
		if(0.0f == normalizedT)
		{// We know the first control point of each subCurve is on the curve. 
			curvePoint = pCurveData._ControlPoints[baseInd].getVector3();
		}
		else if( (pSubCurveInd == pCurveData._SubCurvesRanges.size()-1) && (1.0f == normalizedT))
		{// We also know the last control point is on the curve. 
			curvePoint = pCurveData._ControlPoints[baseInd+pCurveData._Degree].getVector3();
		}
		else
		{// compute the point in the current subcurve.
			Real rational(0);
			unsigned int ptsPerRange = pCurveData.order();
			for(unsigned int p = 0; p < ptsPerRange; ++p)
			{
				const ControlPoint& ctrlPoint = pCurveData._ControlPoints[baseInd+p];
				const Vector3& point = ctrlPoint.getVector3();
				Real Berncoeff = ctrlPoint.getWeight() * _BernsteinPolynomial.bernsteinCoeff(pCurveData._Degree, p, normalizedT);
				rational += Berncoeff;
				curvePoint += point * Berncoeff;
			}
			curvePoint /= rational;
		}
		
		return curvePoint;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	void CurveSolver::constantParametricSubdivision( const CurveData& pCurveData, std::vector<Vector3>& pOutPoints ) const
	{
		// We do the same number of subdiv for each subRange (i.e each polynomial segments).
		unsigned int nbSubdivs = pCurveData.getNbSubdivisions();
		for (unsigned int i=0; i < pCurveData._SubCurvesRanges.size(); ++i)
		{
			unsigned int baseInd = i*pCurveData._Degree;
			// Always push first point of each subcurve.
			pOutPoints.push_back(pCurveData._ControlPoints[baseInd].getVector3());
			if(nbSubdivs > 0)
			{// subdivide.
				const Range& subCurveRange = pCurveData._SubCurvesRanges[i];
				Real end = subCurveRange.getUpperBound();
				Real step = subCurveRange.getLength() / nbSubdivs;
				Real t = subCurveRange.getLowerBound() + step; // first point is already pushed.
				while (t < end)
				{
					Vector3 point = getPointOnCurveFast(t, i, pCurveData);
					pOutPoints.push_back(point);
					t += step;
				}
			}
			if(i == (pCurveData._SubCurvesRanges.size()-1))
			{// Push the last point.
				pOutPoints.push_back(pCurveData._ControlPoints[baseInd+pCurveData._Degree].getVector3());
			}
		}

		//MOut("Points on curve " << pOutPoints.size())
		//for (unsigned int i = 0; i < pOutPoints.size(); ++i)
		//{
		//	MOut(pOutPoints[i]);
		//}
	}

	//--------------------------------------------------------------------------------------------------------------------//
	void CurveSolver::constantSpaceSubdivision( const CurveData& pCurveData, std::vector<Vector3>& pOutPoints ) const
	{
		// Do the subdiv on each subRange (i.e each polynomial segments).
		Real maxSegLength = pCurveData._MaxLength;
		for (unsigned int i=0; i < pCurveData._SubCurvesRanges.size(); ++i)
		{
			unsigned int baseInd = i*pCurveData._Degree;
			const Vector3& startPoint = pCurveData._ControlPoints[baseInd].getVector3();
			const Vector3& endPoint = pCurveData._ControlPoints[baseInd+pCurveData._Degree].getVector3();
			// Start with the max Seg Length in current range.
			Real currentSegLength = (endPoint-startPoint).Length();
			// Always push first point of each subcurve.
			pOutPoints.push_back(startPoint);
			if(currentSegLength > maxSegLength)
			{// subdivide.
				const Range& subCurveRange = pCurveData._SubCurvesRanges[i];
				Real t = subCurveRange.getLowerBound();
				Real end = subCurveRange.getUpperBound();
				Vector3 previousPoint = startPoint;
				Vector3 currentPoint = endPoint;
				while (t < end)
				{
					Real step = subCurveRange.getLength();
					// looking for the candidate step in current range.
					while(currentSegLength > maxSegLength)
					{
						step *= 0.5f; // Start with maximal step = step/2 and divide by 2 while currentSegLength > maxSegLength.
						currentPoint = getPointOnCurveFast(t+step, i, pCurveData);
						currentSegLength = (currentPoint-previousPoint).Length();
					}
					pOutPoints.push_back(currentPoint);
					previousPoint = currentPoint;
					currentPoint = endPoint;
					currentSegLength = (currentPoint-previousPoint).Length();
					t += step;
				}
			}
			if(i == (pCurveData._SubCurvesRanges.size()-1))
			{// Push the last point.
				pOutPoints.push_back(endPoint);
			}
		}

		//MOut("Points on curve " << pOutPoints.size())
		//for (unsigned int i = 0; i < pOutPoints.size(); ++i)
		//{
		//	MOut(pOutPoints[i]);
		//}
	}

	//--------------------------------------------------------------------------------------------------------------------//
	void CurveSolver::curvatureDependentSubdivision( const CurveData& pCurveData, std::vector<Vector3>& pOutPoints ) const
	{
		// Do the subdiv on each subRange (i.e each polynomial segments).
		Real maxDistFromCurrentPointToCurve = pCurveData._MaxDist;
		Real maxAngleDeg = pCurveData._MaxAngle;
		for (unsigned int i=0; i < pCurveData._SubCurvesRanges.size(); ++i)
		{
			unsigned int baseInd = i*pCurveData._Degree;
			const Vector3& ctrlPt0 = pCurveData._ControlPoints[baseInd].getVector3();
			// Always push first point of each subcurve.
			pOutPoints.push_back(ctrlPt0);
			const Vector3& ctrlPtLast = pCurveData._ControlPoints[baseInd+pCurveData._Degree].getVector3();
			if(pCurveData._Degree < 2)
			{
				pOutPoints.push_back(ctrlPtLast);
			}
			else
			{
				const Vector3& crtlPt1 = pCurveData._ControlPoints[baseInd+1].getVector3();
				// Computes Tangent Vector on the first Point.
				// Init first Tangent with the tangent located at the first ctrl point.
				Vector3 tangent1 = crtlPt1-ctrlPt0;

				const Range& subCurveRange = pCurveData._SubCurvesRanges[i];
				Real t = subCurveRange.getLowerBound();
				Real end = subCurveRange.getUpperBound();
				Vector3 currentPoint = ctrlPtLast;
				Vector3 afterPoint = ctrlPtLast;
			
				Vector3 tangent2; // = firstComputedPt - ctrlPt0; tangentEnd.Normalize();
				Real currentAngleDeg = 360;
				while (t < end)
				{
					Real step = subCurveRange.getLength();
					// looking for the candidate step in current range.
					while( currentAngleDeg >= maxAngleDeg)
					{
						step *= 0.5f; // Start with maximal step = step/2 and divide by 2 while currentSegLength > maxSegLength.
						if (t == subCurveRange.getLowerBound())
						{// is first
							currentPoint = getPointOnCurveFast(t+step, i, pCurveData);
							tangent2 = currentPoint - pOutPoints.front();
						}
						else
						{// 
							currentPoint = getPointOnCurveFast(t+step, i, pCurveData);
							afterPoint = getPointOnCurveFast(t+step+step, i, pCurveData);
							tangent2 = afterPoint - pOutPoints.front(); 
						}
						
						currentAngleDeg = angleBetweenDeg(tangent1, tangent2);
						if(step == 0)
						{
							// No Solution.
							break;
						}
					}
					pOutPoints.push_back(currentPoint);
					currentAngleDeg = 360;
					tangent1 = tangent2;
					t += step;					
				}
				if(i == (pCurveData._SubCurvesRanges.size()-1))
				{// Push the last point.
					pOutPoints.push_back(ctrlPtLast);
				}
			}
		}

		//MOut("Points on curve " << pOutPoints.size())
		//for (unsigned int i = 0; i < pOutPoints.size(); ++i)
		//{
		//	MOut(pOutPoints[i]);
		//}
	}

	//--------------------------------------------------------------------------------------------------------------------//	
	/*
	int CurveSolver::FindDrawingPoints(Real t0, Real t1, int insertionIndex, std::list<Vector3>& pList, const CurveData& pCurvData)const
	{
		Real tMid = (t0 + t1) * 0.5f;
		Vector3 p0 = getPointOnCurve(t0, pCurvData);
		Vector3 p1 = getPointOnCurve(t1, pCurvData);

		// check 
		//if((p0 – p1).sqrMagnitude < MINIMUM_SQR_DISTANCE)
		if((p0-p1).Length() >= pCurvData._MaxDist)
		{
			return 0;
		}

		Vector3 pMid = getPointOnCurve(tMid, pCurvData);
		Vector3 leftDirection(p0-pMid); leftDirection.Normalize();
		Vector3 rightDirection(p1-pMid); rightDirection.Normalize();

		//if(leftDirection.Dot(rightDirection)) > threshold)
		if(angleBetweenDeg(leftDirection, rightDirection) < pCurvData._MaxAngle)
		{
			int pointsAddedCount = 0;

			pointsAddedCount += FindDrawingPoints(t0, tMid, insertionIndex, pList, pCurvData); 

			pList.insert(insertionIndex + pointsAddedCount, pMid);
			pointsAddedCount++;

			pointsAddedCount += FindDrawingPoints(tMid, t1, insertionIndex + pointsAddedCount, pList, pCurvData);
			return pointsAddedCount;
		}
		return 0;
	}
	*/

	//--------------------------------------------------------------------------------------------------------------------//
	Real CurveSolver::angleBetweenDeg( const Vector3& pVec1, const Vector3& pVec2 ) const
	{
		Real lenProduct = pVec1.Length() * pVec2.Length();
		// Divide by zero check
		if(lenProduct < 1e-6f)
		{
			lenProduct = 1e-6f;
		}
		Real f = pVec1.Dot(pVec2) / lenProduct;
		return Wm4::Math<Real>::ACos(f) * Wm4::Math<Real>::RAD_TO_DEG;
	}
	
	//--------------------------------------------------------------------------------------------------------------------//
}