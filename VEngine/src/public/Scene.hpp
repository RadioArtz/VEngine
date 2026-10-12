#pragma once
#include <Component.hpp>
#include <vector>
#include <string>
#include <Transform.hpp>

namespace VEngine 
{
	class GameObject;
	class Scene 
	{
	private:
		bool hasBegunPlay;
	public:
		std::string sceneName = "default";
		Transform sceneTransform{};
		Scene(const std::string inName = "default") { sceneName = inName; RegisterScene(this); };
		virtual ~Scene() {};
		std::vector<GameObject*> gameObjects{};
		void RegisterGO( GameObject* inGO) 
		{ 
			gameObjects.push_back(inGO); 
			if (hasBegunPlay)
				inGO->OnBeginPlay();
		};
		virtual void OnRender(double deltaTime);
		virtual void OnUpdate(double deltaTime);
		virtual void OnBeginPlay();

		inline static std::vector<Scene*> activeScenes;
		static std::vector<Scene*> GetActiveScenes() { return activeScenes; };
		static void RegisterScene(Scene* inScene) { activeScenes.push_back(inScene); };

	};
}