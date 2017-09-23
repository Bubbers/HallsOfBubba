//
// Created by simon on 2017-05-01.
//

#ifndef HALLSOFBUBBA_BOSSCONTROLLER_H
#define HALLSOFBUBBA_BOSSCONTROLLER_H


#include <Texture.h>
#include "EnemyComponent.h"

class BossController : public EnemyComponent {

public:

    BossController(std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> bulletSpawner,
                   std::vector<std::shared_ptr<GameObject>> playerObjects);

    virtual void update(float dt) override;

    virtual void beforeCollision(std::shared_ptr<GameObject> collider) override;
    virtual void duringCollision(std::shared_ptr<GameObject> collider) override;

private:

    bool charging = false;
    chag::float3 chargeSpeed = chag::make_vector(0.0f, 0.0f, 0.0f);
    bool hitPlayer = false;

    float stunnedFor = 0.0f;

};


#endif //HALLSOFBUBBA_BOSSCONTROLLER_H
