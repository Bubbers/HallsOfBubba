#include "SpawnGameObject.h"

SpawnGameObject::SpawnGameObject(std::function<std::shared_ptr<GameObject>()> spawnFunction,
                                 std::shared_ptr<Scene> scene) :
                                 m_spawnFunction(spawnFunction),
                                 m_scene(scene)
{
}


void SpawnGameObject::update(float dt)
{
}

void SpawnGameObject::execute()
{
    std::shared_ptr<GameObject> gob = m_spawnFunction();

    m_scene->addShadowCaster(gob);
}
