#include <Graft/FileOutputAction.h>

using namespace graft;

REGISTER_OBJECT_WRAPPER( graft_FileOutputAction,
                         new graft::FileOutputAction,
                         graft::FileOutputAction,
                         "osg::Object graft::Action graft::FileOutputAction" )
{
    ADD_BOOL_SERIALIZER( SaveTextures, false );
}
