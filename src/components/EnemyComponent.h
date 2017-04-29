//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_ENEMYCOMPONENT_H
#define HALLSOFBUBBA_ENEMYCOMPONENT_H


#include <IComponent.h>
#include <ObjectIdentifiers.h>

class EnemyComponent : public IComponent {

public:
    EnemyComponent(std::function<void(GameObject*)> bulletSpawner);
    virtual void update(float dt);

private:
    std::function<void(GameObject*)> bulletSpawner;

};


#endif //HALLSOFBUBBA_ENEMYCOMPONENT_H
