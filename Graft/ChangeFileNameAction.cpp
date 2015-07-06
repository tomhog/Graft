#include <Graft/ChangeFileNameAction.h>

using namespace graft;

REGISTER_OBJECT_WRAPPER( graft_ChangeFileNameAction,
                         new graft::ChangeFileNameAction,
                         graft::ChangeFileNameAction,
                         "osg::Object graft::Action graft::ChangeFileNameAction" )
{
    BEGIN_ENUM_SERIALIZER( Mode, APPEND_STRING );
        ADD_ENUM_VALUE( APPEND_STRING );
        ADD_ENUM_VALUE( PREPEND_STRING );
    END_ENUM_SERIALIZER();  // _mode

    ADD_STRING_SERIALIZER( String, "" );  // _string
}
