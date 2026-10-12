#include "Rendering/StaticMeshComponent.h"
#include <GameObject.h>

void VEngine::StaticMeshComponent::OnRender(double deltaTime)
{
	if (!mesh || !material)
		return;

	material->SetMaterialParam("_model", GetOwner()->transform.GetTransformMatrix());
	material->RenderMaterial();
	mesh->Draw();
}
