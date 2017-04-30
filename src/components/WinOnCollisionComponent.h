#ifndef HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H
#define HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H


#include <IComponent.h>
#include <Scene.h>
#include <AudioManager.h>

class WinOnCollisionComponent: public IComponent {
public:
    WinOnCollisionComponent(std::shared_ptr<Scene> scene, std::function<void(void)> onWin);
    void beforeCollision(std::shared_ptr<GameObject> collider);
    void update(float dt);

private:
    std::shared_ptr<Scene> scene;

    std::shared_ptr<sf::Sound> m_sound;

    std::function<void(void)> onWin;
};


#endif //HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H
