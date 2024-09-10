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

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "shaderprogram.h"

class Camera : public ShaderProgram
{
public:
    explicit Camera();

    void initCamera3D(GLfloat r, GLfloat t, GLfloat p, GLfloat width, GLfloat height);
    void moveCamera3D(GLfloat dx, GLfloat dy);
    void checkCamera3D();

    //GLboolean Move[4] ;
    //GLfloat widthGL ;
    //GLfloat coordX, coordY ; 
 
    glm::vec3 cameraPos3D ;
    GLboolean isMoving ;
    GLboolean Move[2] ;
    GLfloat ratio, zoom, R  ;

private:
    GLfloat phi,theta ; // Spherical 3D in Radian
};

#endif // CAMERA_H
