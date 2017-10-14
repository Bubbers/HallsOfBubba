

#include <Utils.h>
#include "SecondAttackParticle.h"

chag::float3 SecondAttackParticle::initialVelocity() {
    return chag::make_vector(getRand(-.5f, .5f), getRand(0.5f, 5.0f), getRand(-.5f, .5f));
}

chag::float3 SecondAttackParticle::accelerate(chag::float3 velocity, float dt) {
    return chag::make_vector(0.0f, 50.1f, 0.0f);
}
