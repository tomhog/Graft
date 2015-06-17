/*  -*-c++-*-
 *  Copyright (C) 2015 Thomas Hogarth <tom@hogbox.com>, adapted from osgconv
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#include <hbx/PluginRegistry.h>
#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Node>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgUtil/Optimizer>

class OrientationConverter
{
    public :
        OrientationConverter(void);
        void setRotation( const osg::Vec3 &from,
                          const osg::Vec3 &to  );
        void setRotation( float degrees, const osg::Vec3 &axis  );
        void setTranslation( const osg::Vec3 &trans);
        void setScale( const osg::Vec3 &trans);
        void useWorldFrame( bool worldFrame );

        /** return the root of the updated subgraph as the subgraph
          * the node passed in my flatten during optimization.*/
        osg::Node* convert( osg::Node* node );

    private :
        OrientationConverter( const OrientationConverter& ) {}
        OrientationConverter& operator = (const OrientationConverter& ) { return *this; }

        osg::Matrix R, T, S;
        bool _trans_set;
        bool _use_world_frame;

};

using namespace osg;

OrientationConverter::OrientationConverter( void )
{
   R.makeIdentity();
   T.makeIdentity();
   _trans_set = false;
   _use_world_frame = false;
   S.makeIdentity();
}

void OrientationConverter::setRotation( const Vec3 &from, const Vec3 &to )
{
    R = Matrix::rotate( from, to );
}

void OrientationConverter::setRotation( float degrees, const Vec3 &axis )
{
    R = Matrix::rotate( osg::DegreesToRadians(degrees), axis );
}

void OrientationConverter::setTranslation( const Vec3 &trans )
{
    T = Matrix::translate(trans);
    _trans_set = true;
}

void OrientationConverter::setScale( const Vec3 &scale )
{
    S = Matrix::scale(scale);
}

void OrientationConverter::useWorldFrame( bool worldFrame )
{
   _use_world_frame = worldFrame;
}

Node* OrientationConverter::convert( Node *node )
{
    // Order of operations here is :
    // 1. If world frame option not set, translate to world origin (0,0,0)
    // 2. Rotate to new orientation
    // 3. Scale in new orientation coordinates
    // 4. If an absolute translation was specified then
    //        - translate to absolute translation in world coordinates
    //    else if world frame option not set,
    //        - translate back to model's original origin.
    BoundingSphere bs = node->getBound();
    Matrix C;

    if (_use_world_frame)
    {
        C.makeIdentity();
    }
    else
    {
        C = Matrix::translate( -bs.center() );

        if (_trans_set == false)
            T = Matrix::translate( bs.center() );
    }


    osg::Group* root = new osg::Group;
    osg::MatrixTransform* transform = new osg::MatrixTransform;

    transform->setDataVariance(osg::Object::STATIC);
    transform->setMatrix( C * R * S * T );

    if (!S.isIdentity())
    {
        #if !defined(OSG_GLES2_AVAILABLE)
            // Add a normalize state. This will be removed if the FlattenStaticTransformsVisitor works
            transform->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
        #endif
    }

    root->addChild(transform);
    transform->addChild(node);

    osgUtil::Optimizer::FlattenStaticTransformsVisitor fstv;
    root->accept(fstv);
    fstv.removeTransforms(root);

    return root->getChild(0);
}


namespace hbx {

class OrientationConverterAction : public hbx::Action
{
public:
    OrientationConverterAction()
        : hbx::Action(),
        _useWorldSpace(false),
        _rotationAngle(0.0f),
        _rotationAxis(osg::Y_AXIS),
        _translation(osg::Vec3()),
        _scale(osg::Vec3(1.0f,1.0f,1.0f))
    {}

    OrientationConverterAction(const OrientationConverterAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::Action(op, copyop),
       _useWorldSpace(op._useWorldSpace),
       _rotationAngle(op._rotationAngle),
       _rotationAxis(op._rotationAxis),
       _translation(op._translation),
       _scale(op._scale)
    {
    }

    META_Object(hbx,OrientationConverterAction)

    virtual std::string friendlyName(){ return "Orientation Convertor"; }
    virtual std::string description(){ return "Adjust rotation, scale and translation of the root node";}
    virtual std::string category(){ return "Model"; }

    virtual void process(ActionData* aData) {
        if(aData->asNode() == NULL) return;
        OrientationConverter orientation;
        orientation.setRotation(_rotationAngle, _rotationAxis);
        orientation.setTranslation(_translation);
        orientation.setScale(_scale);
        aData->_object = orientation.convert(aData->asNode());
    }

    //
    // serializable properties
    //
    void setUseWorldSpace(bool useWorldSpace){ _useWorldSpace = useWorldSpace; }
    bool getUseWorldSpace() const { return _useWorldSpace; }

    void setRotationAngle(float anAngle){ _rotationAngle = anAngle; }
    float getRotationAngle() const { return _rotationAngle; }

    void setRotationAxis(const osg::Vec3& anAxis){ _rotationAxis = anAxis; }
    const osg::Vec3& getRotationAxis() const { return _rotationAxis; }

    void setTranslation(const osg::Vec3& aTranslation){ _translation = aTranslation; }
    const osg::Vec3& getTranslation() const { return _translation; }

    void setScale(const osg::Vec3& aScale){ _scale = aScale; }
    const osg::Vec3& getScale() const { return _scale; }

protected:
    bool _useWorldSpace;
    float _rotationAngle;
    osg::Vec3 _rotationAxis;
    osg::Vec3 _translation;
    osg::Vec3 _scale;

};

} //end hbx

REGISTER_OBJECT_WRAPPER( hbx_OrientationConverterAction,
                         new hbx::OrientationConverterAction,
                         hbx::OrientationConverterAction,
                         "osg::Object hbx::Action hbx::OrientationConverterAction" )
{
    ADD_BOOL_SERIALIZER(UseWorldSpace, false);
    ADD_FLOAT_SERIALIZER(RotationAngle, 0.0f);
    ADD_VEC3_SERIALIZER(RotationAxis, osg::Vec3(0.0f,1.0f,0.0f));
    ADD_VEC3_SERIALIZER(Translation, osg::Vec3());
    ADD_VEC3_SERIALIZER(Scale, osg::Vec3(1.0f,1.0f,1.0f));
}
