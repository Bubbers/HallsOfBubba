//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_PLAYEROBJECT_H
#define HALLSOFBUBBA_PLAYEROBJECT_H


#include <MoveComponent.h>

class PlayerController : public IComponent {

public:
    PlayerController(std::function<void(chag::float3, chag::float3)> spawnBulletFunc);

    virtual void update(float dt) override;

private:
    std::function<void(chag::float3, chag::float3)> spawnBulletFunc;
};


#endif //HALLSOFBUBBA_PLAYEROBJECT_H
