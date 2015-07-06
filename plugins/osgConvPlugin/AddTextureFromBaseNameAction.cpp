/*  -*-c++-*-
 *  Copyright (C) 2015 Thomas Hogarth <tom@hogbox.com>
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

#include <osg/StateSet>
#include <osg/Texture2D>

#include <osgDB/FileNameUtils>

namespace graft {

class FindStateSets : public osg::NodeVisitor
{
public:
    FindStateSets():
    osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
    {}

    void apply(osg::StateSet* stateset)
    {
        if(stateset == NULL) return;

        if(_statesets.find(stateset) == _statesets.end())
            _statesets[stateset] = 0;
        else
            _statesets[stateset] = _statesets[stateset] + 1;
    }

    void apply(osg::Node& node)
    {
        apply(node.getStateSet());
        traverse(node);
    }

    void apply(osg::Geode& geode)
    {
        apply(geode.getStateSet());

        for(unsigned int i = 0; i<geode.getNumChildren(); ++i)
        {
            apply(geode.getDrawable(i)->getStateSet());
        }
    }

public:
     std::map< osg::StateSet*, int > _statesets;
};

class AddTextureFromBasedName : public graft::Action
{
public:

    enum Mode {
        APPEND = 0,
        PREPEND,
        REPLACE_AFTER
    };

    AddTextureFromBasedName()
        : graft::Action(),
        _sourceChannel(0),
        _destinationChannel(1),
        _mode(APPEND),
        _string("")
    {}

    AddTextureFromBasedName(const AddTextureFromBasedName& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::Action(op, copyop),
       _sourceChannel(op._sourceChannel),
       _destinationChannel(op._destinationChannel),
       _mode(op._mode),
       _string(op._string)
    {
    }

    META_Object(graft,AddTextureFromBasedName)

    virtual std::string friendlyName(){ return "Add Sub Texture"; }
    virtual std::string description(){ return "Add a texture using the name of an existing texture";}
    virtual std::string category(){ return "StateSet"; }

    virtual void process(ActionData* aData)
    {
        if(aData->asNode() == NULL)
            return;

        // find all statesets
        osg::ref_ptr<FindStateSets> findStates = new FindStateSets();
        aData->asNode()->accept(*findStates);

        for(std::map< osg::StateSet*, int >::iterator itr = findStates->_statesets.begin();
            itr != findStates->_statesets.end();
            ++itr)
        {
            //see if it has a texture in source channel
            osg::StateSet* ss = (*itr).first;
            osg::StateAttribute* texat = ss->getTextureAttribute(_sourceChannel, osg::StateAttribute::TEXTURE);
            osg::Texture2D* asTex = dynamic_cast<osg::Texture2D*>(texat);
            if(asTex == NULL) continue;
            osg::Image* img = asTex->getImage();
            if(img == NULL) continue;
            std::string srcname = img->getFileName();

            std::string subFilename = getSubFileName(srcname);
            OSG_ALWAYS << "Sub Filename: " << subFilename << std::endl;

            //should cache these
            osg::ref_ptr<osg::Image> subimg = osgDB::readImageFile(subFilename);
            if(!subimg.valid()) continue;

            osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
            texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
            texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
            texture->setResizeNonPowerOfTwoHint(false);
            texture->setImage(subimg);

            ss->setTextureAttribute(_destinationChannel, texture);
        }
    }


    //
    // Serializable
    //

    inline void setSrcChannel(unsigned int aChannel) { _sourceChannel = aChannel; }
    inline unsigned int getSrcChannel() const { return _sourceChannel; }

    inline void setDestChannel(unsigned int aChannel) { _destinationChannel = aChannel; }
    inline unsigned int getDestChannel() const { return _destinationChannel; }

    inline void setMode(Mode aMode) { _mode = aMode; }
    inline Mode getMode() const { return _mode; }

    inline void setString(const std::string& aString) { _string = aString; }
    inline const std::string& getString() const { return _string; }

protected:

    std::string getSubFileName(std::string aSrcFilename)
    {
        switch(_mode)
        {
            case APPEND:
                return osgDB::getNameLessAllExtensions(aSrcFilename) + _string + osgDB::getFileExtensionIncludingDot(aSrcFilename);
            case PREPEND:
                return osgDB::getFilePath(aSrcFilename) + "/" + _string + osgDB::getSimpleFileName(aSrcFilename);
            case REPLACE_AFTER:
            {
                std::string newName = osgDB::getNameLessAllExtensions(aSrcFilename);
                size_t end = newName.find_last_of(_string[0]);
                newName = newName.substr(0, end);
                return newName + _string + osgDB::getFileExtensionIncludingDot(aSrcFilename);
            }
        }
        return aSrcFilename;
    }

protected:
    unsigned int _sourceChannel;
    unsigned int _destinationChannel;

    Mode _mode;
    std::string _string;

    osg::ref_ptr<osg::Texture2D> _texture;
};

} //end graft

REGISTER_OBJECT_WRAPPER( graft_AddTextureFromBasedName,
                         new graft::AddTextureFromBasedName,
                         graft::AddTextureFromBasedName,
                         "osg::Object graft::Action graft::AddTextureFromBasedName" )
{
    ADD_UINT_SERIALIZER( SrcChannel, 0);
    ADD_UINT_SERIALIZER( DestChannel, 1);
    BEGIN_ENUM_SERIALIZER( Mode, APPEND );
        ADD_ENUM_VALUE( APPEND );
        ADD_ENUM_VALUE( PREPEND );
        ADD_ENUM_VALUE( REPLACE_AFTER );
    END_ENUM_SERIALIZER();  // _mode

    ADD_STRING_SERIALIZER( String, "" );  // _string
}
