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

#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>

#include <vector>
#include <cmath>

#include "shaderprogram.h"

class Particle : public ShaderProgram
{

public:
    explicit Particle();
    ~Particle();

    bool init(float vector_size);
    void set(GLuint inVBO);
    void draw(unsigned int nParticle);
    
private:
    GLfloat arrow_size ;

    GLuint SphereShader, ArrowShader ;
    GLuint VAO, VBO, sphereVBO ;
};


#endif // PARTICLE_H
