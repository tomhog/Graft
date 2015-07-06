#include <Graft/FileInputAction.h>

using namespace graft;

REGISTER_OBJECT_WRAPPER( graft_FileInputAction,
                         new graft::FileInputAction,
                         graft::FileInputAction,
                         "osg::Object graft::Action graft::FileInputAction" )
{
    ADD_STRING_SERIALIZER( FilePath, "" );  // _filePath
}
