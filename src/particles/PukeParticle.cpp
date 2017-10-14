#include <Utils.h>
#include "PukeParticle.h"

chag::float3 PukeParticle::initialPosition() {
    float x = getRand(-0.1f, 0.1f);
    float y = getRand(-0.1f, 0.1f);
    float z = getRand(-0.1f, 0.1f);
    return chag::make_vector(x, y, z);
}

chag::float3 PukeParticle::initialVelocity() {
    float x = getRand(-20.0f, 20.0f);
    float y = getRand(-20.0f, 20.0f);
    float z = getRand(-20.0f, 20.0f);
    return chag::make_vector(-40.0f + x, 15.0f + y, 0.0f + z);
}

chag::float3 PukeParticle::accelerate(chag::float3 velocity, float dt) {
    return velocity + chag::make_vector(0.0f, -35.0f, 0.0f) * dt;
}

float PukeParticle::calcLifetime() {
    return getRand(0.0f, 4000.0f);
}

chag::float3 PukeParticle::calcParticleScale() {
    return chag::make_vector(0.1f, 0.1f, 0.1f);
}

bool PukeParticle::loop(float dt) {
    return looping;
}
