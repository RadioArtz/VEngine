#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace VEngine 
{
	class Transform
	{
	private:
		bool isDirty = false;
		glm::mat4 trans = glm::mat4(1.0f);

		glm::vec3 location = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		glm::vec3 forward = glm::vec3(0.0f);
		glm::vec3 right = glm::vec3(0.0f);
		glm::vec3 up = glm::vec3(0.0f);

	public:
		Transform(	const glm::vec3& inLocation = glm::vec3(0.0f),
					const glm::vec3& inRotation = glm::vec3(0.0f),
					const glm::vec3& inScale = glm::vec3(1.0f))
		{location = inLocation; rotation = inRotation; scale = inScale; isDirty = true; }

		~Transform() = default;

		//setters
	public:
		void SetLocation(float X, float Y, float Z) { location = glm::vec3(X, Y, Z); isDirty = true; }
		void SetLocation(const glm::vec3& inLocation) { location = inLocation; isDirty = true; }

		void SetRotation(float Pitch, float Yaw, float Roll) { rotation = glm::vec3(Pitch, Yaw, Roll); isDirty = true; }
		void SetRotation(const glm::vec3& inRotation) { rotation = inRotation; isDirty = true; }

		void SetScale(float X, float Y, float Z) { scale = glm::vec3(X, Y, Z); isDirty = true; }
		void SetScale(const glm::vec3& inScale) { scale = inScale; isDirty = true; }
	
		//modifiers
	public:
		void AddLocation(float X, float Y, float Z) { location += glm::vec3(X, Y, Z); isDirty = true; }
		void AddLocation(const glm::vec3& inLocation) { location += inLocation; isDirty = true; }

		void AddRotation(float Pitch, float Yaw, float Roll) { rotation += glm::vec3(Pitch, Yaw, Roll); isDirty = true; }
		void AddRotation(const glm::vec3& inRotation) { rotation += inRotation; isDirty = true; }

		void AddScale(float X, float Y, float Z) { scale += glm::vec3(X, Y, Z); isDirty = true; }
		void AddScale(const glm::vec3& inScale) { scale += inScale; isDirty = true; }

		//getters
	public:
		const bool GetDirty() const { return isDirty; }
		const glm::vec3& GetLocation() const { return location; }
		const glm::vec3& GetRotation() const { return rotation; }
		const glm::vec3& GetScale() const { return scale; }

		const glm::vec3& GetForwardVector()
		{
			forward = NormalizeVector(glm::vec3(
				cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x)),
				sin(glm::radians(rotation.x)),
				sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x))));
			return forward;
		}

		const glm::vec3& GetRightVector() 
		{
			right = NormalizeVector(glm::cross(VectorUp(), GetForwardVector()));
			return right;
		}

		const glm::vec3& GetUpVector() 
		{
			up = NormalizeVector(glm::cross(GetForwardVector(), GetRightVector()));
			return up;
		}

		const glm::mat4& GetTransformMatrix() 
		{
			if (isDirty)
			{
				trans = glm::mat4(1.0f);
				trans = glm::translate(trans, location);
				trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				trans = glm::scale(trans, scale);
				isDirty = false;
			}
			return trans;
		}

		//static utils
	public:
		static glm::vec3 NormalizeVector(const glm::vec3& inVector) { return glm::normalize(inVector); }
		static glm::vec3 LookAtVector(const glm::vec3& Origin, const glm::vec3& Target) { return NormalizeVector(Target - Origin); }
		static float GetDistance(const glm::vec3& A, const glm::vec3& B) { return glm::distance(A, B); }

		static const glm::vec3 VectorUp() { return glm::vec3(0.0f, 1.0f, 0.0f); }
		static const glm::vec3 VectorRight() { return glm::vec3(1.0f, 0.0f, 0.0f); }
		static const glm::vec3 VectorForward() { return glm::vec3(0.0f, 0.0f, 1.0f); }
	};
}