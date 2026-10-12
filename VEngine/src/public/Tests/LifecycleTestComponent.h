#pragma once
#include <Component.hpp>
namespace VEngine 
{
	class LifecycleTestComponent : public Component
	{
	public:
		void OnAwake() override;
		void OnBeginPlay() override;
		void OnUpdate(double deltaTime) override;
	};
}