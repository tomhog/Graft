#include "GraftScene.h"

#include <osgViewer/ViewerEventHandlers>
#include <osgGA/OrbitManipulator>
#include <osgGA/StateSetManipulator>

GraftScene::GraftScene()
 : osg::Group()
{
    _modelRoot = new osg::Group();

    _2dCamera = new osg::Camera();
    _2dCamera->setReferenceFrame(::osg::Transform::ABSOLUTE_RF);
    _2dCamera->setProjectionMatrixAsOrtho2D(0.0,1.0,0.0,1.0);
    _2dCamera->setViewMatrix(::osg::Matrix::identity());
    _2dCamera->setRenderOrder(::osg::Camera::POST_RENDER);
    _2dCamera->setComputeNearFarMode(osg::Camera::ComputeNearFarMode::DO_NOT_COMPUTE_NEAR_FAR);
    _2dCamera->setClearColor(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
    _2dCamera->setClearMask(0);

    this->addChild(_modelRoot.get());
    this->addChild(_2dCamera.get());
}

GraftScene::~GraftScene()
{

}

//
// view a 3d model, clearing existing if any
//
void GraftScene::setModel(osg::Node* aNode)
{
    clear();
    _modelRoot->addChild(aNode);
    _previewObject = aNode;
}

//
// view an image view an ortho 2d camera, clearing existing if any
//
void GraftScene::setImage(osg::Image* anImage)
{
    clear();

    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(0.0f,0.0f,1.0f), osg::Vec3(1.0f,0.0f,0.0f), osg::Vec3(0.0f,1.0f,0.0f), 1.0f, 1.0f));

    osg::Texture2D* texture = new osg::Texture2D;
    texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
    texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
    texture->setResizeNonPowerOfTwoHint(false);

    texture->setImage(anImage);

    // set up the drawstate.
    osg::StateSet* dstate = new osg::StateSet;
    dstate->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
    dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    dstate->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);

    geode->setStateSet(dstate);
    _2dCamera->addChild(geode);

    if(anImage->s() >= anImage->t())
    {
        float aspect = (float)anImage->s()/(float)anImage->t();
        _2dCamera->setProjectionMatrixAsOrtho2D(0.0,1.0f,0.0,1.0f*aspect);
    } else {
        float aspect = (float)anImage->t()/(float)anImage->s();
        _2dCamera->setProjectionMatrixAsOrtho2D(0.0,1.0f*aspect,0.0,1.0f);
    }

    _previewObject = anImage;
}

//
// get the preview object
//
osg::Object* GraftScene::getObject()
{
    return _previewObject.get();
}

//
// returns preview object cast to a node
//
osg::Node* GraftScene::getNode()
{
    return dynamic_cast<osg::Node*>(_previewObject.get());
}

//
// returns preview object cast to an image
//
osg::Image* GraftScene::getImage()
{
    return dynamic_cast<osg::Image*>(_previewObject.get());
}

//
// remove all nodes form model root and 2dcamera
//
void GraftScene::clear()
{
    _previewObject = NULL;
    _modelRoot->removeChildren(0, _modelRoot->getNumChildren());
    _2dCamera->removeChildren(0, _2dCamera->getNumChildren());
}

//
// handle resize event, adjusting ortho 2d camera etc
//
void GraftScene::resize(osg::Vec2 aViewSize)
{
    _2dCamera->setViewport(0,0, aViewSize.x(), aViewSize.y());

    // get image aspect
    osg::Image* img = this->getImage();
    if(img != NULL)
    {
        if(img->s() >= img->t())
        {
            float aspect = (float)img->s()/(float)img->t();
            _2dCamera->setProjectionMatrixAsOrtho2D(0.0,1.0f,0.0,1.0f*aspect);
        } else {
            float aspect = (float)img->t()/(float)img->s();
            _2dCamera->setProjectionMatrixAsOrtho2D(0.0,1.0f*aspect,0.0,1.0f);
        }
    }

   //
}
