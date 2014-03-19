#include "BezierControlPoint.h"

namespace Bezier
{
	//--------------------------------------------------------------------------------------------------------------------//
	ControlPoint::ControlPoint(unsigned int pID, const Vector3& pVector3, Real pWeight): 
	_ID(pID), _Vector3(pVector3), _Weight(pWeight)
	{

	}

	//--------------------------------------------------------------------------------------------------------------------//
	ControlPoint::~ControlPoint()
	{

	}

	//--------------------------------------------------------------------------------------------------------------------//
	unsigned int ControlPoint::getID() const
	{
		return _ID;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	const Vector3& ControlPoint::getVector3() const
	{
		return _Vector3;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	Bezier::Real ControlPoint::getWeight() const
	{
		return _Weight;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	std::ostream& operator<< (std::ostream& pOStr, const ControlPoint& pControlPoint)
	{
		return pOStr << "CP " << pControlPoint._ID << " : " << pControlPoint._Vector3 << ' ' << pControlPoint._Weight;
	}

	//--------------------------------------------------------------------------------------------------------------------//
}