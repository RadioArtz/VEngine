#pragma once
#include <Rendering/Mesh.h>
#include <Rendering/Material.h>

namespace VEngine
{
	class StaticMeshComponent
	{
	private:
		Mesh* usedMesh;
		Material* usedMaterial;

	public:
		StaticMeshComponent(Mesh* meshPtr) { usedMesh = meshPtr; }

	};
}