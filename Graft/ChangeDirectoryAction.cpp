#include <Graft/ChangeDirectoryAction.h>

using namespace graft;

REGISTER_OBJECT_WRAPPER( graft_ChangeDirectoryAction,
                         new graft::ChangeDirectoryAction,
                         graft::ChangeDirectoryAction,
                         "osg::Object graft::Action graft::ChangeDirectoryAction" )
{
    ADD_STRING_SERIALIZER( Directory, "" );  // _directory
}
