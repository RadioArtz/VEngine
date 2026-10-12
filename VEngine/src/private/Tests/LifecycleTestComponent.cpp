#include <Tests/LifecycleTestComponent.h>
#include <iostream>

void VEngine::LifecycleTestComponent::OnAwake()
{
	//std::cout << "OnAwake" << std::endl;
}

void VEngine::LifecycleTestComponent::OnBeginPlay() 
{
	//std::cout << "OnBeginPlay" << std::endl;
}

void VEngine::LifecycleTestComponent::OnUpdate(double deltaTime) 
{
	//std::cout << "OnUpdate" << std::endl;
}