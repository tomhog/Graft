#include "ChangeDirectoryAction.h"

using namespace hbx;

REGISTER_OBJECT_WRAPPER( hbx_ChangeDirectoryAction,
                         new hbx::ChangeDirectoryAction,
                         hbx::ChangeDirectoryAction,
                         "osg::Object hbx::Action hbx::ChangeDirectoryAction" )
{
    ADD_STRING_SERIALIZER( Directory, "" );  // _directory
}
