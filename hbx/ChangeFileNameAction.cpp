#include "ChangeFileNameAction.h"

using namespace hbx;

REGISTER_OBJECT_WRAPPER( hbx_ChangeFileNameAction,
                         new hbx::ChangeFileNameAction,
                         hbx::ChangeFileNameAction,
                         "osg::Object hbx::Action hbx::ChangeFileNameAction" )
{
    BEGIN_ENUM_SERIALIZER( Mode, APPEND_STRING );
        ADD_ENUM_VALUE( APPEND_STRING );
        ADD_ENUM_VALUE( PREPEND_STRING );
    END_ENUM_SERIALIZER();  // _mode

    ADD_STRING_SERIALIZER( String, "" );  // _string
}
