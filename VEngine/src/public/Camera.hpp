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
	Transform* transPtr; // FIXME: If you go for leaving default values for everything, make sure to also initialize this! You can use {}, { nullptr } or = nullptr, whatever you think fits most. I personally enjoy {} initializers but = nullptr would fit best here probably.
	float verticalFOV = 45.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;
	float aspectRatio = 16.0f / 9.0f;
	bool isViewDirty = true;
	bool isPerspectiveDirty = true;

public:
	// FIXME: Would be hard to use in certain situations because those methods are not const.
	//			Maybe make view, isViewDirty mutable and this method const or a const version of this function like this: `const glm::mat4& GetViewMatrix() const { return const_cast<Camera*>(this)->GetViewMatrix(); }`
	const glm::mat4& GetViewMatrix()
	{
		isViewDirty = transPtr->GetDirty();
		if (isViewDirty) 
		{
			view = glm::lookAt(transPtr->GetLocation(), transPtr->GetLocation() + transPtr->GetForwardVector(), Transform::VectorUp());
			isViewDirty = false;
		}
		return view;
	};

	const glm::mat4& GetProjectionMatrix() 
	{
		if (isPerspectiveDirty) 
		{
			projection = glm::perspective(glm::radians(verticalFOV), aspectRatio, nearClip, farClip);
			isPerspectiveDirty = false;
		}
		return projection;
	}
	
	// FIXME: Really want to make those all optional params?
	// FIXME: Makes more sense to return those values here by value e.g. float GetAspectRatio() const
	//			(Return value is smaller than the size of a reference (8 bytes on 64-Bit systems)
	void SetAspectRatio(float inAspect = 16.0f / 9.0f) { aspectRatio = inAspect; isPerspectiveDirty = true; }; 
	const float& GetAspectRatio() const { return aspectRatio; };

	void SetFieldOfView(float inFOV = 45.0f) { verticalFOV = inFOV; isPerspectiveDirty = true; };
	const float& GetFieldOfView() const { return verticalFOV; };

	void SetNearClipPlane(float inNearClip = 0.01f) { nearClip = inNearClip; isPerspectiveDirty = true; };
	const float& GetNearClipPlane() const { return nearClip; };

	void SetFarClipPlane(float inFarclip = 1000.0f) { farClip = inFarclip; isPerspectiveDirty = true; };
	const float& GetFarClipPlane() const { return farClip; };
};