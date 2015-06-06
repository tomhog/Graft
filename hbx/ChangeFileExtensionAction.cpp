#include "ChangeFileExtensionAction.h"

using namespace hbx;

REGISTER_OBJECT_WRAPPER( hbx_ChangeFileExtensionAction,
                         new hbx::ChangeFileExtensionAction,
                         hbx::ChangeFileExtensionAction,
                         "osg::Object hbx::Action hbx::ChangeFileExtensionAction" )
{
    BEGIN_ENUM_SERIALIZER( Type, NODE );
        ADD_ENUM_VALUE( NODE );
        ADD_ENUM_VALUE( IMAGE );
    END_ENUM_SERIALIZER();  // _mode

    ADD_STRING_SERIALIZER( Extension, "" );  // _extension
}
