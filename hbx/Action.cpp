#include <hbx/Action.h>

#include <osgDB/ObjectWrapper>

using namespace hbx;

REGISTER_OBJECT_WRAPPER( hbx_Action,
                         /*new hbx::Action*/ NULL,
                         hbx::Action,
                         "osg::Object hbx::Action" )
{
    ADD_BOOL_SERIALIZER( Enabled, true );
}



