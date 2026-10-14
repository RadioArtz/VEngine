#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace VEngine 
{
	class GameObject;

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

		Transform* parent = nullptr;

		void MarkDirty() 
		{
			isDirty = true;
			for (Transform* trans : children)
				trans->MarkDirty();
		}

	public:
		GameObject* owningGameObject = nullptr;
		std::vector<Transform*> children{};

		Transform(	const glm::vec3& inLocation = glm::vec3(0.0f),
					const glm::vec3& inRotation = glm::vec3(0.0f),
					const glm::vec3& inScale = glm::vec3(1.0f))
		{location = inLocation; rotation = inRotation; scale = inScale; MarkDirty(); }

		~Transform() = default;

		void SetParent(Transform* inParent)
		{
			if (parent) 
				std::erase(parent->children, this);

			parent = inParent;

			if (parent)
				parent->children.push_back(this);

			MarkDirty();
		}

		void SetOwningGameObject(GameObject* go)
		{
			owningGameObject = go;
		}

		Transform* GetParent() { return parent; }
		//setters
	public:
		void SetRelativeLocation(float X, float Y, float Z) { location = glm::vec3(X, Y, Z); MarkDirty(); }
		void SetRelativeLocation(const glm::vec3& inLocation) { location = inLocation; MarkDirty(); }

		void SetRelativeRotation(float Pitch, float Yaw, float Roll) { rotation = glm::vec3(Pitch, Yaw, Roll); MarkDirty(); }
		void SetRelativeRotation(const glm::vec3& inRotation) { rotation = inRotation; MarkDirty(); }

		void SetRelativeScale(float X, float Y, float Z) { scale = glm::vec3(X, Y, Z); MarkDirty(); }
		void SetRelativeScale(const glm::vec3& inScale) { scale = inScale; MarkDirty(); }
	
		//modifiers
	public:
		void AddRelativeLocation(float X, float Y, float Z) { location += glm::vec3(X, Y, Z); MarkDirty(); }
		void AddRelativeLocation(const glm::vec3& inLocation) { location += inLocation; MarkDirty(); }

		void AddRelativeRotation(float Pitch, float Yaw, float Roll) { rotation += glm::vec3(Pitch, Yaw, Roll); MarkDirty(); }
		void AddRelativeRotation(const glm::vec3& inRotation) { rotation += inRotation; MarkDirty(); }

		void AddRelativeScale(float X, float Y, float Z) { scale += glm::vec3(X, Y, Z); MarkDirty(); }
		void AddRelativeScale(const glm::vec3& inScale) { scale += inScale; MarkDirty(); }

		//getters
	public:
		const bool GetDirty() const { return isDirty; }
		const glm::vec3& GetLocation() const { return location; }
		const glm::vec3& GetRotation() const { return rotation; }
		const glm::vec3& GetScale() const { return scale; }

		const glm::vec3 GetForwardVector()
		{
			return MatrixGetForwardVector(GetTransformMatrix());
		}

		const glm::vec3 GetRightVector() 
		{
			return MatrixGetRightVector(GetTransformMatrix());
		}

		const glm::vec3 GetUpVector() 
		{
			return MatrixGetUpVector(GetTransformMatrix());
		}

		const glm::mat4& GetTransformMatrix() 
		{
			if (isDirty)
			{
				//calculate own transformMatrix
				trans = glm::mat4(1.0f);
				trans = glm::translate(trans, location);
				trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				trans = glm::scale(trans, scale);
				//calculateWorldTransformMatrix
				if(parent)
					trans = parent->GetTransformMatrix() * trans;

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
	private:
		static glm::vec3 MatrixGetTranslation(const glm::mat4& m) { return glm::vec3(m[3]); }
		static glm::vec3 MatrixGetRightVector(const glm::mat4& m) { return glm::normalize(-glm::vec3(m[0])); }
		static glm::vec3 MatrixGetUpVector(const glm::mat4& m) { return glm::normalize(glm::vec3(m[1])); }
		static glm::vec3 MatrixGetForwardVector(const glm::mat4& m) { return glm::normalize(glm::vec3(m[2])); }
	};
}