#include "BezierMacros.h"
#include "BezierCurveData.h"

namespace Bezier
{
	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------class Range-------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	Range::Range( Real pLowerBound /*= 0*/, Real pUpperBound /*= 1*/ ): _LowerBound(pLowerBound), _UpperBound(pUpperBound)
	{
		MTest_E(_UpperBound < _LowerBound);
	}

	//--------------------------------------------------------------------------------------------------------------------//
	Range::~Range()
	{

	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool Range::includes( Real pT ) const
	{
		return (pT <= _UpperBound) && (pT >= _LowerBound);
	}

	//--------------------------------------------------------------------------------------------------------------------//
	Real Range::normalize( Real pT )const
	{
		//MTest_E_MSG(! includes(pT), "Can't normalise t " << pT << " in the range " << *this << " : the value is out of the range");
		Real dist = getLength();
		Real ret(0);
		if(ret != dist) // if(0 != dist)
		{
			ret = (pT-_LowerBound) / dist;
		}
		ret = max(min(ret, 1), 0);
		return ret;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	Bezier::Real Range::getLowerBound() const
	{
		return _LowerBound;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	Bezier::Real Range::getUpperBound() const
	{
		return _UpperBound;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	Bezier::Real Range::getLength() const
	{
		return _UpperBound-_LowerBound;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	std::ostream& operator<< (std::ostream& pOStr, const Range& pRange)
	{
		pOStr << "Range [ " << pRange.getLowerBound() << " ; " << pRange.getUpperBound() << " ]";
		return pOStr;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------class CurveData---------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	CurveData::CurveData(): _ID(0), _ControlPoints(), _IsRational(false), _Degree(0), _ApproximationTech(), _Range(), _SubCurvesRanges(), 
		_Resolution(1), _MaxLength(1), _MaxDist(1), _MaxAngle(20) 
	{

	}

	//--------------------------------------------------------------------------------------------------------------------//
	CurveData::~CurveData()
	{

	}

	//--------------------------------------------------------------------------------------------------------------------//
	unsigned int Bezier::CurveData::order() const
	{
		return _Degree + 1;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	int CurveData::getSubCurveIndex( Real pT ) const
	{
		int rangeInd = -1; // error value.
		int nbRanges = (int)_SubCurvesRanges.size();
		for(int i = 0; i < nbRanges; ++i)
		{
			const Range& range = _SubCurvesRanges[i];
			if(range.includes(pT))
			{
				rangeInd = i;
				break;
			}
		}
		return rangeInd;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	unsigned int CurveData::getNbSubdivisions() const
	{
		return _Degree * _Resolution;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	std::ostream& operator<< (std::ostream& pOStr, const CurveData& pCurveData)
	{
		pOStr << "==========================BezierCurveData==========================\n";
		pOStr << "==> ID : " << pCurveData._ID << "\n";
		if(pCurveData._IsRational)
		{
			pOStr << "==> Type : Rational\n";
		}
		else
		{
			pOStr << "==> Type : Non-Rational\n";
		}
		pOStr << "==> Degree : " << pCurveData._Degree << "\n";
		pOStr << "==> Approx Technique : ";
		switch(pCurveData._ApproximationTech)
		{
		case ConstantParametricSubdivision: 
			pOStr << "ConstantParametricSubdivision\n==> Resolution param : " << pCurveData._Resolution << "\n";break;
		case ConstantSpaceSubdivision: 
			pOStr << "ConstantSpaceSubdivision\n==> MaxLength param : " << pCurveData._MaxLength << "\n"; break;
		case CurvatureDependentSubdivision: 
			pOStr << "CurvatureDependentSubdivision\n==> MaxDist param : " << pCurveData._MaxDist << "\n"
				"==> MaxAngle param : " << pCurveData._MaxAngle << "\n"; break;
		}
		pOStr << "==> " << pCurveData._Range << "\n";

		if(! pCurveData._SubCurvesRanges.empty())
		{
			pOStr << "==> Sub-curve ranges ("<< pCurveData._SubCurvesRanges.size() << ")" << " : \n";
			for (unsigned int i=0; i < pCurveData._SubCurvesRanges.size(); ++i)
			{
				pOStr << "\t" << pCurveData._SubCurvesRanges[i] << "\n";
			}
		}

		if(!pCurveData._ControlPoints.empty())
		{
			pOStr << "==> Control Points ("<< pCurveData._ControlPoints.size() << ")" << " :\n";
			for (unsigned int i=0; i < pCurveData._ControlPoints.size(); ++i)
			{
				pOStr << "\t" << pCurveData._ControlPoints[i] << "\n";
			}
		}

		return pOStr;
	}

	//--------------------------------------------------------------------------------------------------------------------//
}
