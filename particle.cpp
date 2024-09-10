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

#include "particle.h"

#define PI 3.1415926f
#define NUM_LAT 16
#define NUM_LON 32


Particle::Particle()
{

}


bool Particle::init(float vector_size)
{
    arrow_size = (GLfloat) vector_size ;

    SphereShader =  buildShaderVF("./Render/Sphere.vecSh","./Render/Sphere.fragSh");
    ArrowShader  = buildShaderVGF("./Render/Arrow.vecSh","./Render/Arrow.geoSh","./Render/Sphere.fragSh");

    std::vector<GLfloat> vertices ;
    for(unsigned int j=0 ; j<NUM_LAT ; ++j)
    {
        float theta_up   = PI*((float) j/NUM_LAT);
        float theta_down = PI*((float) (j+1)/NUM_LAT);
        for(unsigned int i=0 ; i<=NUM_LON ; ++i)
        {
            float phi = 2*PI*((float) i/NUM_LON);

            vertices.push_back((GLfloat) cos(phi)*sin(theta_up));
            vertices.push_back((GLfloat) sin(phi)*sin(theta_up));
            vertices.push_back((GLfloat) cos(theta_up));

            vertices.push_back((GLfloat) cos(phi)*sin(theta_down));
            vertices.push_back((GLfloat) sin(phi)*sin(theta_down));
            vertices.push_back((GLfloat) cos(theta_down));
        }
    }

    // Buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &sphereVBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
      // Data transfer
      glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
      // Pointer of Sphere
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
      glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

return true; }


void Particle::set(GLuint inVBO)
{
    this->VBO = inVBO ;
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
      // Pointer of Position + Radius
      glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
      glVertexAttribDivisor(1, 1); // the location(1) will update when finish drawing 1 instance.
      glEnableVertexAttribArray(1);
      // Pointer of Velocity + Mass
      glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(4*sizeof(GLfloat)));
      glVertexAttribDivisor(2, 1); // the location(2) will update when finish drawing 1 instance.
      glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}


void Particle::draw(unsigned int nParticle)
{
    glBindVertexArray(VAO);
      glUseProgram(SphereShader);
      glUniformMatrix4fv(glGetUniformLocation(SphereShader, "Trans"), 1, GL_FALSE, ptrTrans);
      glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 2*NUM_LAT*(NUM_LON+1), (GLuint)nParticle);
    /*
      glUseProgram(ArrowShader);
      glUniformMatrix4fv(glGetUniformLocation(ArrowShader, "Trans"), 1, GL_FALSE, ptrTrans);
      glUniform1f(glGetUniformLocation(ArrowShader,"Size"), arrow_size);
        glDrawArraysInstanced(GL_POINTS, 0, 1, (GLuint)nParticle);
    */
      glUseProgram(0);
    glBindVertexArray(0);
}


Particle::~Particle()
{
    glDeleteProgram(SphereShader);
    glDeleteProgram(ArrowShader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &sphereVBO);
}


