#include "FileOutputAction.h"

using namespace hbx;

REGISTER_OBJECT_WRAPPER( hbx_FileOutputAction,
                         new hbx::FileOutputAction,
                         hbx::FileOutputAction,
                         "osg::Object hbx::Action hbx::FileOutputAction" )
{
    ADD_STRING_SERIALIZER( String, "_conv" );  // _string
    BEGIN_ENUM_SERIALIZER( Mode, APPEND );
        ADD_ENUM_VALUE( OVERWRITE );
        ADD_ENUM_VALUE( APPEND );
        ADD_ENUM_VALUE( SET_EXTENSION );
    END_ENUM_SERIALIZER();  // _mode
}
