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


#ifndef BOX_H
#define BOX_H

#include <glad/glad.h>

#include <vector>

#include "shaderprogram.h"

class Box : public ShaderProgram
{

public:
    explicit Box();
    ~Box();

    bool init(float lx, float ly, float lz);
    void draw();
    void clear();

private:
    GLfloat Lx, Ly, Lz ;
    GLuint Shader ;
    GLuint VAO, VBO ;
};


#endif // BOX_H
