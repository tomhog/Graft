#include <Graft/Action.h>

#include <osgDB/ObjectWrapper>

using namespace graft;

REGISTER_OBJECT_WRAPPER( graft_Action,
                         /*new graft::Action*/ NULL,
                         graft::Action,
                         "osg::Object graft::Action" )
{
    ADD_BOOL_SERIALIZER( Enabled, true );
}



