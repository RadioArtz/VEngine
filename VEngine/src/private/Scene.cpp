#include "Scene.hpp"
#include <GameObject.h>

void VEngine::Scene::OnUpdate(float deltaTime)
{
	for (GameObject* go : gameObjects) 
	{
		go->OnUpdate(deltaTime);
	}
}