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

#include "comshader.h"

ComShader::ComShader()
{

}

ComShader::~ComShader()
{

}


bool ComShader::readSource(const char *sourcePath, std::string &comCode)
{
    // 0. Read file
    std::ifstream file(sourcePath,std::ifstream::in);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // 1. Retrieve the source code from filePath
    comCode = buffer.str();

return true; }


GLuint ComShader::createComSh(const char *comShaderCode, const GLint lengthCode)
{
    // Compile shaders
    GLint success; GLchar infoLog[512]; // Check
    GLuint comShader ;

    // Only Compute Shader (Optional)
    comShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(comShader, 1, &comShaderCode, &lengthCode);
    glCompileShader(comShader);
    // Print compile errors if any
    glGetShaderiv(comShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(comShader, 512, NULL, infoLog);
        std::cout << "ERROR::COMPUTE::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, comShader);

    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Should detach and delete shader after a successful link.
    glDetachShader(shaderProgram, comShader);
    glDeleteShader(comShader);

return shaderProgram ; }

