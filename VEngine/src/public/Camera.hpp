#pragma once
#include <glm/glm.hpp>
#include <Transform.hpp>

class Camera
{
public:
	Camera(Transform* inTransformPtr) { transPtr = inTransformPtr; };
	~Camera() {};

private:
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(0.0f);
	Transform* transPtr = nullptr;
	float verticalFOV = 45.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;
	float aspectRatio = 16.0f / 9.0f;
	bool isViewDirty = true;
	bool isPerspectiveDirty = true;

public:
	const glm::mat4& GetViewMatrix()
	{
		isViewDirty = transPtr->GetDirty();
		if (isViewDirty) 
		{
			view = glm::lookAt(transPtr->GetLocation(), transPtr->GetLocation() + transPtr->GetForwardVector(), Transform::VectorUp());
			isViewDirty = false;
		}
		return view;
	}

	const glm::mat4& GetProjectionMatrix() 
	{
		if (isPerspectiveDirty) 
		{
			projection = glm::perspective(glm::radians(verticalFOV), aspectRatio, nearClip, farClip);
			isPerspectiveDirty = false;
		}
		return projection;
	}
	
	void SetAspectRatio(float inAspect = 16.0f / 9.0f) { aspectRatio = inAspect; isPerspectiveDirty = true; }
	float GetAspectRatio() const { return aspectRatio; }

	void SetFieldOfView(float inFOV = 45.0f) { verticalFOV = inFOV; isPerspectiveDirty = true; }
	float GetFieldOfView() const { return verticalFOV; }

	void SetNearClipPlane(float inNearClip = 0.01f) { nearClip = inNearClip; isPerspectiveDirty = true; }
	float GetNearClipPlane() const { return nearClip; }

	void SetFarClipPlane(float inFarclip = 1000.0f) { farClip = inFarclip; isPerspectiveDirty = true; }
	float GetFarClipPlane() const { return farClip; }
};