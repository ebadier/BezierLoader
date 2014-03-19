#ifndef BezierBersteinPolynomial_H
#define BezierBersteinPolynomial_H

#include <ostream>
#include <vector>
#include "Bezier.h"

namespace Bezier
{
	///\class BernsteinPolynomial is an helper to get coefficients of Bernstein Polynomials in an efficient way.
	/// Binomials coefficients are computed at construction.
	/// The restriction is that you cannot ask for a bernsteinCoeff > pMaxDegree.
	class BernsteinPolynomial
	{
	public:
		///\brief Constructor.
		/// B[n,k] are computed and stocked in _BinomialCoeffs.
		/// This class was tested for pMaxDegree <= 20.
		/// Check yourself if you want to use pMaxDegree > 20.
		BernsteinPolynomial(unsigned int pMaxDegree);

	public:
		///\brief computes the binomial coefficient for control point k and degree n.
		static unsigned int binomialCoeff(unsigned int n, unsigned int k);
		///\brief return true if t C [0;1].
		static bool isNormalized(Real t);

		///\brief get the Bernstein coefficient for control point k and degree n at t.
		/// Launch an exception if ! t C [0;1].
		/// Launch an exception if (n > pMaxDegree) || (k > pMaxDegree). 
		Real bernsteinCoeff(unsigned int n, unsigned int k, Real t)const;

		friend std::ostream& operator<< (std::ostream& pOStr, const BernsteinPolynomial& pBernsteinPolynomial);

	private:
		std::vector<std::vector<unsigned int>> _BinomialCoeffs; ///< precomputed Binomial coefficients as B[n,k].
	};
}

#endif