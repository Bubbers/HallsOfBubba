
#pragma once

#include <Scene.h>
#include <Collider.h>
#include <Camera.h>
#include <Renderer.h>

class Room {

public:
    Room();
    ~Room();

    void load();
    void update(float dt);
    void display(Renderer &renderer,
                 std::shared_ptr<Camera> camera,
                 float timeSinceStart,
                 float timeSinceLastCall);

private:
    void createLight();

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Collider> m_collider;
};
