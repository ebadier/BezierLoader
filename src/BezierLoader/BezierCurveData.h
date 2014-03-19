#ifndef BezierCurveData_H
#define BezierCurveData_H

#include <vector>
#include "BezierControlPoint.h"

namespace Bezier
{
	///\enum ApproximationTech defines available techniques to approximate a Bezier curve.
	typedef enum { ConstantParametricSubdivision, ConstantSpaceSubdivision, CurvatureDependentSubdivision } ApproximationTech;

	///\class Range stock bounds in which a Bezier curve is defined.
	class Range
	{
	public:
		///\brief Constructor.
		Range(Real pLowerBound = 0, Real pUpperBound = 1);
		///\brief Destructor.
		~Range();
		///\brief return whether pT C [_LowerBound;_UpperBound].
		bool includes(Real pT)const;
		///\brief normalize t regarding [_LowerBound;_UpperBound].
		///\param pT : a real value.
		///\return t C [0;1].
		Real normalize(Real pT)const;

		///\brief Accessors.
		Real getLowerBound()const;
		Real getUpperBound()const;
		Real getLength()const;

		friend std::ostream& operator<< (std::ostream& pOStr, const Range& pRange);

	private:
		Real _LowerBound;
		Real _UpperBound;
	};

	///\class CurveData stock parameters relative to a Bezier Curve.
	class CurveData
	{
	public:
		CurveData();
		~CurveData();

		///\brief return the index of the subCurve involved by the pT value.
		/// if pT value can be included in two subCurves, the first subCurve index is returned.
		///\param pT : t C _Range
		///\return -1 if !(t C _Range).
		int getSubCurveIndex( Real pT) const;
		///\return _Degree + 1.
		unsigned int order()const;
		///\brief return the number of Subdivisions to do on each polynomial segment (#curves)
		/// when using ConstantParametricSubdivision.
		/// if 0 : 0, each polynomial curve segment must be represented by a single line segment.
		unsigned int getNbSubdivisions()const;

		friend std::ostream& operator<< (std::ostream& pOStr, const CurveData& pCurveData);

	public:
		unsigned int				_ID; ///< if we want to identify this.
		std::vector<ControlPoint>	_ControlPoints; ///< list of the control points.
		bool						_IsRational; ///< is the curve Rational ?
		unsigned int				_Degree; ///< the degree of the polynomial segments composing the curve.
		ApproximationTech			_ApproximationTech; ///< the technique used to approximate the curve.
		Range						_Range; ///< the Range in which the curve is defined.
		std::vector<Range>			_SubCurvesRanges; ///< The curve could be composed of many subcurves.

		unsigned int				_Resolution; ///< Resolution used for ConstantParametricSubdivision.
		Real						_MaxLength; ///< Max length criteria for ConstantSpaceSubdivision.
		Real						_MaxDist; ///< Max Dist parameter for CurvatureDependentSubdivision.
		Real						_MaxAngle; ///< Max Angle (in degrees) for CurvatureDependentSubdivision.
	};
}

#endif