#include <Graft/Scene.h>

#include <osg/BlendEquation>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/OrbitManipulator>
#include <osgGA/StateSetManipulator>

#include <osg/io_utils>

using namespace graft;

Scene::Scene()
 : osg::Group()
{
    _modelRoot = new osg::Group();

    _2dCamera = new osg::Camera();
    _2dCamera->setReferenceFrame(::osg::Transform::ABSOLUTE_RF);
    _2dCamera->setProjectionMatrixAsOrtho2D(0.0,1.0,0.0,1.0);
    _2dCamera->setViewMatrix(::osg::Matrix::identity());
    _2dCamera->setRenderOrder(::osg::Camera::POST_RENDER);
    _2dCamera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
    _2dCamera->setClearColor(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
    _2dCamera->setClearMask(0);

    this->addChild(_modelRoot.get());
    this->addChild(_2dCamera.get());
}

Scene::~Scene()
{

}

//
// view a 3d model, clearing existing if any
//
void Scene::setModel(osg::Node* aNode)
{
    clear();
    _modelRoot->addChild(aNode);
    _previewObject = aNode;
}

//
// view an image view an ortho 2d camera, clearing existing if any
//
void Scene::setImage(osg::Image* anImage)
{
    clear();

    if(anImage == NULL)
        return;

    float w = anImage->s();
    float h = anImage->t();

    // clamp width and height to current viewport dimensions
    if(w > _viewSize.x() || h > _viewSize.y())
    {
        if(w >= h)
        {
            float aspect = h / w;
            w = _viewSize.x();
            h = w * aspect;

            if(h > _viewSize.y())
            {
                aspect = w / h;
                h = _viewSize.y();
                w = h * aspect;
            }

        } else {
            float aspect = w / h;
            h = _viewSize.y();
            w = h * aspect;

            if(w > _viewSize.x())
            {
                aspect = h / w;
                w = _viewSize.x();
                h = w * aspect;
            }
        }
    }

    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(-(w*0.5f),-(h*0.5f),1.0f), osg::Vec3(w,0.0f,0.0f), osg::Vec3(0.0f,h,0.0f), 1.0f, 1.0f));

    osg::Texture2D* texture = new osg::Texture2D;
    texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::NEAREST);
    texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::NEAREST);
    texture->setResizeNonPowerOfTwoHint(false);

    texture->setImage(anImage);

    // set up the drawstate.
    osg::StateSet* dstate = new osg::StateSet;
    dstate->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
    dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    dstate->setMode(GL_BLEND, osg::StateAttribute::ON);
    dstate->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON);
    dstate->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    dstate->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);

    geode->setStateSet(dstate);
    _2dCamera->addChild(geode);

    _previewObject = anImage;
}

//
// get the preview object
//
osg::Object* Scene::getObject()
{
    return _previewObject.get();
}

//
// returns preview object cast to a node
//
osg::Node* Scene::getNode()
{
    return dynamic_cast<osg::Node*>(_previewObject.get());
}

//
// returns preview object cast to an image
//
osg::Image* Scene::getImage()
{
    return dynamic_cast<osg::Image*>(_previewObject.get());
}

//
// remove all nodes form model root and 2dcamera
//
void Scene::clear()
{
    _previewObject = NULL;
    _modelRoot->removeChildren(0, _modelRoot->getNumChildren());
    _2dCamera->removeChildren(0, _2dCamera->getNumChildren());
}

//
// handle resize event, adjusting ortho 2d camera etc
//
void Scene::resize(osg::Vec2 aViewSize)
{
    _viewSize = aViewSize;
    _2dCamera->setViewport(0,0, aViewSize.x(), aViewSize.y());

    osg::Vec2 hvs = aViewSize * 0.5f;
    _2dCamera->setProjectionMatrixAsOrtho2D(-hvs.x(),hvs.x(),-hvs.y(),hvs.y());

    if(this->getImage() != NULL)
        setImage(this->getImage());
   //
}
