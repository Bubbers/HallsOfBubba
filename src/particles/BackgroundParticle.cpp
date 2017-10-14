#include "Utils.h"
#include "BackgroundParticle.h"

float3 BackgroundParticle::initialPosition() {
    float rand = getRand(0.0f, 360.0f);
    float rand2  = getRand(0.0f, 33.3f);
	return make_vector((float)cos(rand) * rand2, (float)sin(rand) * rand2, 0.0f);
}

float3 BackgroundParticle::initialVelocity() {
    return make_vector(getRand(-.5f, .5f), getRand(0.0f, 100.0f), getRand(-.5f, .5f));
}

float3 BackgroundParticle::accelerate(float3 velocity, float dt) {
    return velocity;
}

float BackgroundParticle::calcLifetime() {
    return getRand(0.0f,5000.0f);
}

float3 BackgroundParticle::calcParticleScale() {
    return make_vector(.03f, .03f, .03f);
}

bool BackgroundParticle::loop(float dt) {
    return looping;
}