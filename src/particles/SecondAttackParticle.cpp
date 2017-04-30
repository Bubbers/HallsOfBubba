
#include "Utils.h"
#include "SecondAttackParticle.h"

chag::float3 SecondAttackParticle::initialPosition() {
    float rand = getRand(0.0f, 360.0f);
    float rand2  = getRand(0.0f, 0.03f);
    return chag::make_vector((float)cos(rand) * rand2 * 2, (float)sin(rand) * rand2, (float)sin(rand) * rand2 * 2);
}

chag::float3 SecondAttackParticle::initialVelocity() {
    return chag::make_vector(getRand(-.5f, .5f), getRand(0.0f, 100.0f), getRand(-.5f, .5f));
}

chag::float3 SecondAttackParticle::accelerate(chag::float3 velocity) {
    return chag::make_vector(0.0f, 50.1f, 0.0f);
}

float SecondAttackParticle::calcLifetime() {
    return getRand(0.0f,3000.0f);
}

chag::float3 SecondAttackParticle::calcParticleScale() {
    return chag::make_vector(.05f, .05f, .05f);
}

bool SecondAttackParticle::loop(float dt) {
    return looping;
}

void SecondAttackParticle::setLooping(bool value){
    looping = value;
}