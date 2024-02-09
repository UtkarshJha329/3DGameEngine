#pragma once

#include "../SystemsManager.h"
#include "../EntityTabler.h"

#include "../Components/Transform.h"
#include "../Components/MeshData.h"
#include "../ShaderUtils.h"
#include "../Components/Camera.h"
#include "../Components/ParentChild.h"

//class DebugShader {
//
//public:
//    int shaderProgram;
//    glm::vec3 lineColor;
//
//public:
//    DebugShader(glm::vec3 color) {
//
//        lineColor = color;
//
//        const char* vertexShaderSource = "#version 330 core\n"
//            "layout (location = 0) in vec4 aPos;\n"
//            "uniform mat4 transform;\n"
//            "uniform mat4 cameraTransformMat;\n"
//            "uniform mat4 projection;\n"
//            "vec3 FragPos;\n"
//            "void main()\n"
//            "{\n"
//                "FragPos = vec3(transform * aPos);\n"
//                "gl_Position = projection * inverse(cameraTransformMat) * vec4(FragPos, 1.0);\n"
//        "}\n\0";
//
//        const char* fragmentShaderSource = "#version 330 core\n"
//            "layout (location = 0) out vec4 FragColor;\n"
//            "uniform vec3 color;\n"
//            "void main()\n"
//            "{\n"
//            "   FragColor = vec4(color, 1.0f);\n"
//            "}\n\0";
//
//        // vertex shader
//        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//        glCompileShader(vertexShader);
//        // check for shader compile errors
//        checkCompileErrors(vertexShader, "VERTEX");
//
//        // fragment shader
//        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//        glCompileShader(fragmentShader);
//        // check for shader compile errors
//        checkCompileErrors(fragmentShader, "FRAGMENT");
//
//        // link shaders
//        shaderProgram = glCreateProgram();
//        glAttachShader(shaderProgram, vertexShader);
//        glAttachShader(shaderProgram, fragmentShader);
//        glLinkProgram(shaderProgram);
//        // check for linking errors
//        checkCompileErrors(shaderProgram, "PROGRAM");
//
//        glDeleteShader(vertexShader);
//        glDeleteShader(fragmentShader);
//
//    }
//
//    void setColor(glm::vec3 color) {
//        lineColor = color;
//    }
//
//    int draw(unsigned int VAO) {
//        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(lineColor));
//
//        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//        return 1;
//    }
//
//    ~DebugShader() {
//        glDeleteProgram(shaderProgram);
//    }
//};

class DebugDrawSystem : public System {

public:

	DebugDrawSystem() {
		systemName = typeid(DebugDrawSystem).name();
	}

	void UnorderedInit() override {}
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

public:
    //DebugShader* debugShader;
};