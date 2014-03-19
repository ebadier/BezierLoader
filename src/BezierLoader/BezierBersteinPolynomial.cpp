#include "BezierMacros.h"
#include "BezierBersteinPolynomial.h"

namespace Bezier
{
	//--------------------------------------------------------------------------------------------------------------------//
	unsigned int BernsteinPolynomial::binomialCoeff( unsigned int n, unsigned int k )
	{
		unsigned int result = 1;
		int min = min(k, n-k);
		for(int i=1; i <= min; ++i)
		{
			result *= (n-i+1);
			result /= i;
		}
		return result;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool BernsteinPolynomial::isNormalized( Real t )
	{
		return (t <= 1.0f) && (t >= 0.0f);
	}
	
	//--------------------------------------------------------------------------------------------------------------------//
	Real BernsteinPolynomial::bernsteinCoeff(unsigned int n, unsigned int k, Real t)const
	{
		MDebugTest_E(! isNormalized(t));
		MDebugTest_E( (n >= _BinomialCoeffs.size()) || (k >= _BinomialCoeffs.size()));
		Real res = _BinomialCoeffs[n][k] * std::pow(Real(1) - t, int(n - k)) * std::pow(t, int(k));
		return res;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	BernsteinPolynomial::BernsteinPolynomial(unsigned int pMaxDegree): _BinomialCoeffs(pMaxDegree+1, std::vector<unsigned int>(pMaxDegree+1))
	{
		for (unsigned int n = 0; n <= pMaxDegree; ++n)
		{
			for (unsigned int i = 0; i <= pMaxDegree; ++i)
			{
				_BinomialCoeffs[n][i] = binomialCoeff(n, i);
			}
		}
	}

	//--------------------------------------------------------------------------------------------------------------------//
	std::ostream& operator<<(std::ostream& pOStr, const BernsteinPolynomial& pBernsteinPolynomial)
	{
		pOStr << "=========================================Binomial Coefficients Table=========================================\n";
		for (unsigned int n = 0; n < pBernsteinPolynomial._BinomialCoeffs.size(); ++n)
		{
			pOStr << "B[ " << n << "; i ] ";

			for (unsigned int i = 0; i < pBernsteinPolynomial._BinomialCoeffs[n].size(); ++i)
			{
				pOStr << pBernsteinPolynomial._BinomialCoeffs[n][i] << " ";
			}
			pOStr << "\n";
		}
		return pOStr;
	}

	//--------------------------------------------------------------------------------------------------------------------//
}