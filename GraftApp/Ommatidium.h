#ifndef OMMATIDIUM_H
#define OMMATIDIUM_H

#include <osg/Camera>
#include <osg/io_utils>

class Ommatidium
{
public:

    class Camera
    {
    public:
        osg::Matrix _viewMatrix;
        osg::Matrix _projectionMatrix;

        void computeViewProjectionMatrix()
        {
            _viewProjectionMatrix = _viewMatrix * _projectionMatrix;
            _invViewProjectionMatrix = osg::Matrix::inverse(_viewProjectionMatrix);
        }

        osg::Vec4 worldToClip(osg::Vec3 aWorldPoint)
        {
            osg::Vec4 clipPoint = osg::Vec4(aWorldPoint.x(), aWorldPoint.y(), aWorldPoint.z(), 1.0f) * _viewProjectionMatrix;
            clipPoint.w() = 1.0f;// / clipPoint.w();
            clipPoint = osg::Vec4(clipPoint.x()/clipPoint.w(), clipPoint.y()/clipPoint.w(), clipPoint.z()/clipPoint.w(), clipPoint.w());
            return clipPoint;
        }

        osg::Vec3 clipToWorld(osg::Vec4 aClipPoint)
        {
            osg::Vec4 projectedPoint = (aClipPoint * _invViewProjectionMatrix);
            projectedPoint.w() = 1.0f;///projectedPoint.w();
            return osg::Vec3(projectedPoint.x()*projectedPoint.w(), projectedPoint.y()*projectedPoint.w(), projectedPoint.z()*projectedPoint.w());
        }

        void clipToRay(osg::Vec2 aClipPoint, osg::Vec3& anOrigin, osg::Vec3& aDirection)
        {
            osg::Vec4 clipPoint = osg::Vec4(aClipPoint.x(), aClipPoint.y(), 0.0f, 1.0f);
            osg::Vec4 clipForward = clipPoint;
            clipForward.z() += 1.0f;

            osg::Vec4 projectedPoint = (clipPoint * _invViewProjectionMatrix);
            projectedPoint = osg::Vec4(projectedPoint.x()/projectedPoint.w(), projectedPoint.y()/projectedPoint.w(), projectedPoint.z()/projectedPoint.w(), projectedPoint.w());

            osg::Vec4 projectedForward = (clipForward * _invViewProjectionMatrix);
            projectedForward = osg::Vec4(projectedForward.x()/projectedForward.w(), projectedForward.y()/projectedForward.w(), projectedForward.z()/projectedForward.w(), projectedForward.w());

            OSG_ALWAYS << "projectedPoint: " << projectedPoint << " projectedForward: " << projectedForward << std::endl;

            osg::Vec4 projectedDelta = projectedForward - projectedPoint;
            osg::Vec3 projectedDirection = osg::Vec3(projectedDelta.x(), projectedDelta.y(), projectedDelta.z());
            projectedDirection.normalize();

            OSG_ALWAYS << "projectedDelta: " << projectedDelta << " projectedDirection: " << projectedDirection << std::endl;

            anOrigin = osg::Vec3(projectedPoint.x(), projectedPoint.y(), projectedPoint.z());
            aDirection = osg::Vec3(projectedDirection.x(), projectedDirection.y(), projectedDirection.z());

            OSG_ALWAYS << "anOrigin: " << anOrigin << " aDirection: " << aDirection << std::endl;
        }

        osg::Vec3 clipToWorld(osg::Vec2 aClipPoint, float aDistance)
        {
            osg::Vec3 origin;
            osg::Vec3 direction;
            clipToRay(aClipPoint, origin, direction);

            OSG_ALWAYS << "Origin: " << origin << " Direction: " << direction << std::endl;
            osg::Vec3 worldPoint = origin + (direction * aDistance);
            return worldPoint;
        }

    protected:
        osg::Matrix _viewProjectionMatrix;
        osg::Matrix _invViewProjectionMatrix;
    };

    Ommatidium()
    {init();}
    virtual ~Ommatidium()
    {}

    void init()
    {
        float ccdw = 640.0f;
        float ccdh = 480.0f;

        float fov = 45.0f;
        float zNear = 0.01f;
        float zFar = 10.0f;
        float zRange = zFar - zNear;
        float aspect = ccdw / ccdh;

        osg::Vec3 forward = osg::Vec3(0.0f,0.0f,-1.0f);
        osg::Vec3 right = osg::Vec3(1.0f,0.0f,0.0f);
        osg::Vec3 up = osg::Vec3(0.0f,1.0f,0.0f);

        //setup camera view matricies, units in meters
        float seperation = 2.0f;
        float offset = seperation * 0.5f;

        //first camera on the left
        osg::Vec3 leftEye = -right * offset;
        _cam1._viewMatrix = osg::Matrix::lookAt(leftEye, leftEye + forward, up);
        _cam1._projectionMatrix = osg::Matrix::perspective(fov, aspect, zNear, zFar);
        _cam1.computeViewProjectionMatrix();

        //second camera to the right
        osg::Vec3 rightEye = right * offset;
        _cam2._viewMatrix = osg::Matrix::lookAt(rightEye, rightEye + forward, up);
        _cam2._projectionMatrix = osg::Matrix::perspective(fov, aspect, zNear, zFar);
        _cam2.computeViewProjectionMatrix();

        //project world point through cam1
        osg::Vec3 worldPoint = forward * zFar;
        OSG_ALWAYS << "StartWorldPoint: " << worldPoint << std::endl;

        osg::Vec4 c1ComputedClipPoint = _cam1.worldToClip(worldPoint);
        OSG_ALWAYS << "c1ComputedClipPoint: " << c1ComputedClipPoint << std::endl;

        //project back to world coords
        osg::Vec3 clComputedWorldPoint = _cam1.clipToWorld(c1ComputedClipPoint);
        OSG_ALWAYS << "clComputedWorldPoint: " << clComputedWorldPoint << std::endl;

        osg::Vec3 clRayWorldPoint = _cam1.clipToWorld(osg::Vec2(c1ComputedClipPoint.x(), c1ComputedClipPoint.y()), 7.0f);//-zNear);
        OSG_ALWAYS << "clRayWorldPoint: " << clRayWorldPoint << std::endl;
    }

    Camera _cam1;
    Camera _cam2;
};

#endif // OMMATIDIUM_H
