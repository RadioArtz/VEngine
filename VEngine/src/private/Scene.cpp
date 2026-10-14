#include "Scene.hpp"
#include <GameObject.h>

void VEngine::Scene::RegisterGO(GameObject* inGO)
{
	gameObjects.push_back(inGO);
	if (hasBegunPlay)
		inGO->OnBeginPlay();
}

void VEngine::Scene::OnRender(double deltaTime)
{
	for (GameObject* go : gameObjects)
		go->OnRender(deltaTime);
}

void VEngine::Scene::OnUpdate(double deltaTime)
{
	for (GameObject* go : gameObjects) 
	{
		go->OnUpdate(deltaTime);
	}
}

void VEngine::Scene::OnBeginPlay() 
{
	hasBegunPlay = true;

	for (GameObject* go : gameObjects)
		go->OnBeginPlay();
}