#pragma once
#include <vector>
#include <Component.hpp>
#include <Transform.hpp>
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
		Transform transform;

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

		template <std::derived_from<Component> T> T* GetComponent() 
		{
			for(Component* comp : components)
			{
				if (typeid(*comp) == typeid(T))
					return comp;
			}
			return nullptr;
		};

		template <std::derived_from<Component> T> std::vector<T*> GetComponentsOfType() 
		{
			std::vector<T*> comps{};
			for (Component* comp : components) 
			{
				if (T* ptr = dynamic_cast<T*>(comp))
					comps.push_back(ptr);
			}
			return comps;
		};

#pragma region Convenience Helpers
		void SetParent(GameObject* newParent) { transform.SetParent(&newParent->transform); }
		void SetParent(Transform* newParent) { transform.SetParent(newParent); }
#pragma endregion

	};
}