#pragma once
namespace VEngine 
{
	class Component 
	{
	public:
		Component() {};
		virtual ~Component() {};

		virtual void OnAwake() {};
		virtual void OnBeginPlay() {};
		virtual void OnUpdate(double deltaTime) {};
		virtual void OnConstruct() {};
	};
}