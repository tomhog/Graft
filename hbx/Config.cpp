#include <hbx/Config.h>

using namespace hbx;

REGISTER_OBJECT_WRAPPER( ConfigObject,
                         new hbx::ConfigObject,
                         hbx::ConfigObject,
                         "osg::Object hbx::ConfigObject" )
{
    ADD_INT_SERIALIZER( Precision, 2 );  // _precision
    ADD_STRING_SERIALIZER( LastImportDirectory, "" );  // _lastImportDirectory
    ADD_STRING_SERIALIZER( LastExportDirectory, "" );  // _lastExportDirectory
    ADD_VEC4_SERIALIZER(ClearColor, osg::Vec4(77.0f/255.0f, 184.0f/255.0f, 219.0f/255.0f, 0.0f));
}
