#pragma once
#include <glm/glm.hpp>

namespace VEngine
{
	struct LightColor
	{
		glm::vec3 Color = glm::vec3(1.0f);
		float Intensity = 1.0f;
	};

	class Light
	{
	public:
		LightColor diffuseColor;
		LightColor specularColor;

		//bool castShadows;
		//float shadowFadeDistance
		//float maxDrawDistace;
		//float drawDistanceFade;
	};

	class PointLight : public Light 
	{
	public:
		float specularHardness = 32;
	};
}