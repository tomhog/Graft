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

#include <Graft/PluginRegistry.h>
#include <Graft/NamedNodesAction.h>

namespace graft {

class RenameNodesAction : public graft::NamedNodesAction
{
public:

    enum Mode {
        APPEND = 0,
        PREPEND,
        REPLACE,
        ABSOLUTE
    };

    RenameNodesAction()
        : graft::NamedNodesAction(),
        _mode(APPEND),
        _string("")
    {}

    RenameNodesAction(const RenameNodesAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::NamedNodesAction(op, copyop),
       _mode(op._mode),
       _string(op._string)
    {
    }

    META_Object(graft,RenameNodesAction)

    virtual std::string friendlyName(){ return "Rename Nodes"; }
    virtual std::string description(){ return "Rename a set of nodes matching the search";}
    virtual std::string category(){ return "Model"; }

    virtual void processNamedNode(ActionData* aData, osg::Node* aNode)
    {
        if(aNode == NULL)
            return;

        switch(_mode)
        {
        case APPEND:
            aNode->setName(aNode->getName()+_string);
            break;
        case PREPEND:
            aNode->setName(_string + aNode->getName());
            break;
        case REPLACE:
        {
            std::string newName = aNode->getName();
            size_t start = aNode->getName().find(_searchString);
            newName.replace(start, _searchString.size(), _string);
            aNode->setName(newName);
        }
            break;
        case ABSOLUTE:
            aNode->setName(_string);
            break;
        }
    }


    //
    // Serializable
    //
    inline void setMode(Mode aMode) { _mode = aMode; }
    inline Mode getMode() const { return _mode; }

    inline void setString(const std::string& aString) { _string = aString; }
    inline const std::string& getString() const { return _string; }

protected:
    Mode _mode;
    std::string _string;
};

} //end graft

REGISTER_OBJECT_WRAPPER( graft_RenameNodesAction,
                         new graft::RenameNodesAction,
                         graft::RenameNodesAction,
                         "osg::Object graft::Action graft::NamedNodesAction graft::RenameNodesAction" )
{
    BEGIN_ENUM_SERIALIZER( Mode, APPEND );
        ADD_ENUM_VALUE( APPEND );
        ADD_ENUM_VALUE( PREPEND );
        ADD_ENUM_VALUE( REPLACE );
        ADD_ENUM_VALUE( ABSOLUTE );
    END_ENUM_SERIALIZER();  // _mode

    ADD_STRING_SERIALIZER( String, "" );  // _string
}
