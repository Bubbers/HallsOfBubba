

#pragma once

#include <ParticleConf.h>

class TorchParticle : public ParticleConf {
public:
    TorchParticle() {}
    ~TorchParticle() {}

    chag::float3 initialPosition();
    chag::float3 initialVelocity();

    chag::float3 accelerate(chag::float3 velocity, float dt);
    float calcLifetime();
    chag::float3 calcParticleScale();
    bool loop(float dt);

    GLuint blendFunc = GL_ONE;
private:
};




