#pragma once
#include <vector>
#include <Component.hpp>
#include <TransformComponent.hpp>
#include <string>

namespace VEngine 
{
	class GameObject 
	{
	public:
		std::string objectName = "GameObject";
		std::vector<Component*> components{};
		GameObject(std::string inName = "GameObject");
		~GameObject() {};
	protected:
		TransformComponent* transform = nullptr;

	public: 
		virtual void OnAwake() {};
		virtual void OnBeginPlay() {};
		virtual void OnUpdate() {};
		virtual void OnConstruct() {};
	};
}