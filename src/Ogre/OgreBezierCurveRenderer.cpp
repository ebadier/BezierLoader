#include "OgreFramework.h"
#include "BezierCurveData.h"

#include "OgreBezierCurveRenderer.h"

//--------------------------------------------------------------------------------------------------------------------//
OgreBezierCurveRenderer::OgreBezierCurveRenderer( unsigned int pMaxDegree ): Bezier::CurveRenderer(pMaxDegree), _Curves()
{

}

//--------------------------------------------------------------------------------------------------------------------//
OgreBezierCurveRenderer::~OgreBezierCurveRenderer()
{

}

//--------------------------------------------------------------------------------------------------------------------//
const std::vector<Ogre::SceneNode*>& OgreBezierCurveRenderer::getCurves() const
{
	return _Curves;
}

//--------------------------------------------------------------------------------------------------------------------//
void OgreBezierCurveRenderer::renderCurve( const Bezier::CurveData& pCurveData )
{
	// Compute Points on curve.
	std::vector<Bezier::Vector3> points;
	switch (pCurveData._ApproximationTech)
	{
	case Bezier::ConstantParametricSubdivision: _CurveSolver.constantParametricSubdivision(pCurveData, points); break;
	case Bezier::ConstantSpaceSubdivision: _CurveSolver.constantSpaceSubdivision(pCurveData, points); break;
	case Bezier::CurvatureDependentSubdivision: _CurveSolver.curvatureDependentSubdivision(pCurveData, points); break;
	}

	// create a global scene Node for both Control Points and the curve.
	Ogre::SceneManager* sceneMgr = OgreFramework::getSingleton().m_pSceneMgr;
	Ogre::stringstream curveName; 
	curveName << "Curv_" << pCurveData._ID;
	Ogre::SceneNode* curveNode = sceneMgr->getRootSceneNode()->createChildSceneNode(curveName.str());

	// Create a colour for the Curve and Control points.
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(curveName.str() + "Mat", "General");
	Ogre::ColourValue colour;
	{
		colour.r = Ogre::Math::RangeRandom(0.1f, 1.0f);
		colour.g = Ogre::Math::RangeRandom(0.1f, 1.0f);
		colour.b = Ogre::Math::RangeRandom(0.1f, 1.0f);
		material->getTechnique( 0 )->getPass( 0 )->setDiffuse(colour);
		material->getTechnique( 0 )->getPass( 0 )->setAmbient(colour);
	}
	
	// Render Control Points.
	{
		Ogre::stringstream ctrlPtNamePrefix;
		ctrlPtNamePrefix << "CP_" << pCurveData._ID;
		const std::vector<Bezier::ControlPoint>& ctrlpts = pCurveData._ControlPoints;
		for (unsigned int i = 0; i < ctrlpts.size(); ++i)
		{
			const Bezier::ControlPoint& ctrlpt = ctrlpts[i];
			Ogre::stringstream ctrlPtName;
			ctrlPtName << ctrlPtNamePrefix.str() << "_" << ctrlpt.getID();
			Ogre::Entity* ctrlPtEnt = sceneMgr->createEntity(ctrlPtName.str(), "sphere.mesh");
			ctrlPtEnt->setMaterialName(material->getName());

			Ogre::SceneNode* ctrlPtNode = curveNode->createChildSceneNode(ctrlPtName.str() + "Node");
			ctrlPtNode->attachObject(ctrlPtEnt);
			ctrlPtNode->setScale(0.0005f, 0.0005f, 0.0005f);
			const Bezier::Vector3& ctrlPtPos = ctrlpt.getVector3();
			ctrlPtNode->setPosition(ctrlPtPos.X(), ctrlPtPos.Y(), ctrlPtPos.Z());
		}
	}
	
	// Render the curve.
	{
		Ogre::ManualObject* curveObject =  sceneMgr->createManualObject(curveName.str());
		Ogre::SceneNode* curveObjectNode = curveNode->createChildSceneNode(curveName.str() + "ObjectNode");
		curveObject->begin(material->getName(), Ogre::RenderOperation::OT_LINE_STRIP);
		for (unsigned int i = 0; i < points.size(); ++i)
		{
			const Bezier::Vector3& pt = points[i];
			curveObject->position(pt.X() , pt.Y(), pt.Z()); 
		}
		curveObject->end(); 
		curveObjectNode->attachObject(curveObject);
	}
	curveNode->_update(true, true); // build AABB to have a chance to center the camera on a curve later.
	//curveNode->showBoundingBox(true);
	_Curves.push_back(curveNode);
}

//--------------------------------------------------------------------------------------------------------------------//