#pragma once
#include <Component.hpp>
#include <GameObject.h>
#include <vector>
#include <string>
namespace VEngine 
{
class Scene 
{
public:
	std::string sceneName = "default";
	Scene(const std::string inName = "default") { sceneName = inName; RegisterScene(this); };
	virtual ~Scene() {};
	std::vector<GameObject*> gameObjects{};
	void RegisterGO( GameObject* inGO) { gameObjects.push_back(inGO); };
	virtual void OnRender(float deltaTime) {};
	virtual void OnUpdate(float deltaTime) {};
	virtual void OnBeginPlay() {};

	inline static std::vector<Scene*> activeScenes;
	static std::vector<Scene*> GetActiveScenes() { return activeScenes; };
	static void RegisterScene(Scene* inScene) { activeScenes.push_back(inScene); };

};
}