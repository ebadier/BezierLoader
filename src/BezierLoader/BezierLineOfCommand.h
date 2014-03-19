#ifndef BezierLineOfCommand_H
#define BezierLineOfCommand_H

#include <string>
#include "BezierStringHlp.h"

/// \brief Cette classe est faite pour fournir un acces simple a la ligne de commande
/// utilisation simple : 
/// sur la ligne de commande : prog.exe -fichierConfig config1.txt 
/// std::string fichierconfig = LineOfCommand::lookNowForString("fichierConfig","configDefaut.txt");
/// les fonctions lookNowForInt et lookNowForFloat existent aussi
/// il est possible aussi de faire : LineOfCommand::isInTheCommandLine("monoption");
namespace Bezier
{
	class LineOfCommand
	{
	private:
		///\brief interdit
		LineOfCommand(void);
		///\brief interdit
		~LineOfCommand(void);
		///\brief interdit
		LineOfCommand(const LineOfCommand&);
		///\brief interdit
		LineOfCommand& operator=(const LineOfCommand&);

	public:
		/// \brief recherche immediate dans la ligne de commande de '-pName' et renvoit l'element suivant
		static int lookNowForInt(const std::string& pName, int pDefaut = 0)
		{
			int sArgc = getArgc();
			char** sArgv = getArgv();

			int res = pDefaut;

			int arg = isInTheCommandLine(pName);
			if(arg && sArgc>arg+1)
			{
				res = StringHlp::parseInt(sArgv[arg+1]);
			}
			return res;
		}

		/// \brief recherche immediate dans la ligne de commande de '-pName' et renvoit l'element suivant 
		static float lookNowForFloat(const std::string& pName, float pDefaut = 0.0)
		{
			int sArgc = getArgc();
			char** sArgv = getArgv();

			float res = pDefaut;
			int arg = isInTheCommandLine(pName);
			if(arg&& sArgc>arg+1)
			{
				res = StringHlp::parseFloat(sArgv[arg+1]);
			}
			return res;
		}
		/// \brief recherche immediate dans la ligne de commande de '-pName' et renvoit l'element suivant. 
		static std::string lookNowForString(const std::string& pName, const std::string& pDefaut="")
		{
			int sArgc = getArgc();
			char** sArgv = getArgv();

			std::string res = pDefaut;
			int arg = isInTheCommandLine(pName);
			if(arg&&sArgc>arg+1)
			{
				res = sArgv[arg+1];
			}
			return res;
		}

		/// \brief recherche immediate dans la ligne de commande de -pName
		/// renvoit 0 si pas trouve
		/// renvoit l'indice si trouve (indice >1);
		static int indexInTheCommandLine(const std::string& pName)
		{
			int sArgc = getArgc();
			char** sArgv = getArgv();

			int res = 0;
			bool found = false;
			for(int arg = 1; arg<sArgc && !found; arg++)
			{
				if(sArgv[arg][0]=='-')
				{
					std::string argWithScore = sArgv[arg];
					std::string argWithoutScore = argWithScore.substr(1);
					if(pName==argWithoutScore)
					{
						res = arg;
						found = true;
					}
				}
			}
			return res;
		}
		static bool isInTheCommandLine(const std::string& pName)
		{
			return (indexInTheCommandLine(pName)) > 0 ;
		}

		///\brief acces a l'argc traditionnel
		static int getArgc(){return __argc;}

		/// \brief acces a l'argv traditionnel
		static char** getArgv(){return __argv;}
	};
}
#endif