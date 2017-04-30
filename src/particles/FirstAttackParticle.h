#pragma once


#include <ParticleConf.h>

class FirstAttackParticle : public ParticleConf{
public:
    FirstAttackParticle() {}
    ~FirstAttackParticle() {}

    chag::float3 initialPosition();
    virtual chag::float3 initialVelocity();

    virtual chag::float3 accelerate(chag::float3 velocity);
    float calcLifetime();
    chag::float3 calcParticleScale();
    bool loop(float dt);

    GLuint blendFunc = GL_ONE;

    void setLooping(bool value);
};



