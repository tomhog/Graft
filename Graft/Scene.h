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

#ifndef GRAFTSCENE_H
#define GRAFTSCENE_H

#include <Graft/Export.h>

#include <osg/Group>
#include <osg/Image>

namespace graft
{

//
// group node for viewing either a 3D model or a 2D image
//
class GRAFT_EXPORT Scene : public osg::Group
{
public:
    Scene();
    ~Scene();

    // view a 3d model, clearing existing if any
    void setModel(osg::Node* aNode);

    // view an image view an ortho 2d camera, clearing existing if any
    void setImage(osg::Image* anImage);

    // get the preview object
    osg::Object* getObject();

    // returns preview object cast to a node
    osg::Node* getNode();

    // returns preview object cast to an image
    osg::Image* getImage();

    // remove all nodes form model root and 2dcamera
    void clear();

    // handle resize event, adjusting ortho 2d camera etc
    void resize(osg::Vec2 aViewSize);

private:
    // node for attaching 3d model
    osg::ref_ptr<osg::Group> _modelRoot;

    // ortho 2d camera for viewing images
    osg::ref_ptr<osg::Camera> _2dCamera;

    // a model or image being viewed
    osg::ref_ptr<osg::Object> _previewObject;

    osg::Vec2 _viewSize;
};


} // end graft namesapce

#endif // GRAFTSCENE_H
