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


#include "box.h"


Box::Box()
{

}


bool Box::init(float lx, float ly, float lz)
{
    Shader = buildShaderVF("./Render/Box.vecSh","./Render/Box.fragSh");
    Lx = (GLfloat) lx ;
    Ly = (GLfloat) ly ;
    Lz = (GLfloat) lz ;

    GLfloat vertices[] = {
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,

         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f
    };

    // Buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
      // Data transfer
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      // Pointer of Position
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
      glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

return true; }


void Box::draw()
{
    glUseProgram(Shader);
    glUniformMatrix4fv(glGetUniformLocation(Shader, "Trans"), 1, GL_FALSE, ptrTrans);
    glUniform3f(glGetUniformLocation(Shader,"Size"), Lx, Ly, Lz);
      glBindVertexArray(VAO);
        // Faces
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
          glUniform4f(glGetUniformLocation(Shader,"Color"), 0.9f, 0.9f, 0.9f, 0.3f);
          glDrawArrays(GL_TRIANGLES, 0, 36);
        glDisable(GL_CULL_FACE);
        // Line borders
        glDisable(GL_DEPTH_TEST);
          glUniform4f(glGetUniformLocation(Shader,"Color"), 0.1f, 0.1f, 0.1f, 0.5f);
          glDrawArrays(GL_LINE_STRIP, 0, 36);
        glEnable(GL_DEPTH_TEST);
      glBindVertexArray(0);
    glUseProgram(0);
}


Box::~Box()
{
    glDeleteProgram(Shader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


