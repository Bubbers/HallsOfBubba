
#pragma once
#include <IComponent.h>
#include <GameObject.h>
#include <Scene.h>
#include "Executable.h"

class SpawnGameObject : public IComponent, public Executable
{
public:
   SpawnGameObject(std::function<std::shared_ptr<GameObject>()> spawnFunction,
                   std::shared_ptr<Scene> scene);

   void update(float dt);    
   void execute();

private:
   std::shared_ptr<Scene> m_scene;
   std::function<std::shared_ptr<GameObject>()> m_spawnFunction;
};

