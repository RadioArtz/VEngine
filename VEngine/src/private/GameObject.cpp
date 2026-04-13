#pragma once
#include <GameObject.h>
#include <Scene.hpp>

VEngine::GameObject::GameObject(std::string inName)
{
	this->objectName = inName;
	Scene::activeScenes[0]->RegisterGO(this);
};