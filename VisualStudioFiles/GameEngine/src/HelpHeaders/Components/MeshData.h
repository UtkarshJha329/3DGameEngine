#pragma once

#include "../Component.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string.h>

#include "../TextureHandeling/Texture_H.h"

struct VAA_Data {

	GLuint index;
	GLint sizeOfVertex;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void* offset;
};

struct AABB {
	glm::vec3 min;
	glm::vec3 max;
};

class CommonMeshData {

public:

	std::vector<Texture*> loadedTextures;

	inline static CommonMeshData* Instance() {
		return inst;
	}

	void SetInstance() {
		inst = this;
	}

private:

	inline static CommonMeshData* inst;
};

class MeshData : public Component<MeshData>{

public:

	static VAA_Data stdVAAData[];
	static unsigned int stdNumVAAs;
	static int stride;

	unsigned int numVertices = 0;

	unsigned int numTextures = 0;

	unsigned int VAO, VBO, EBO;

	AABB aabb;

	int verticesSize = 0;

	std::string meshDataDir = "temp";
	std::string meshPath = "temp";
	std::string meshName = "temp";

	//std::vector<MeshData*>meshes;
	std::vector<unsigned int>meshEntities;
	std::vector<unsigned int> textureIndex;
	std::vector<unsigned int> indecies;
	std::vector<float> vertices;

	static unsigned int mainCameraEntity;

	virtual void SerializeComponent() override;
	virtual void DeSerializeComponent() override;
	virtual void UIfyComponent() override;

};

/*	OBSOLETE CODE

	MeshData MakeMesh(std::vector<float> vertices,
						std::vector<unsigned int> &indecies,
						std::vector<unsigned int> &textureIndex,
						VAO_Data* vaoDatas, int numVaos, GLsizeiptr bufferSize);


*/