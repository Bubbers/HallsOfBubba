//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_PLAYEROBJECT_H
#define HALLSOFBUBBA_PLAYEROBJECT_H


#include <MoveComponent.h>
#include <ObjectIdentifiers.h>

class PlayerController : public IComponent {

public:
    PlayerController(std::function<void(GameObject*)> spawnBulletFunc, Camera *camera);
    chag::float3 getPlaneIntersectionPoint();

    virtual void update(float dt) override;

private:
    std::function<void(GameObject*)> spawnBulletFunc;
    Camera *camera;
    float timeSinceLastShot = 1.0f;

    chag::float3 getMouseRayInWorldSpace(float mouseX, float mouseY, int width, int height) const;
};


#endif //HALLSOFBUBBA_PLAYEROBJECT_H
