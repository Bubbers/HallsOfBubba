#ifndef HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H
#define HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H


#include <IComponent.h>

class WinOnCollisionComponent: public IComponent {
public:
    WinOnCollisionComponent();
    void beforeCollision(std::shared_ptr<GameObject> collider);
    void update(float dt);
};


#endif //HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H
