
#include "Utils.h"
#include "FirstAttackParticle.h"

chag::float3 FirstAttackParticle::initialPosition() {
    float rand = getRand(0.0f, 360.0f);
    float rand2  = getRand(0.0f, 0.03f);
    return chag::make_vector((float)cos(rand) * rand2 * 2, (float)sin(rand) * rand2, (float)sin(rand) * rand2 * 2);
}

chag::float3 FirstAttackParticle::initialVelocity() {
    return chag::make_vector(getRand(-.5f, .5f),0.0f, getRand(-.5f, .5f));
}

chag::float3 FirstAttackParticle::accelerate(chag::float3 velocity) {
    return chag::make_vector(0.0f, 00.1f, 0.0f);
}

float FirstAttackParticle::calcLifetime() {
    return getRand(0.0f,3000.0f);
}

chag::float3 FirstAttackParticle::calcParticleScale() {
    return chag::make_vector(.05f, .05f, .05f);
}

bool FirstAttackParticle::loop(float dt) {
    return looping;
}

void FirstAttackParticle::setLooping(bool value){
    looping = value;
}