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
#include <osg/NodeVisitor>
#include <osgUtil/TangentSpaceGenerator>

class GenerateTangentsGeometryVisitor
    : public osg::NodeVisitor
{
public:

    GenerateTangentsGeometryVisitor(bool generateTangents=true, bool generateBinormals=true, unsigned int tangentChannel=6, unsigned int binormalChannel=7, unsigned int uvChannel=0)
        : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ),
        _generateTangents(generateTangents),
        _generateBinormals(generateBinormals),
        _tangentArrayIndex(tangentChannel),
        _binormalArrayIndex(binormalChannel),
        _normalMapUVChannel(uvChannel)
    {
    }

    virtual void apply( osg::Geode & geode )
    {
        for( unsigned int ii = 0; ii < geode.getNumDrawables(); ++ii )
        {
            osg::ref_ptr< osg::Geometry > geometry = dynamic_cast< osg::Geometry * >( geode.getDrawable( ii ) );
            if( geometry.valid() )
            {
                osg::ref_ptr<osgUtil::TangentSpaceGenerator> tangentGenerator = new osgUtil::TangentSpaceGenerator();
                tangentGenerator->generate(geometry, _normalMapUVChannel);

                osg::Vec4Array* tangentArray = tangentGenerator->getTangentArray();
                osg::Vec4Array* biNormalArray = tangentGenerator->getBinormalArray();

                if(tangentArray != NULL && tangentArray->size() > 0 && _generateTangents)
                {
                    geometry->setVertexAttribArray(_tangentArrayIndex, tangentArray);
                    geometry->setVertexAttribBinding(_tangentArrayIndex, osg::Geometry::BIND_PER_VERTEX);
                }

                if(biNormalArray != NULL && biNormalArray->size() > 0 && _generateBinormals)
                {
                    geometry->setVertexAttribArray(_binormalArrayIndex, biNormalArray);
                    geometry->setVertexAttribBinding(_binormalArrayIndex, osg::Geometry::BIND_PER_VERTEX);
                }
            }
        }
    }

    virtual void apply( osg::Node & node )
    {
        traverse( node );
    }

protected:
    bool _generateTangents;
    bool _generateBinormals;
    unsigned int _tangentArrayIndex;
    unsigned int _binormalArrayIndex;

    unsigned int _normalMapUVChannel;

};

namespace graft {

class GenerateTangentsVisitorAction : public graft::VisitorAction
{
public:
    GenerateTangentsVisitorAction()
        : graft::VisitorAction(),
          _generateTangents(true),
          _generateBinormals(true),
          _tangentArrayIndex(6),
          _binormalArrayIndex(7),
          _normalMapUVChannel(0)
    {}

    GenerateTangentsVisitorAction(const GenerateTangentsVisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::VisitorAction(op, copyop),
       _generateTangents(op._generateTangents),
       _generateBinormals(op._generateBinormals),
       _tangentArrayIndex(op._tangentArrayIndex),
       _binormalArrayIndex(op._binormalArrayIndex),
       _normalMapUVChannel(op._normalMapUVChannel)
    {
    }

    META_Object(graft,GenerateTangentsVisitorAction)

    virtual std::string friendlyName(){ return "Generate Tangents"; }
    virtual std::string description() { return "Generate tangent and binormal vectors for each geometry"; }
    virtual std::string category(){ return "Model"; }

    virtual osg::NodeVisitor* allocateVisitor(){ return new GenerateTangentsGeometryVisitor(_generateTangents, _generateBinormals, _tangentArrayIndex, _binormalArrayIndex, _normalMapUVChannel); }

    //
    // serializable properties
    //
    inline void setGenerateTangents(bool generateTangents) { _generateTangents = generateTangents; }
    inline bool getGenerateTangents() const { return _generateTangents; }

    inline void setGenerateBinormals(bool generateBinormals) { _generateBinormals = generateBinormals; }
    inline bool getGenerateBinormals() const { return _generateBinormals; }

    inline void setTangentIndex(unsigned int aChannel) { _tangentArrayIndex = aChannel; }
    inline unsigned int getTangentIndex() const { return _tangentArrayIndex; }

    inline void setBinormalIndex(unsigned int aChannel) { _binormalArrayIndex = aChannel; }
    inline unsigned int getBinormalIndex() const { return _binormalArrayIndex; }

    inline void setUVChannel(unsigned int aChannel) { _normalMapUVChannel = aChannel; }
    inline unsigned int getUVChannel() const { return _normalMapUVChannel; }

protected:
    bool _generateTangents;
    bool _generateBinormals;
    unsigned int _tangentArrayIndex;
    unsigned int _binormalArrayIndex;

    unsigned int _normalMapUVChannel;
};

} // end graft

REGISTER_OBJECT_WRAPPER( graft_GenerateTangentsVisitorAction,
                         new graft::GenerateTangentsVisitorAction,
                         graft::GenerateTangentsVisitorAction,
                         "osg::Object graft::Action graft::GenerateTangentsVisitorAction" )
{
    ADD_BOOL_SERIALIZER( GenerateTangents,  true);  // _generateTangents
    ADD_BOOL_SERIALIZER( GenerateBinormals,  true);  // _generateBinormals
    ADD_UINT_SERIALIZER( TangentIndex,  6);  // _tangentArrayIndex
    ADD_UINT_SERIALIZER( BinormalIndex,  7);  // _binormalArrayIndex
    ADD_UINT_SERIALIZER( UVChannel,  0);  // _normalMapUVChannel
}
