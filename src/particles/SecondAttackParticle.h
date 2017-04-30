#pragma once


#include <ParticleConf.h>

class SecondAttackParticle : public ParticleConf{
public:
    SecondAttackParticle() {}
    ~SecondAttackParticle() {}

    chag::float3 initialPosition();
    chag::float3 initialVelocity();

    chag::float3 accelerate(chag::float3 velocity);
    float calcLifetime();
    chag::float3 calcParticleScale();
    bool loop(float dt);

    GLuint blendFunc = GL_ONE;

    void setLooping(bool value);
};



