#ifndef Bezier_H
#define Bezier_H

#include "Wm4Vector3.h"

///\namespace Bezier is the root namespace for all classes relative to the use of Bezier algorithms.
namespace Bezier
{

#define USE_DOUBLE
#ifdef USE_DOUBLE
	typedef double Real;
#else
	typedef float Real;
#endif

typedef Wm4::Vector3<Real> Vector3;

}

#endif