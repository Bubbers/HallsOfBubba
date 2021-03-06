//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_ENEMYCOMPONENT_H
#define HALLSOFBUBBA_ENEMYCOMPONENT_H


#include <IComponent.h>
#include <ObjectIdentifiers.h>
#include <linmath/float3.h>
#include <functional>
#include <GameObject.h>

class EnemyComponent : public IComponent {

public:
    EnemyComponent(std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> bulletSpawner,
                   std::vector<std::shared_ptr<GameObject>> playerObjects);
    virtual void update(float dt) override;

    virtual void beforeCollision(std::shared_ptr<GameObject> collider) override;

    virtual void duringCollision(std::shared_ptr<GameObject> collider) override;

protected:
    std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> bulletSpawner;
    std::vector<std::shared_ptr<GameObject>> playerObjects;
    std::shared_ptr<GameObject> currentTarget;
    chag::float3 locationAtLastUpdate;
    float previousXSpeed = 0;
    float previousYSpeed = 0;

    void orientEnemyTowardsPlayer() const;
    static std::shared_ptr<GameObject> randomPlayer(std::vector<std::shared_ptr<GameObject>> playerObjects);
};


#endif //HALLSOFBUBBA_ENEMYCOMPONENT_H
