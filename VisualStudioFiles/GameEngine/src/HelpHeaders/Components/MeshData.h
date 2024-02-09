#pragma once

#include "../Component.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string.h>

#include "../TextureHandeling/Texture_H.h"
#include "../ShaderUtils.h"

struct VAA_Data {

	GLuint index;
	GLint sizeOfVertex;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void* offset;
};


class DebugShader {

public:
    int shaderProgram;
    glm::vec3 lineColor;

    unsigned int VAO, VBO, EBO, VAA;

public:
    DebugShader(glm::vec3 color, std::vector<float> &bbCoords, unsigned int* indicies) {

        lineColor = color;

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 transform;\n"
            "uniform mat4 cameraTransformMat;\n"
            "uniform mat4 projection;\n"
            "vec3 FragPos;\n"
            "void main()\n"
            "{\n"
            "FragPos = vec3(transform * vec4(aPos, 1.0f));\n"
            "gl_Position = projection * inverse(cameraTransformMat) * vec4(FragPos, 1.0);\n"
            "}\n\0";

        const char* fragmentShaderSource = "#version 330 core\n"
            "layout (location = 0) out vec4 FragColor;\n"
			"layout (location = 1) out vec4 ColourBuffer;\n"
			"layout(location = 2) out int thisEntityID;\n"
            "uniform vec3 color;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(color, 1.0f);\n"
			"	ColourBuffer = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n"
			"	thisEntityID = -1;\n"
            "}\n\0";

        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        checkCompileErrors(vertexShader, "VERTEX");

        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        checkCompileErrors(fragmentShader, "FRAGMENT");

        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        checkCompileErrors(shaderProgram, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, bbCoords.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

		VAA = 0;
		glVertexAttribPointer(VAA, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(VAA);

		glBindVertexArray(VAA);
    }

    void setColor(glm::vec3 color) {
        lineColor = color;
    }

    int draw(unsigned int VAO) {
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(lineColor));

        glBindVertexArray(VAO);

		glUseProgram(shaderProgram);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        return 1;
    }

    ~DebugShader() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

        glDeleteProgram(shaderProgram);
    }
};

struct AABB {
	glm::vec3 min;
	glm::vec3 max;

	std::vector<float> bbcoords;

	unsigned int indices[36] = {
		0, 2, 3, 0, 3, 1,
		2, 6, 7, 2, 7, 3,
		6, 4, 5, 6, 5, 7,
		4, 0, 1, 4, 1, 5,
		0, 4, 6, 0, 6, 2,
		1, 5, 7, 1, 7, 3,
	};

	~AABB() {
	}

    DebugShader* debugShader;
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
