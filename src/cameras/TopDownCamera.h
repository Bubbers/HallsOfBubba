//
// Created by johan on 2017-04-28.
//

#ifndef HALLSOFBUBBA_TOPDOWNCAMERA_H
#define HALLSOFBUBBA_TOPDOWNCAMERA_H

#include <PerspectiveCamera.h>

class TopDownCamera : public PerspectiveCamera{
public:

    TopDownCamera(const chag::float3 &centralFloorPoint, int width, int height);
    virtual void update(float dt);
};


#endif //HALLSOFBUBBA_TOPDOWNCAMERA_H
