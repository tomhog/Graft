#include "NamedNodesAction.h"

using namespace hbx;

REGISTER_OBJECT_WRAPPER( hbx_NamedNodesAction,
                         /*new hbx::NamedNodesAction*/ NULL,
                         hbx::NamedNodesAction,
                         "osg::Object hbx::Action hbx::NamedNodesAction" )
{
    ADD_STRING_SERIALIZER( SearchString, "");
    ADD_BOOL_SERIALIZER( WholeWord, false );
}
