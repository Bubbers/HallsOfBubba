#ifndef HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H
#define HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H


#include <IComponent.h>
#include <Scene.h>
#include <AudioManager.h>
#include <functional>

class ChangeRoomOnCollision: public IComponent {
public:
    ChangeRoomOnCollision(std::shared_ptr<Scene> scene, std::function<void(void)> onCollision);
    void beforeCollision(std::shared_ptr<GameObject> collider);
    void update(float dt);

private:
    std::shared_ptr<Scene> scene;

    std::shared_ptr<sf::Sound> m_sound;

    std::function<void(void)> onCollision;
};


#endif //HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H
