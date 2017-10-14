

#pragma once


#include "FirstAttackParticle.h"

class SecondAttackParticle : public FirstAttackParticle{
public:
    chag::float3 initialVelocity() override;

    chag::float3 accelerate(chag::float3 velocity, float dt) override;

};



