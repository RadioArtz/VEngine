#pragma once
#include <glm/glm.hpp>
#include <vector>


namespace VEngine
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		unsigned int VAO, VBO, EBO;
		void PrepareMesh();

		Mesh(std::vector<Vertex> inVertices, std::vector<unsigned int>& inIndices)
		{
			vertices = inVertices;
			indices = inIndices;

			PrepareMesh();
		}

		void Draw();
	};
}

