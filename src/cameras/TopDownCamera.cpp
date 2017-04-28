//
// Created by johan on 2017-04-28.
//

#include "TopDownCamera.h"

TopDownCamera::TopDownCamera(const chag::float3 &centralFloorPoint, int width, int height) :
        PerspectiveCamera(centralFloorPoint + chag::make_vector(0.0f,10.0f,-10.0f),
               centralFloorPoint,
               chag::normalize(chag::make_vector(0.0f,1.0f,1.0f)),
               45,
               float(width) / float(height),
               0.1f, 50000.0f){
}

void TopDownCamera::update(float dt) {

}
