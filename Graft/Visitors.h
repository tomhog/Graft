#ifndef GRAFT_VISITORS_H
#define GRAFT_VISITORS_H

#include <Graft/Export.h>

#include <osg/NodeVisitor>
#include <osg/Texture2D>

namespace graft
{

typedef std::set< osg::ref_ptr<osg::Texture> > TextureSet;

class GRAFT_EXPORT FindTexturesVisitor : public osg::NodeVisitor
{
public:

    FindTexturesVisitor():
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
    {}

    virtual void apply(osg::Node& node)
    {
        if (node.getStateSet()) apply(*node.getStateSet());
        traverse(node);
    }

    virtual void apply(osg::Geode& node)
    {
        if (node.getStateSet()) apply(*node.getStateSet());

        for(unsigned int i=0;i<node.getNumDrawables();++i)
        {
            osg::Drawable* drawable = node.getDrawable(i);
            if (drawable && drawable->getStateSet()) apply(*drawable->getStateSet());
        }

        traverse(node);
    }

    virtual void apply(osg::StateSet& stateset)
    {
        // search for the existence of any texture object attributes
        for(unsigned int i=0;i<stateset.getTextureAttributeList().size();++i)
        {
            osg::Texture* texture = dynamic_cast<osg::Texture*>(stateset.getTextureAttribute(i,osg::StateAttribute::TEXTURE));
            if (texture)
            {
                _textureSet.insert(texture);
            }
        }
    }

    TextureSet                          _textureSet;
};


} // end graft namespace

#endif // GRAFT_VISITORS_H

