#include <Graft/ChangeFileExtensionAction.h>

using namespace graft;

REGISTER_OBJECT_WRAPPER( graft_ChangeFileExtensionAction,
                         new graft::ChangeFileExtensionAction,
                         graft::ChangeFileExtensionAction,
                         "osg::Object graft::Action graft::ChangeFileExtensionAction" )
{
    BEGIN_ENUM_SERIALIZER( Type, NODE );
        ADD_ENUM_VALUE( NODE );
        ADD_ENUM_VALUE( IMAGE );
    END_ENUM_SERIALIZER();  // _mode

    ADD_STRING_SERIALIZER( Extension, "" );  // _extension
}
