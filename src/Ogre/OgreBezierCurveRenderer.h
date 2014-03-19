#ifndef OgreBezierCurveRenderer_H
#define OgreBezierCurveRenderer_H

#include "BezierCurveRenderer.h"

namespace Ogre
{
	class SceneNode;
}

namespace Bezier
{
	class CurveData;
}

///\class OgreBezierCurveRenderer
/// render a Bezier curve with Ogre engine.
class OgreBezierCurveRenderer : public Bezier::CurveRenderer
{
private:
	OgreBezierCurveRenderer(const OgreBezierCurveRenderer&);
	OgreBezierCurveRenderer& operator=(const OgreBezierCurveRenderer&);

public:
	///\brief Constructor.
	///\param pMaxDegree Allows this to be able to manage degree <= pMaxDegree.
	OgreBezierCurveRenderer(unsigned int pMaxDegree);
	virtual ~OgreBezierCurveRenderer();
	///\brief render the curve pCurveData.
	virtual void renderCurve(const Bezier::CurveData& pCurveData);
	///\brief access the rendered curves.
	const std::vector<Ogre::SceneNode*>& getCurves()const;	

private:
	std::vector<Ogre::SceneNode*>	_Curves; ///< cached rendered curves.
};

#endif