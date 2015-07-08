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

#include <osg/Texture2D>
#include <osgDB/ReadFile>


namespace graft {

class AddShaderProgramAction : public graft::NamedNodesAction
{
public:

    AddShaderProgramAction()
        : graft::NamedNodesAction(),
        _vertexShaderPath(""),
        _fragmentShaderPath("")
    {}

    AddShaderProgramAction(const AddShaderProgramAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::NamedNodesAction(op, copyop),
       _vertexShaderPath(op._vertexShaderPath),
       _fragmentShaderPath(op._fragmentShaderPath)
    {
    }

    META_Object(graft,AddShaderProgramAction)

    virtual std::string friendlyName(){ return "Add Shader Program"; }
    virtual std::string description(){ return "Load a vertex and or fragment shader for all nodes matching search";}
    virtual std::string category(){ return "StateSet"; }

    virtual void processNamedNode(ActionData* aData, osg::Node* aNode)
    {
        if(aNode == NULL || !_program.valid())
            return;

        aNode->getOrCreateStateSet()->setAttribute(_program.get(), osg::StateAttribute::ON);
    }

    // overload process to load the image
    virtual void process(ActionData* aData)
    {
        _program = new osg::Program();
        if(!_vertexShaderPath.empty())
            _program->addShader(osgDB::readShaderFile(osg::Shader::VERTEX, _vertexShaderPath));

        if(!_fragmentShaderPath.empty())
            _program->addShader(osgDB::readShaderFile(osg::Shader::FRAGMENT, _fragmentShaderPath));

        NamedNodesAction::process(aData);
    }

    //
    // Serializable
    //
    inline void setVertexShaderPath(const std::string& aPath) { _vertexShaderPath = aPath; }
    inline const std::string& getVertexShaderPath() const { return _vertexShaderPath; }

    inline void setFragmentShaderPath(const std::string& aPath) { _fragmentShaderPath = aPath; }
    inline const std::string& getFragmentShaderPath() const { return _fragmentShaderPath; }

protected:
    std::string _vertexShaderPath;
    std::string _fragmentShaderPath;

    osg::ref_ptr<osg::Program> _program;
};

} //end graft

REGISTER_OBJECT_WRAPPER( graft_AddShaderProgramAction,
                         new graft::AddShaderProgramAction,
                         graft::AddShaderProgramAction,
                         "osg::Object graft::Action graft::NamedNodesAction graft::AddShaderProgramAction" )
{
    ADD_STRING_SERIALIZER( VertexShaderPath, "" );  // _vertexShaderPath
    ADD_STRING_SERIALIZER( FragmentShaderPath, "" );  // _fragmentShaderPath
}
