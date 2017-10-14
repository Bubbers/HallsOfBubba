#ifndef HALLSOFBUBBA_PUKEPARTICLE_H
#define HALLSOFBUBBA_PUKEPARTICLE_H


#include <ParticleConf.h>

class PukeParticle : public ParticleConf {


public:
    virtual ~PukeParticle() = default;

    virtual chag::float3 initialPosition();

    virtual chag::float3 initialVelocity();

    virtual chag::float3 accelerate(chag::float3 velocity, float dt);

    virtual float calcLifetime();

    virtual chag::float3 calcParticleScale();

    virtual bool loop(float dt);
};


#endif //HALLSOFBUBBA_PUKEPARTICLE_H
