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

#include "shaderprogram.h"

ShaderProgram::ShaderProgram()
{

}

ShaderProgram::~ShaderProgram()
{

}

glm::mat4 ShaderProgram::Trans = glm::mat4() ;
GLfloat* ShaderProgram::ptrTrans = glm::value_ptr(Trans);

GLuint ShaderProgram::buildShaderVF(const char *vertexPath, const char *fragmentPath)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode = readShader(vertexPath);
    std::string fragmentCode = readShader(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    GLint success; GLchar infoLog[512]; // Check
    GLuint verShader, fragShader ;

    // Vertex Shader
    verShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(verShader, 1, &vShaderCode, NULL);
    glCompileShader(verShader);
    // Print compile errors if any
    glGetShaderiv(verShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(verShader, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fShaderCode, NULL);
    glCompileShader(fragShader);
    // Print compile errors if any
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 3.Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, verShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Should detach and delete shader after a successful link.
    glDetachShader(shaderProgram, verShader);
    glDetachShader(shaderProgram, fragShader);
    glDeleteShader(verShader);
    glDeleteShader(fragShader);

return shaderProgram ; }

GLuint ShaderProgram::buildShaderVGF(const char *vertexPath, const char *geometryPath, const char *fragmentPath)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode = readShader(vertexPath);
    std::string fragmentCode = readShader(fragmentPath);
    std::string geometryCode = readShader(geometryPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();

    // 2. Compile shaders
    GLint success; GLchar infoLog[512]; // Check
    GLuint verShader, fragShader, geoShader ;

    // Vertex Shader
    verShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(verShader, 1, &vShaderCode, NULL);
    glCompileShader(verShader);
    // Print compile errors if any
    glGetShaderiv(verShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(verShader, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fShaderCode, NULL);
    glCompileShader(fragShader);
    // Print compile errors if any
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Geometry Shader
    geoShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geoShader, 1, &gShaderCode, NULL);
    glCompileShader(geoShader);
    // Print compile errors if any
    glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geoShader, 512, NULL, infoLog);
        std::cout << "ERROR::GEOMETRY::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 3.Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, verShader);
    glAttachShader(shaderProgram, fragShader);
    glAttachShader(shaderProgram, geoShader);

    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Should detach and delete shader after a successful link.
    glDetachShader(shaderProgram, verShader);
    glDetachShader(shaderProgram, fragShader);
    glDetachShader(shaderProgram, geoShader);
    glDeleteShader(verShader);
    glDeleteShader(fragShader);
    glDeleteShader(geoShader);

return shaderProgram ; }

GLuint ShaderProgram::buildComSh(const char *comPath)
{
    // 1. Retrieve the source code from filePath
    std::string comCode = readShader(comPath);
    const char *comShaderCode = comCode.c_str();

    // 2. Compile shaders
    GLint success; GLchar infoLog[512]; // Check
    GLuint comShader ;

    // Only Compute Shader (Optional)
    comShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(comShader, 1, &comShaderCode, NULL);
    glCompileShader(comShader);
    // Print compile errors if any
    glGetShaderiv(comShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(comShader, 512, NULL, infoLog);
        std::cout << "ERROR::COMPUTE::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 3.Create shader program
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


std::string ShaderProgram::readShader(const char *filePath)
{
    std::ifstream inFile ;
    inFile.open(filePath, std::ifstream::in);

    std::stringstream stream ;
    stream << inFile.rdbuf() ;
    std::string str = stream.str();

    inFile.close();

    return str ;
}
