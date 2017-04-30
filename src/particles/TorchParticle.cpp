#include "Utils.h"
#include "TorchParticle.h"

chag::float3 TorchParticle::initialPosition() {
    float rand = getRand(0.0f, 360.0f);
    float rand2  = getRand(0.0f, 0.5f);
    return chag::make_vector((float)cos(rand) * rand2 * 2, (float)sin(rand) * rand2, (float)sin(rand) * rand2 * 2);
}

chag::float3 TorchParticle::initialVelocity() {
    return chag::make_vector(getRand(-.5f, .5f), getRand(0.0f, 100.0f), getRand(-.5f, .5f));
}

chag::float3 TorchParticle::accelerate(chag::float3 velocity) {
    return chag::make_vector(0.0f, 50.1f, 0.0f);
}

float TorchParticle::calcLifetime() {
    return getRand(0.0f,3000.0f);
}

chag::float3 TorchParticle::calcParticleScale() {
    return chag::make_vector(.20f, .20f, .20f);
}

bool TorchParticle::loop(float dt) {
    return looping;
}

void TorchParticle::setLooping(bool value){
    looping = value;
}