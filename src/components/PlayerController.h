//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_PLAYEROBJECT_H
#define HALLSOFBUBBA_PLAYEROBJECT_H

#define PLAYER_STRENGTH 19.0f

#include <MoveComponent.h>
#include <ObjectIdentifiers.h>
#include <functional>
#include <ControlStatus.h>
#include "JumpingController.h"
#include <ControlsManager.h>

class PlayerController : public JumpingController {

public:
    PlayerController(std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBulletFunc,
                     std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBlastBulletFunc,
                     Camera *camera, std::shared_ptr<PointLight> light, ControlStatus::Activator activator);
    chag::float3 getPlaneIntersectionPoint();

    virtual void update(float dt) override;

    void beforeCollision(std::shared_ptr<GameObject> collider) override;

    void duringCollision(std::shared_ptr<GameObject> collider) override;

private:
    std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBulletFunc;
    std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBlastBulletFunc;
    Camera *camera;
    float timeSinceLastShotAttackLMB = 1.0f;
    float timeSinceLastShotAttackRMB = 1.0f;
    chag::float3 locationAtLastUpdate;
    std::shared_ptr<PointLight> light;
    std::pair<float, float> prevMousePosition = std::pair<float, float>(-1, -1);
    std::pair<float, float> virtualMousePosition = std::pair<float, float>(0, 0);
    ControlStatus::Activator activator;

    chag::float3 getMouseRayInWorldSpace(float mouseX, float mouseY, int width, int height) const;

    bool checkMouseMovedAndUpdatePrevPosition();

    void updateVirtualMousePositionBasedOnJoystick(int width, int height);

    float clip(float min, float toClip, float max);

    void movePlayer(float dt, std::shared_ptr<GameObject> &owner_ptr,
                                    ControlsManager *controlsManager);

    void handleShoots(float dt, std::shared_ptr<GameObject> &owner_ptr, ControlsManager *cm);

    void rotateTowardsMouse(std::shared_ptr<GameObject> &owner_ptr);
};


#endif //HALLSOFBUBBA_PLAYEROBJECT_H
