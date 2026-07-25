#pragma once
#include <GameObject.h>
#include <Scene.hpp>
#include <Game.hpp>

VEngine::GameObject::GameObject(std::string inName)
{
	this->objectName = inName;
	Scene::activeScenes[0]->RegisterGO(this);
	owningScene = Scene::activeScenes[0];
	transform.SetOwningGameObject(this);
	transform.SetParent(&owningScene->sceneTransform);
}

void VEngine::GameObject::OnUpdate(double deltaTime)
{
	for (Component* comp : components) 
	{
		comp->OnUpdate(deltaTime);
	}
}