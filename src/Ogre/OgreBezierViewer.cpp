#include "OgreFramework.h"
#include "BezierMacros.h"
#include "BezierCurveData.h"
#include "OgreBezierViewer.h"

//--------------------------------------------------------------------------------------------------------------------//
OgreBezierViewer::OgreBezierViewer(unsigned int pMaxDegreeForCurves): _CurveRenderer(pMaxDegreeForCurves), _CurveLabel(NULL), _CurrentCurveTracked(0), _Shutdown(false)
{

}

//--------------------------------------------------------------------------------------------------------------------//
OgreBezierViewer::~OgreBezierViewer()
{
	delete OgreFramework::getSingletonPtr();
	_CurveLabel = NULL; // OgreFramework delete it.
}

//--------------------------------------------------------------------------------------------------------------------//
void OgreBezierViewer::startDemo(const std::vector<Bezier::CurveData>& pCurves)
{
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("OgreBezierViewer", this, 0))
	{
		return;
	}
	_Shutdown = false;

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");

	setupDemoScene();

	for (unsigned int i = 0; i < pCurves.size(); ++i)
	{
		_CurveRenderer.renderCurve(pCurves[i]);
	}

	showAllCurves();

	runDemo();
}

//--------------------------------------------------------------------------------------------------------------------//
void OgreBezierViewer::setupDemoScene()
{
	Ogre::SceneManager* sceneMgr = OgreFramework::getSingletonPtr()->m_pSceneMgr;
	OgreFramework::getSingletonPtr()->m_pViewport->setBackgroundColour(Ogre::ColourValue::Black);
	sceneMgr->setAmbientLight(Ogre::ColourValue::White);
	sceneMgr->createLight("Light")->setPosition(75,75,75);

	OgreBites::SdkTrayManager* trayMgr = OgreFramework::getSingletonPtr()->m_pTrayMgr;
	trayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	trayMgr->toggleAdvancedFrameStats();
	trayMgr->hideCursor();
	_CurveLabel = trayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "CurveLabel", "CURVE", 100.0f);
	_CurveLabel->show();
}

//--------------------------------------------------------------------------------------------------------------------//
void OgreBezierViewer::runDemo()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");
	
	double timeSinceLastFrame = 0;
	double startTime = 0;

	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
	OgreFramework::getSingletonPtr()->m_pRenderWnd->setActive(true);
	OgreFramework::getSingletonPtr()->m_pRenderWnd->setDeactivateOnFocusChange(false);

	while(!_Shutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
	{
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())_Shutdown = true;

		Ogre::WindowEventUtilities::messagePump();
	
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
					
			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			OgreFramework::getSingletonPtr()->m_pMouse->capture();

			OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
		
			timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}

//--------------------------------------------------------------------------------------------------------------------//
bool OgreBezierViewer::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

	return true;
}

//--------------------------------------------------------------------------------------------------------------------//
bool OgreBezierViewer::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

	if(keyEventRef.key == OIS::KC_SPACE)
	{
		switchCurve();
	}

	if(keyEventRef.key == OIS::KC_F)
	{
		OgreBites::SdkTrayManager* trayMgr = OgreFramework::getSingletonPtr()->m_pTrayMgr;
		trayMgr->toggleAdvancedFrameStats();
	}

	if(keyEventRef.key == OIS::KC_RETURN)
	{
		showAllCurves();
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------//
void OgreBezierViewer::switchCurve(bool pFirstime)
{
	const std::vector<Ogre::SceneNode*>& renderedCurves = _CurveRenderer.getCurves();
	if(!pFirstime)
	{
		++_CurrentCurveTracked;
		if(_CurrentCurveTracked >= renderedCurves.size())
		{
			_CurrentCurveTracked = 0;
		}
	}
	else
	{
		_CurrentCurveTracked = 0;
	}

	if(! renderedCurves.empty())
	{
		// Disable other curves.
		for (unsigned int i = 0; i < renderedCurves.size(); ++i)
		{
			if(i != _CurrentCurveTracked)
			{
				renderedCurves[i]->setVisible(false, true);
			}
		}

		// look at current curve
		Ogre::Camera* cam = OgreFramework::getSingletonPtr()->m_pCamera;
		Ogre::SceneNode* curve = renderedCurves[_CurrentCurveTracked];
		const Ogre::Vector3& curvePos = curve->_getWorldAABB().getCenter();
		cam->setPosition(curvePos + Ogre::Vector3(0, 0, -10));
		cam->lookAt(curvePos);
		curve->setVisible(true, true);
		// set current curve name visible.
		_CurveLabel->setCaption(curve->getName());
	}
	else
	{
		_CurveLabel->setCaption("No Curves !");
	}
}

//--------------------------------------------------------------------------------------------------------------------//
void OgreBezierViewer::showAllCurves()
{
	// Enable all curves.
	const std::vector<Ogre::SceneNode*>& renderedCurves = _CurveRenderer.getCurves();
	if(! renderedCurves.empty())
	{
		for (unsigned int i = 0; i < renderedCurves.size(); ++i)
		{
			renderedCurves[i]->setVisible(true, true);
		}

		// look at first curve
		_CurrentCurveTracked = 0;
		Ogre::Camera* cam = OgreFramework::getSingletonPtr()->m_pCamera;
		Ogre::SceneNode* curve = renderedCurves[_CurrentCurveTracked];
		const Ogre::Vector3& curvePos = curve->_getWorldAABB().getCenter();
		cam->setPosition(curvePos + Ogre::Vector3(0, 0, -10));
		cam->lookAt(curvePos);
		// set current curve name visible.
		_CurveLabel->setCaption("All Curves");
	}
	else
	{
		_CurveLabel->setCaption("No Curves !");
	}
}

//--------------------------------------------------------------------------------------------------------------------//