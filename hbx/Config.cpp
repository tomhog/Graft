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
}
