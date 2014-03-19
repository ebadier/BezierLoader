#include "BezierMacros.h"
#include "BezierStringHlp.h"
#include "BezierObjLoader.h"

#include <limits>
#include <iostream>
#include <sstream>

namespace Bezier
{
// Require statements.
#define CSTYPE	"cstype"
#define CTECH	"ctech"
#define DEG		"deg"
#define CURV	"curv"
#define PARM	"parm"
#define END		"end"

// Index of Statements in _Statementfounds List.
#define CSTYPE_IND	0
#define CTECH_IND	1
#define DEG_IND		2
#define CURV_IND	3
#define PARM_IND	4
#define END_IND		5

// Arguments
#define RATIONAL "rat"

// Conditions
#define MIN_DEGREE 1
#define MIN_CtrlPts 2
	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------struct Vertex-------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	ObjLoader::Vertex::Vertex()
	{
		// Default values.
		_Data[0] = 0;
		_Data[1] = 0;
		_Data[2] = 0;
		_Data[3] = 1;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------class ObjLoader-------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------//
	Bezier::ObjLoader::ObjLoader(): _VertexCache(), _CurveDataCache(), _Statementfounds(6, false)
	{
	
	}

	//--------------------------------------------------------------------------------------------------------------------//
	ObjLoader::~ObjLoader()
	{

	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool ObjLoader::loadCurves( const std::string& pObjFilepath, std::vector<CurveData>& pOutCurves)
	{
		// Open the file.
		std::ifstream objFile(pObjFilepath);
		MTest_E_MSG("obj" != Bezier::StringHlp::extract_extention(pObjFilepath), "Wrong file extension : " << pObjFilepath << "\n .obj extension expected");
		MTest_E_MSG(! objFile.is_open(), "File not found : " << pObjFilepath);
		MOut("File " << pObjFilepath << " opened !");

		std::string line;
		std::string attribute;
		int lineNum = 0;
		std::ostringstream errorMsg("");
		while(std::getline(objFile, line))
		{
			++lineNum;
			if( ("" == line) || ('#' == line[0]) )
			{	// Skip everything and continue with the next line
				continue;
			}

			std::istringstream lineStream(line);
			lineStream >> attribute;

			// Vertex
			if("v" == attribute)
			{
				Vertex vertex;
				if(tryParseVertex(lineStream, vertex, errorMsg))
				{
					_VertexCache.push_back(vertex);
				}
			}
			else if(CSTYPE == attribute)
			{// Here we start parsing a new curve.
				if(!_Statementfounds[CSTYPE_IND])
				{
					if(tryParseType(lineStream, errorMsg))
					{
						resetStatements(); // OK Let's begin parse the other required statements.
						_Statementfounds[CSTYPE_IND] = true;
					}
				}
				else
				{
					errorMsg << CSTYPE << " statement already encountered for the current curve";
				}
			}
			else if(_Statementfounds[CSTYPE_IND])
			{// We can continue to parse the curve.
				if(CTECH == attribute)
				{
					if(!_Statementfounds[CTECH_IND])
					{
						if(tryParseTechnique(lineStream, errorMsg))
						{
							_Statementfounds[CTECH_IND] = true;
						}
					}
					else
					{
						errorMsg << CTECH << " statement already encountered for the current curve";
					}
				}
				else if(DEG == attribute)
				{
					if(!_Statementfounds[DEG_IND])
					{
						if(tryParseDegree(lineStream, errorMsg))
						{
							_Statementfounds[DEG_IND] = true;
						}
					}
					else
					{
						errorMsg << DEG << " statement already encountered for the current curve";
					}
				}
				else if(CURV == attribute)
				{
					// We can only parse PARM if CURV was.
					if(_Statementfounds[DEG_IND])
					{
						if(!_Statementfounds[CURV_IND])
						{
							if (tryParseCurve(lineStream, errorMsg))
							{
								_Statementfounds[CURV_IND] = true;
							}
						}
						else
						{
							errorMsg << CURV << " statement already encountered for the current curve";
						}
					}
					else
					{
						errorMsg << DEG << " statement must take place before " << CURV << " statement";
					}
					
				}
				else if( PARM == attribute )
				{
					// We can only parse PARM if CURV was.
					if(_Statementfounds[CURV_IND])
					{
						if(!_Statementfounds[PARM_IND])
						{
							if(tryParseParm(lineStream, errorMsg))
							{
								_Statementfounds[PARM_IND] = true;
							}	
						}
						else
						{
							errorMsg << PARM << " statement already encountered for the current curve";
						}
					}
					else
					{
						errorMsg << CURV << " statement must take place before " << PARM << " statement";
					}
				}
				else if(END == attribute)
				{
					if(!_Statementfounds[END_IND])
					{
						_Statementfounds[END_IND] = true;
						// Check for error before creating new CurveData.
						if(allStatementsFound(errorMsg) /*&& _CurveDataCache.isValid(errorMsg)*/)
						{
							_CurveDataCache._ID = pOutCurves.size() + 1;
							pOutCurves.push_back(_CurveDataCache);
							// prepare to parse a new curve.
							resetStatements();
						}
					}
					else
					{
						errorMsg << END << " statement already encountered for the current curve";
					}
				}
				else // Check for non-bezier primitives.
				{
					errorMsg << "non-bezier primitive found " << attribute;
				}
			}
			else
			{
				errorMsg << "statement " <<  attribute << " has no reason to be here";
			}

			if("" != errorMsg.str())
			{
				std::stringstream globalMsg;
				globalMsg << "Error in file " << pObjFilepath << " , line : " << lineNum << "\n";
				globalMsg << errorMsg.str();
				MExit(globalMsg.str());
			}
		}

		return true;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool ObjLoader::tryParseVertex( std::istringstream& pVertexLine, Vertex& pOutVertex, std::ostringstream& pErrorMsg)const
	{
		// Looking for Vertexs in pVertexLine.
		Bezier::Real value;
		int i=0;
		while (pVertexLine >> value)
		{
			pOutVertex._Data[i] = value;
			++i;
		}
		//MOut("Vertex : " << pOutVertex._Data[0] <<  "  " << pOutVertex._Data[1] << "  " << pOutVertex._Data[2] <<  "  " << pOutVertex._Data[3]);

		// Check for errors.
		if(pOutVertex._Data[3] <= Real(0))
		{
			pErrorMsg << "A positive W value is recommended. Using zero or negative values may result in an undefined point in a curve or surface.";
			return false;
		}
		return true;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool ObjLoader::tryParseType(std::istringstream& pTypeLine, std::ostringstream& pErrorMsg )
	{
		const std::string& line = pTypeLine.str();
		if( (std::string::npos == line.find("bezier")) && (std::string::npos == line.find("Bezier")) )
		{
			pErrorMsg << "Only \"bezier\" or \"Bezier\" are allowed as " << CSTYPE;
			return false;
		}
		
		_CurveDataCache._IsRational = (std::string::npos != line.find(RATIONAL));

		return true;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	void ObjLoader::resetStatements()
	{
		std::fill(_Statementfounds.begin(), _Statementfounds.end(), false);
	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool ObjLoader::allStatementsFound(std::ostringstream& pErrorMsg ) const
	{
		if(std::count(_Statementfounds.begin(), _Statementfounds.end(), true) == _Statementfounds.size())
		{
			return true;
		}
		
		// create error message.
		pErrorMsg << "Can't create the curve because some statements were missed : ";
		for (unsigned int i = 0; i < _Statementfounds.size(); ++i)
		{
			if(! _Statementfounds[i])
			{
				std::string statementMissed = ""; 
				switch(i)
				{
				case CSTYPE_IND: statementMissed = CSTYPE; break;
				case CTECH_IND: statementMissed = CTECH; break;
				case DEG_IND: statementMissed = DEG; break;
				case CURV_IND: statementMissed = CURV; break;
				case PARM_IND: statementMissed = PARM; break;
				case END_IND: statementMissed = END; break;
				}
				pErrorMsg << statementMissed << " ";
			}
		}
		return false;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool ObjLoader::tryParseDegree( std::istringstream& pDegreeLine, std::ostringstream& pErrorMsg )
	{
		// Looking for u only.
		int maxDeg = maxDegreeForCurves();
		int deg = -1;
		if(! (pDegreeLine >> deg))
		{
			pErrorMsg << "Can't read deg value";
			return false;
		}
		if( (deg < MIN_DEGREE) || (deg > maxDeg) ) // 2 control points min in Obj specif.
		{
			pErrorMsg << "deg value must be C [ " << MIN_DEGREE << " ; " << maxDeg << " ]";
			return false;
		}

		_CurveDataCache._Degree = static_cast<unsigned int>(deg);
		return true;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool ObjLoader::tryParseTechnique( std::istringstream& pTechniqueLine, std::ostringstream& pErrorMsg )
	{
		// Parse Technique.
		std::string ctech;
		if(! (pTechniqueLine >> ctech))
		{
			pErrorMsg << "Can't read ctech parameter";
			return false;
		}
		// set Technique and parse args.
		if( "cparm" == ctech)
		{
			_CurveDataCache._ApproximationTech = Bezier::ConstantParametricSubdivision;
			// Looking for res.
			int res = -1;
			if(! (pTechniqueLine >> res))
			{
				pErrorMsg << "Can't read res value";
				return false;
			}
			if(res < 0)
			{
				pErrorMsg << "res must be > 0";
				return false;
			}
			_CurveDataCache._Resolution = static_cast<unsigned int>(res);
		}
		else if("cspace" == ctech)
		{
			_CurveDataCache._ApproximationTech = Bezier::ConstantSpaceSubdivision;
			// Looking for maxlength.
			Real maxlength(-1);
			if(! (pTechniqueLine >> maxlength))
			{
				pErrorMsg << "Can't read maxlength value";
				return false;
			}
			if(maxlength < Real(0))
			{
				pErrorMsg <<  "maxlength must be > 0";
				return false;
			}
			_CurveDataCache._MaxLength = maxlength;
		}
		else if("curv" == ctech)
		{
			_CurveDataCache._ApproximationTech = Bezier::CurvatureDependentSubdivision;
			// Looking for maxdist and maxangle.
			Real maxdist(-1), maxangle(90);
			if(! (pTechniqueLine >> maxdist >> maxangle))
			{
				pErrorMsg << "Can't read maxdist or maxangle value";
				return false;
			}
			if(maxdist < Real(0))
			{
				pErrorMsg << "maxdist must be > 0";
				return false;
			}
			if( (maxangle > 360) || (maxangle < 0) )
			{
				pErrorMsg << "Please specify a value for maxangle [0;360]";
				return false;
			}
			_CurveDataCache._MaxDist = maxdist;
			_CurveDataCache._MaxAngle = maxangle;
		}
		else
		{
			pErrorMsg << "ctech only allows \"cparm\" or \"cspace\" or \"curv\" as parameter";
			return false;
		}
		return true;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool ObjLoader::tryParseCurve( std::istringstream& pCurveLine, std::ostringstream& pErrorMsg )
	{
		// Parse Range.
		Real start(0), end(0);
		if(! (pCurveLine >> start >> end))
		{
			pErrorMsg << "Wrong float value format found when parsing curv range[u0 u1]";
			return false;
		}
		_CurveDataCache._Range = Range(start, end);

		// Parse Control points.
		std::vector<int> vertexIndexs;
		{
			int vertexIndex = -1;
			while(pCurveLine >> vertexIndex)
			{
				if(vertexIndex < 1)
				{
					pErrorMsg <<  "negative or 0 value for vertex index is not allowed";
					return false;
				}
				else
				{
					vertexIndexs.push_back(vertexIndex);
				}
				vertexIndex = -1; // to check if parsing failed.
			}
		}
		
		if(vertexIndexs.size() < MIN_CtrlPts)
		{
			pErrorMsg <<  "A minimum of 2 control points are required for a curve";
			return false;
		}

		// Check if a vertex index try to reference a non-existing vertex.
		int maxVertexInd = *std::max_element(vertexIndexs.begin(), vertexIndexs.end());
		if(maxVertexInd > (int)_VertexCache.size())
		{
			pErrorMsg <<  "Can't reference vertex #" << maxVertexInd << " , there are only " << _VertexCache.size() << " vertex declared on top of the current curve";
			return false;
		}

		if(_CurveDataCache._Degree >= vertexIndexs.size())
		{
			pErrorMsg <<  "A minimum of deg+1 control points are required for a Bezier curve";
			return false;
		}
		
		// Fill Control Points
		_CurveDataCache._ControlPoints.clear();
		for (unsigned int i = 0; i < vertexIndexs.size(); ++i)
		{
			unsigned int vertInd = vertexIndexs[i];
			try
			{
				// vertex index start from index 1 in the obj file.
				const Vertex& vertex = _VertexCache[vertInd-1];
				Vector3 coord(vertex._Data[0], vertex._Data[1], vertex._Data[2]);
				Real weight(vertex._Data[3]);
				if( (! _CurveDataCache._IsRational) && (weight != Real(1)) )
				{
					pErrorMsg <<  "A non-rational curve couldn't have control points with : W != 1.0";
					return false;
				}
				_CurveDataCache._ControlPoints.push_back(ControlPoint(vertInd, coord, weight));
			}
			catch (std::exception e)
			{
				pErrorMsg <<  "Trying to access a vertex with a wrong index " << vertInd;
				return false;
			}
		}
		return true;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	bool ObjLoader::tryParseParm( std::istringstream& pParmLine, std::ostringstream& pErrorMsg )
	{
		std::string param;
		if(! (pParmLine >> param))
		{
			pErrorMsg << "can't read parm parameter";
			return false;
		}
		if("u" != param)
		{
			pErrorMsg << "curves only allows \"u\" as parameter";
			return false;
		}

		// Parse special points : the ones which must already be on the curve. 
		std::vector<Real> specialPoints;
		Real spPoint(_CurveDataCache._Range.getLowerBound() - Real(1)); // init out of range.
		Real previous = std::numeric_limits<Real>::lowest();
		while(pParmLine >> spPoint)
		{
			if(_CurveDataCache._Range.includes(spPoint))
			{
				if(previous >= spPoint)
				{
					pErrorMsg << "u value must increase monotonically";
					return false;
				}
				specialPoints.push_back(spPoint);
				previous = spPoint;
			}
			else
			{
				pErrorMsg << "u value " << spPoint << " can't be out of curv range : " << _CurveDataCache._Range;
				return false;
			}
		}

		// Check u parameter consistency.
		if(specialPoints.size() < MIN_CtrlPts)
		{
			pErrorMsg << "A minimum of two values are required for u";
			return false;
		}
		if( specialPoints.size() != ((_CurveDataCache._ControlPoints.size()/_CurveDataCache._Degree)+1) )
		{
			pErrorMsg << "u must have a number of ((k/n) + 1) values";
		}
		// Check Consistency with the number of control points.
		unsigned int nbcurves = specialPoints.size()-1;
		if( ( (_CurveDataCache._ControlPoints.size()-1) % _CurveDataCache._Degree ) != 0)
		{
			pErrorMsg << "Can't create " << nbcurves << " Bezier curves of degree " << _CurveDataCache._Degree << 
				" with a set of " << _CurveDataCache._ControlPoints.size() << " control points";
			return false;
		}

		// Create SubCurve Ranges for our Bezier Curve.
		{
			_CurveDataCache._SubCurvesRanges.resize(nbcurves);
			// Check acceptable Length for a subRange.
			Real Goodlength = _CurveDataCache._Range.getLength() / nbcurves;
			if(1 == nbcurves)
			{
				Goodlength = _CurveDataCache._Range.getLength();
			}			
			for (unsigned int i=0; i < nbcurves; ++i)
			{
				Range subRange (specialPoints[i], specialPoints[i+1]);
				if(subRange.getLength() != Goodlength)
				{
					pErrorMsg << "Can't create a range with u values [" << subRange.getLowerBound() <<
						" ; " << subRange.getUpperBound() << 
						"].\nTo ensure sub curves have the same degree, the range length must be " << Goodlength;
					return false;
				}
				_CurveDataCache._SubCurvesRanges[i] = subRange;
			}
		}

		return true;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	unsigned int ObjLoader::maxDegreeForCurves()
	{
		static const unsigned int maxDeg = 20; 
		return maxDeg;
	}

	//--------------------------------------------------------------------------------------------------------------------//
	//const std::vector<std::string>& ObjLoader::getRequiredStatements()
	//{
	//	static bool init = false;
	//	static std::vector<std::string> requiredstatements;
	//	if(! init)
	//	{
	//		requiredstatements.push_back(CSTYPE);
	//		requiredstatements.push_back(CTECH);
	//		requiredstatements.push_back(DEG);
	//		requiredstatements.push_back(CURV);
	//		requiredstatements.push_back(PARM);
	//		requiredstatements.push_back(END);
	//		init = true;
	//	}
	//	return requiredstatements;
	//}

	//--------------------------------------------------------------------------------------------------------------------//
	//void ObjLoader::clearStatementsDone()
	//{
	//	StatementList::iterator iter = _StatementsDone.begin();
	//	StatementList::iterator end = _StatementsDone.end();
	//	for (; iter != end; ++iter)
	//	{
	//		iter->second = false;
	//	}
	//}

	//--------------------------------------------------------------------------------------------------------------------//
}