#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
private:
	bool isDirty = false;
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 location = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

public:
	Transform(	const glm::vec3& inLocation = glm::vec3(0.0f),
				const glm::vec3& inRotation = glm::vec3(0.0f),
				const glm::vec3& inScale = glm::vec3(1.0f)){location = inLocation; rotation = inRotation; scale = inScale; isDirty = true; };	

	~Transform() = default;

	void SetLocation(float X, float Y, float Z) { location = glm::vec3(X, Y, Z); isDirty = true; };
	void SetLocation(const glm::vec3& inLocation) { location = inLocation; isDirty = true; };

	void SetRotation(float Pitch, float Yaw, float Roll) { rotation = glm::vec3(Pitch, Yaw, Roll); isDirty = true; };
	void SetRotation(const glm::vec3& inRotation) { rotation = inRotation; isDirty = true; };

	void SetScale(float X, float Y, float Z) { scale = glm::vec3(X, Y, Z); isDirty = true; };
	void SetScale(const glm::vec3& inScale) { scale = inScale; isDirty = true; };

	const glm::vec3& GetLocation() { return location; };
	const glm::vec3& GetRotation() { return rotation; };
	const glm::vec3& GetScale() { return scale; };

	const glm::mat4& GetTransformMatrix() 
	{
		if (isDirty)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, location);
			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, -1.0f));
			model = glm::scale(model, scale);
			isDirty = false;
		}
		return model;
	};

	//static stuff
	static glm::vec3 NormalizeVector(const glm::vec3& inVector) { return glm::normalize(inVector); };

	static const glm::vec3 VectorUp() { return glm::vec3(0.0f, 1.0f, 0.0f); };
	static const glm::vec3 VectorRight() { return glm::vec3(1.0f, 0.0f, 0.0f); };
	static const glm::vec3 VectorForward() { return glm::vec3(0.0f, 0.0f, 1.0f); };
};