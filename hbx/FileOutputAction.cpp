#include "FileOutputAction.h"

using namespace hbx;

REGISTER_OBJECT_WRAPPER( hbx_FileOutputAction,
                         new hbx::FileOutputAction,
                         hbx::FileOutputAction,
                         "osg::Object hbx::Action hbx::FileOutputAction" )
{
    ADD_BOOL_SERIALIZER( SaveTextures, false );
}
