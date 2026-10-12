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

void VEngine::GameObject::OnBeginPlay() 
{
	if (hasBegunPlay)
		return;
	hasBegunPlay = true;
	for (Component* comp : components)
		comp->OnBeginPlay();
}

void VEngine::GameObject::OnUpdate(double deltaTime)
{
	if (!allowTickBeforeBeginPlay && !hasBegunPlay)
		return;

	for (Component* comp : components) 
		comp->OnUpdate(deltaTime);
}

void VEngine::GameObject::OnRender(double deltaTime)
{
	for (Component* comp : components)
		comp->OnRender(deltaTime);
}