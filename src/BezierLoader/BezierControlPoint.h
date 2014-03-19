#ifndef BezierVector_H
#define BezierVector_H

#include "Bezier.h"

namespace Bezier
{
	///\class ControlPoint represent a control point used to influence Bezier Curves.
	/// This class could be used to build both rational and non-rational Bezier Curves.
	class ControlPoint
	{
	public: 
		///\brief Constructor
		ControlPoint(unsigned int pID, const Vector3& pVector3 = Vector3::ZERO, Real pWeight = Real(1));
		///\brief Destructor
		~ControlPoint();

		///\brief Accessors.
		unsigned int getID()const;
		const Vector3& getVector3()const; 
		Real getWeight()const;

		friend std::ostream& operator<< (std::ostream& pOStr, const ControlPoint& pControlPoint);

	private:
		unsigned int	_ID; ///< if we want to identify this.
		Vector3			_Vector3; ///< x, y, z coords (default 0.0).
		Real			_Weight; ///< weight (default 1.0).
	};
}

#endif