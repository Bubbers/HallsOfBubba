//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_PLAYEROBJECT_H
#define HALLSOFBUBBA_PLAYEROBJECT_H


#include <MoveComponent.h>

class PlayerController : public IComponent {

public:
    virtual void update(float dt);

};


#endif //HALLSOFBUBBA_PLAYEROBJECT_H
