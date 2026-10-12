#pragma once
#include <Rendering/Mesh.h>
#include <Rendering/Material.h>
#include <Component.hpp>

namespace VEngine
{
	class StaticMeshComponent : public Component
	{
	private:
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;

	public:
		StaticMeshComponent(std::shared_ptr<Mesh> inMesh, std::shared_ptr<Material> inMaterial) : mesh(std::move(inMesh)), material(std::move(inMaterial)) {}
		void OnRender(double deltaTime) override;
	};
}