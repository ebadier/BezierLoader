#ifndef BezierObjLoader_H
#define BezierObjLoader_H

#include "BezierCurveData.h"

namespace Bezier
{
	///\class ObjLoader load Bezier curves from an .obj file.
	/// The .obj file must respect these rules:
	/// #vertex could be declared anywhere in the file, but on top of where they are referenced.
	/// #Negatives index for vertex reference is not allowed.
	///	#You can declare any Bezier curve as you want one after each other.
	/// bezier curve declaration must start with "cstype" and must finish with "end". 
	///	v -2.300000 1.950000 0.000000
	///	v -2.200000 0.790000 0.000000
	///	v -2.340000 -1.510000 0.000000
	///	v -1.530000 -1.490000 0.000000
	///	v -0.720000 -1.470000 0.000000
	///	v -0.780000 0.230000 0.000000
	///	v 0.070000 0.250000 0.000000
	///	v 0.920000 0.270000 0.000000
	///	v 0.800000 -1.610000 0.000000
	///	v 1.620000 -1.590000 0.000000
	///	v 2.440000 -1.570000 0.000000
	///	v 2.690000 0.670000 0.000000
	///	v 2.900000 1.980000 0.000000
	///	cstype bezier
	///	ctech cparm 1.000000 
	///	deg 3
	///	v 0.092000 0.250000 0.000000
	///	curv 0.000000 4.000000 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	///	parm u 0.000000 1.000000 2.000000 3.000000 4.000000
	///	end
	/// ...
	/// Be careful : lod/usemtl/mtllib/shadow_obj/trace_obj are not managed for the moment.
	class ObjLoader
	{
	private:
		// copy is not allowed.
		ObjLoader(const ObjLoader&);
		ObjLoader& operator=(const ObjLoader&);
		
		///\brief Helper to collect vertex infos when parsing.
		struct Vertex
		{
			///\brief Constructor.
			/// init _Data with default values (0,0,0,1).
			Vertex();
			Real _Data[4]; ///< x, y, z, w.
		};

		///\brief try to parse a vertex of any dimensions from a line.
		bool tryParseVertex(std::istringstream& pVertexLine, Vertex& pOutVertex, std::ostringstream& pErrorMsg)const;
		///\brief try to parse the curve type.
		bool tryParseType(std::istringstream& pTypeLine, std::ostringstream& pErrorMsg);
		///\brief try to parse the curve degree.
		bool tryParseDegree(std::istringstream& pDegreeLine, std::ostringstream& pErrorMsg);
		///\brief try to parse Approximation Technique.
		bool tryParseTechnique(std::istringstream& pTechniqueLine, std::ostringstream& pErrorMsg);
		///\brief try to parse the control points and the range.
		bool tryParseCurve(std::istringstream& pCurveLine, std::ostringstream& pErrorMsg);
		///\brief try to parse the parameter of the curve.
		bool tryParseParm(std::istringstream& pParmLine, std::ostringstream& pErrorMsg);
		///\brief check this method after the end statement is reached.
		///\return true if the parameters are valid, false otherwise.
		//bool isCurveDataValid(std::string& pErrorMsg)const;
		///\brief fill _Statementfounds with false.
		void resetStatements();
		///\brief return whether all required statements were found.
		bool allStatementsFound(std::ostringstream& pErrorMsg)const;

	public:
		ObjLoader();
		~ObjLoader();

		///\brief get the max degree allowed by .obj format for curves.
		static unsigned int maxDegreeForCurves();

		///\brief load some CurveData from an .obj file.
		///\param pObjFilepath : the full path to the .obj file.
		///\param pOutCurves : the data describing the curves.
		///\return false if loading failed or if curves are not valid, true otherwise.
		/// launch an Exception if failed.
		bool loadCurves(const std::string& pObjFilepath, std::vector<CurveData>& pOutCurves);
		
	private:
		std::vector<Vertex>			_VertexCache; ///< Useful to stock vertexs when parsing.
		CurveData					_CurveDataCache; ///< stock infos on the curve being parsed.
		std::vector<bool>			_Statementfounds; ///< to check if statements cstype/ctech/deg/curv/parm (in this order) were parsed before parsing a new curve.
	};
}

#endif

