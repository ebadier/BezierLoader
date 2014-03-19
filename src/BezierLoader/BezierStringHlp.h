#ifndef BezierStringHlp_H
#define BezierStringHlp_H

#include <sstream>
#include <vector>
#include <string>

namespace Bezier
{
	///\brief cette classe contient des methodes d'aide pour manipuler des strings.
	class StringHlp
	{
	public:
		template<typename T>
		static T parse(const std::string& val)
		{
			std::istringstream str(val);
			T res;
			str >> res;
			return res;
		}

		static int parseInt(const std::string& val)
		{
			std::istringstream str(val);
			int res = 0;
			str >> res;
			return res;
		}
		static float parseFloat(const std::string& val)
		{
			std::istringstream str(val);
			float res = 0.0;
			str >> res;
			return res;
		}
		static bool parseBool(const std::string& val)
		{
			bool res = false;
			if("true"==val||"1"==val||"yes"==val||"YES"==val||"Yes"==val||"Ok"==val||"ok"==val||"OK"==val||"TRUE"==val||"True"==val||"o"==val)
			{
				res=true;
			}else{
				res = false;
			}
			return res;
		}

		/// on utilise une fonction de decoupage de string pour obtenir facilement une liste
		/// le decoupage se fait selon le caractere ','
		/// la string contient l ensemble des separateurs possibles
		static std::vector<std::string> splitInVector(const std::string& pStringToSplit, std::string separators=",")
		{
			std::vector<std::string> resultat;
			std::string lStringToSplit = pStringToSplit;

			int n = 0 ; //Position courante sur la string
			int next = 0 ; //Position suivante sur la string
			while(next != -1)
			{
				//On recupere le nom de la prochaine node XML :
				next = (int)lStringToSplit.find_first_of(separators,n);
				std::string part = lStringToSplit.substr(n, next-n);
				resultat.push_back(part);
				n = next+1 ;
			}
			return resultat;
		}

		/// la string contient 1 separateur
		static std::vector<std::string> splitInVectorV2(const std::string& pStringToSplit, std::string separator=",")
		{
			std::vector<std::string> resultat;
			std::string lStringToSplit = pStringToSplit;

			int n = 0 ; //Position courante sur la string
			int next = 0 ; //Position suivante sur la string
			while(next != std::string::npos)
			{
				//On recupere le nom
				next = (int)lStringToSplit.find(separator,n);
				std::string part = lStringToSplit.substr(n, next-n);
				resultat.push_back(part);
				n = next+separator.size() ;
			}
			return resultat;
		}

		//--------------------------------------------------------------------//		
		//--------------------------------------------------------------------//
		//----------------AIDE POUR LES NOMS DE FICHIERS----------------------//
		//--------------------------------------------------------------------//
		//--------------------------------------------------------------------//

		static std::string extract_extention( const std::string& p_path)
		{
			// Extraction de l'extension du fichier
			size_t ext_pos = p_path.find_last_of( '.' );
			std::string img_ext = p_path.substr( ext_pos+1 );
			return img_ext;
		}

		static std::string extract_fileName( const std::string& p_path)
		{
			// on essaie slash et anti-slash
			size_t ext_pos = p_path.find_last_of( '/' );
			if(ext_pos<2) 
			{
				ext_pos = p_path.find_last_of( '\\' );
			}
			std::string img_ext = p_path.substr( ext_pos+1 );
			return img_ext;
		}

		static std::string extract_path( const std::string& p_path)
		{
			// on essaie slash puis anti-slash
			size_t ext_pos = p_path.find_last_of( '/' );
			if(ext_pos<2) 
			{
				ext_pos = p_path.find_last_of( '\\' );
			}
			std::string img_ext = p_path.substr( 0,ext_pos );
			return img_ext;
		}

		/// \brief cette fonction renvoie le nom sans l extension ni le point.
		/// exemple : "phto.jpg" donnera "phto"
		static std::string extract_name_before_point( const std::string& p_path)
		{
			// Extraction de l'extension du fichier
			size_t ext_pos = p_path.find_last_of( '.' );
			std::string img_ext = p_path.substr(0, ext_pos);
			return img_ext;
		}
	};

}
#endif