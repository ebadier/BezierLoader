#ifndef OgreBezierViewer_H
#define OgreBezierViewer_H

#include "OISKeyboard.h"

#include "OgreBezierCurveRenderer.h"

namespace OgreBites
{
	class Label;
}

///\brief a demo of BezierLoader with Ogre rendering.
class OgreBezierViewer : public OIS::KeyListener
{
public:
	///\brief Constructor.
	OgreBezierViewer(unsigned int pMaxDegreeForCurves);
	virtual ~OgreBezierViewer();
	///\brief catch user inputs.
	virtual bool keyPressed(const OIS::KeyEvent &keyEventRef);
	virtual bool keyReleased(const OIS::KeyEvent &keyEventRef);

	///\brief render the curves.
	void startDemo(const std::vector<Bezier::CurveData>& pCurves);

private:
	void setupDemoScene();
	void runDemo();
	void switchCurve(bool pFirstime = false);
	void showAllCurves();

private:
	OgreBezierCurveRenderer		_CurveRenderer; ///< used to Render the curves.
	OgreBites::Label*			_CurveLabel; ///< To show which curve is currently viewed.
	unsigned int				_CurrentCurveTracked; ///< to switch from one curve to another.
	bool						_Shutdown; ///< should we stop ?
};

#endif