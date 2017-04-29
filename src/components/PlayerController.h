//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_PLAYEROBJECT_H
#define HALLSOFBUBBA_PLAYEROBJECT_H


#include <MoveComponent.h>
#include <ObjectIdentifiers.h>

class PlayerController : public IComponent {

public:
    PlayerController(std::function<void(GameObject*)> spawnBulletFunc);

    virtual void update(float dt) override;

private:
    std::function<void(GameObject*)> spawnBulletFunc;
};


#endif //HALLSOFBUBBA_PLAYEROBJECT_H
