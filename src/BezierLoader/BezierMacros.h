#ifndef Bezier_Macros_H
#define Bezier_Macros_H

#include <iostream>
#include <sstream>
#ifdef _MSC_VER
	#include <windows.h>
#endif

///\brief Log Things.
#define MFileLineFunc(X)	__FILE__ << "(" << __LINE__ << ") [" << __FUNCTION__ << "] -> " << X
#define MOut(X)				std::cout << X << std::endl;
#define MOut_Er(X)			MOut(MFileLineFunc(X))
#ifdef _MSC_VER
	#define MPopup(X)		{ std::stringstream ss; ss << X; MessageBox(NULL, ss.str().c_str(),"",MB_OK); }
	#define MPause			system("pause");
#else
	#define MPopup(X)
	#define MPause			std::cout << "Press Enter to continue..."; std::cin.get();
#endif
#define MOutAndPopup(X)		MOut(X) MPopup(X)
#define MWarning(X)			MOutAndPopup(MFileLineFunc(X))

///\brief Exit throwing an exception.
#ifdef _MSC_VER	
	#define MExit(X)	{std::stringstream ss; ss << MFileLineFunc("Error: " << X); \
						throw std::exception(ss.str().c_str()); }
#else
	#define MExit(X)	{std::stringstream ss; ss << MFileLineFunc("Error: " << X); \
						throw std::runtime_error(ss.str().c_str()); }
#endif

///\brief Assertions with different severity consequences (Log/Popup/exception).
#define MTest_E_MSG(XTest,YMessage)			if(XTest){MExit(YMessage);}
#define MTest_E(XTest)						MTest_E_MSG( XTest, #XTest )
#define MTest_W(XTest,YMessage,RetValue)	if(XTest){MWarning(YMessage); return RetValue;}
#define MTest_O(XTest,YMessage,RetValue)	if(XTest){MOut_Er(YMessage); return RetValue;}

///\brief use the following macros to enable/disable assertions regarding debug/release mode.
#ifdef NDEBUG
	#define MDebugOut(X)
	#define MDebugOut_Er(X)
	#define MDebugWarning(X)
	#define MDebugTest_E_MSG(XTest,YMessage)
	#define MDebugTest_E(XTest)
	#define MDebugTest_W(XTest,YMessage,RetValue)
	#define MDebugTest_O(XTest,YMessage,RetValue)
#else
	#define MDebugOut			MOut
	#define MDebugOut_Er		MOut_Er
	#define MDebugWarning		MWarning
	#define MDebugTest_E_MSG	MTest_E_MSG
	#define MDebugTest_E		MTest_E
	#define MDebugTest_W		MTest_W
	#define MDebugTest_O		MTest_O
#endif

#endif