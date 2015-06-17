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

namespace hbx {

osg::Image* createSubImage(int sCol, int sRow, int width, int height, osg::Image* source)
{
    //check source is safe
    if(source==NULL)
        return NULL;

    if(sCol + width > source->s() || sRow + height > source->t())
        return NULL;

    //get the number of elements in a pixel
    int pixelSizeInBytes = source->getPixelSizeInBits() / (CHAR_BIT * sizeof(unsigned char));
    unsigned int widthInBytes = width * pixelSizeInBytes;
    unsigned int srcWidthInBytes = source->s() * pixelSizeInBytes;

    //create the pixel buufer for the new image
    unsigned char* subBuf = new unsigned char[(width*height)*pixelSizeInBytes];

    unsigned char* src = source->data() + (sRow*widthInBytes) + (sCol*pixelSizeInBytes);

    //loop all the rows  or y dir of the source image
    for(int row=0; row<height; row++)
    {
        unsigned char* dstRowStart =  subBuf + (row*widthInBytes);
        //copy widthInBytes into subBuf
        memcpy(dstRowStart, src, widthInBytes);
        src += srcWidthInBytes;
    }

    //create the new image
    osg::Image* subImage = new osg::Image();

    subImage->allocateImage(width, height, 1, source->getPixelFormat(), GL_UNSIGNED_BYTE, 1);
    subImage->setImage( width,height ,1, source->getInternalTextureFormat(), source->getPixelFormat(),
                        GL_UNSIGNED_BYTE, subBuf, osg::Image::NO_DELETE, 1);

    return subImage;
}


class CropImageToSquare : public hbx::Action
{
public:
    CropImageToSquare()
        : hbx::Action()
    {}

    CropImageToSquare(const CropImageToSquare& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::Action(op, copyop)
    {
    }

    META_Object(hbx,CropImageToSquare)

    virtual std::string friendlyName(){ return "Crop Image To Square"; }
    virtual std::string description(){ return "Make Square based on shortest dimension";}
    virtual std::string category(){ return "Image"; }

    virtual void process(ActionData* aData) {
        if(aData->asImage() == NULL) return;
        osg::Image* img = aData->asImage();

        int shortest = osg::minimum<int>(img->s(), img->t());

        osg::ref_ptr<osg::Image> cropped = createSubImage(0,0, shortest, shortest, img);

        aData->_object = cropped.get();
    }

protected:
};

} //end hbx

REGISTER_OBJECT_WRAPPER( hbx_CropImageToSquare,
                         new hbx::CropImageToSquare,
                         hbx::CropImageToSquare,
                         "osg::Object hbx::Action hbx::CropImageToSquare" )
{
}
