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

#include <hbx/PluginRegistry.h>
#include <hbx/Visitors.h>

#include <osg/NodeVisitor>
#include <osg/Texture2D>
#include <osgDB/FileNameUtils>

namespace hbx {

class LocaliseTexturesVisitorAction : public hbx::Action
{
public:

    LocaliseTexturesVisitorAction()
        : hbx::Action(),
          _folder("Images")
    {}

    LocaliseTexturesVisitorAction(const LocaliseTexturesVisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::Action(op, copyop),
       _folder(op._folder)
    {
    }

    META_Object(hbx,LocaliseTexturesVisitorAction)

    virtual std::string friendlyName(){ return "Localise Image Paths"; }
    virtual std::string description() { return "Localise Texture Image file paths with optional folder name"; }
    virtual std::string category(){ return "Model"; }

    virtual void process(ActionData* aData)
    {
        if(aData->asNode() == NULL) return;

        FindTexturesVisitor findTex;
        aData->asNode()->accept(findTex);
        if(findTex._textureSet.size() == 0) return;

        for(TextureSet::iterator itr=findTex._textureSet.begin();
            itr!=findTex._textureSet.end();
            ++itr)
        {
            osg::Texture* texture = const_cast<osg::Texture*>(itr->get());
            osg::Texture2D* texture2D = dynamic_cast<osg::Texture2D*>(texture);
            if(texture2D == NULL) continue;
            osg::Image* img = texture2D->getImage();
            if(img == NULL) continue;

            std::string local = "./" + (_folder.length() > 0) ? _folder + "/" : "./";
            std::string localname = local + osgDB::getSimpleFileName(img->getFileName());
            img->setFileName(localname);
        }
    }


    //
    // serializable properties
    //
    inline void setFolder(const std::string& aFolder) { _folder = aFolder; }
    inline const std::string& getFolder() const { return _folder; }

protected:
    std::string _folder;
};

} // end hbx

REGISTER_OBJECT_WRAPPER( hbx_LocaliseTexturesVisitorAction,
                         new hbx::LocaliseTexturesVisitorAction,
                         hbx::LocaliseTexturesVisitorAction,
                         "osg::Object hbx::Action hbx::LocaliseTexturesVisitorAction" )
{
    ADD_STRING_SERIALIZER( Folder, "Images" );  // _folder
}
