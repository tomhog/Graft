#include <Graft/NamedNodesAction.h>

using namespace graft;

REGISTER_OBJECT_WRAPPER( graft_NamedNodesAction,
                         /*new graft::NamedNodesAction*/ NULL,
                         graft::NamedNodesAction,
                         "osg::Object graft::Action graft::NamedNodesAction" )
{
    ADD_STRING_SERIALIZER( SearchString, "");
    ADD_BOOL_SERIALIZER( WholeWord, false );
}
