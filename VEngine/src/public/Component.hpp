#pragma once


namespace VEngine 
{
	class GameObject;

	class Component 
	{
	protected:
		GameObject* owner = nullptr;

	public:
		Component() {};
		virtual ~Component() {};

		virtual void OnAwake() {};
		virtual void OnBeginPlay() {};
		virtual void OnUpdate(double deltaTime) {};
		virtual void OnRender(double deltaTime) {}

		friend class GameObject;

		GameObject* GetOwner() const { return owner; }
	};
}