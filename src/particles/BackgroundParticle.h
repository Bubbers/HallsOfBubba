#ifndef BUBBA_3D_FIREPARTICLE_H
#define BUBBA_3D_FIREPARTICLE_H

#include "ParticleConf.h"

using namespace chag;

class BackgroundParticle : public ParticleConf {
public:
    BackgroundParticle() {}
    ~BackgroundParticle() {}

    float3 initialPosition();
    float3 initialVelocity();

    float3 accelerate(float3 velocity, float dt);
    float calcLifetime();
    float3 calcParticleScale();
    bool loop(float dt);

    GLuint blendFunc = GL_ONE;
private:
};


#endif //BUBBA_3D_FIREPARTICLE_H
