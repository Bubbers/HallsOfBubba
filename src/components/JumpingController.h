#ifndef HALLSOFBUBBA_JUMPINGCONTROLLER_H
#define HALLSOFBUBBA_JUMPINGCONTROLLER_H


#include <MoveComponent.h>

class JumpingController : public MoveComponent {

public:
    JumpingController(float strength);
    virtual void update(float dt);
    bool isJumping();

protected:
    virtual void jump();
    virtual void onLand(){};

private:
    bool jumping = false;
    float strength;

};


#endif //HALLSOFBUBBA_JUMPINGCONTROLLER_H
