/*  -*-c++-*-
 *  Copyright (C) 2015 Thomas Hogarth <tom@hogbox.com>, adapted from osgconv
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#include <hbx/PluginRegistry.h>
#include <hbx/NamedNodesAction.h>

namespace hbx {

class RemoveNodesAction : public hbx::NamedNodesAction
{
public:

    enum Mode {
        REMOVE_ALL = 0,
        REPARENT_CHILDREN
    };

    RemoveNodesAction()
        : hbx::NamedNodesAction(),
        _mode(REMOVE_ALL)
    {}

    RemoveNodesAction(const RemoveNodesAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::NamedNodesAction(op, copyop),
       _mode(op._mode)
    {
    }

    META_Object(hbx,RemoveNodesAction)

    virtual std::string friendlyName(){ return "Remove Nodes"; }
    virtual std::string description(){ return "Remove a set of nodes matching the search";}
    virtual std::string category(){ return "Model"; }

    virtual void processNamedNode(ActionData* aData, osg::Node* aNode)
    {
        if(aNode == NULL)
            return;

        // get the parent
        osg::Group* nodeAsGroup = aNode->asGroup();
        osg::Node::ParentList parents = aNode->getParents();

        switch(_mode)
        {
            case REPARENT_CHILDREN:
            {
                if(nodeAsGroup != NULL)
                {
                    for(osg::Node::ParentList::iterator itr=parents.begin();
                        itr != parents.end();
                        ++itr)
                    {
                        for(unsigned int c=0; c<nodeAsGroup->getNumChildren(); c++)
                            (*itr)->addChild(nodeAsGroup->getChild(c));
                    }
                }
            }
            //don't break after reparent so we aslo do remove
            case REMOVE_ALL:
            {
                for(osg::Node::ParentList::iterator itr=parents.begin();
                    itr != parents.end();
                    ++itr)
                {
                    (*itr)->removeChild(aNode);
                }
                break;
            }
        }
    }


    //
    // Serializable
    //
    inline void setMode(Mode aMode) { _mode = aMode; }
    inline Mode getMode() const { return _mode; }

protected:
    Mode _mode;
};

} //end hbx

REGISTER_OBJECT_WRAPPER( hbx_RemoveNodesAction,
                         new hbx::RemoveNodesAction,
                         hbx::RemoveNodesAction,
                         "osg::Object hbx::Action hbx::NamedNodesAction hbx::RemoveNodesAction" )
{
    BEGIN_ENUM_SERIALIZER( Mode, REMOVE_ALL );
        ADD_ENUM_VALUE( REMOVE_ALL );
        ADD_ENUM_VALUE( REPARENT_CHILDREN );
    END_ENUM_SERIALIZER();  // _mode
}
