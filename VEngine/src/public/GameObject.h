#pragma once
#include <vector>
#include <Component.hpp>
#include <TransformComponent.hpp>
#include <string>
#include <utility>

namespace VEngine 
{
	class GameObject 
	{
	public:
		std::string objectName = "GameObject";
		std::vector<Component*> components{};
		GameObject(std::string inName = "GameObject");
		~GameObject() {};
		TransformComponent* transform = nullptr;

	protected:
		class Scene* owningScene = nullptr;

	public: 
		void OnAwake() {};
		void OnBeginPlay() {};
		void OnUpdate(double deltaTime);
		void OnConstruct() {};

		template <std::derived_from<Component> T, typename... Args> T* AddComponent(Args&&... args)
		{
			T* comp = new T(std::forward<Args>(args)...);
			components.push_back(comp);
			return comp;
		};
	};
}