#include "FileInputAction.h"

using namespace hbx;

REGISTER_OBJECT_WRAPPER( hbx_FileInputAction,
                         new hbx::FileInputAction,
                         hbx::FileInputAction,
                         "osg::Object hbx::Action hbx::FileInputAction" )
{
    ADD_STRING_SERIALIZER( FilePath, "" );  // _filePath
}
