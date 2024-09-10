/*
#
# Hard-sphere collision simulation
# = collision rate Experiment
#
# version 1.0.0
#
# Copyright (c) 2023
# Somrath Kanoksirirath <somrathk@gmail.com>
*/

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

protected:
    static glm::mat4 Trans ;
    static GLfloat *ptrTrans ;
    
    GLuint buildShaderVF(const char *vertexPath, const char *fragmentPath);
    GLuint buildShaderVGF(const char *vertexPath, const char *fragmentPath, const char *geometryPath);
    GLuint buildComSh(const char *comPath);

private:
    std::string readShader(const char *filePath);

};

#endif // SHADERPROGRAM_H
