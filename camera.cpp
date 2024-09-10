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

#include "camera.h"

#define Deg1  1.7453292E-2
#define Deg90 1.5707963f

Camera::Camera()
{
    //coordX = coordY = 10.0f ;
    zoom = 1.0f ;
    Move[0] = Move[1] = GL_FALSE ;
    isMoving = GL_FALSE ;
}

void Camera::initCamera3D(GLfloat r, GLfloat t, GLfloat p, GLfloat width, GLfloat height)
{
    cameraPos3D = glm::vec3(0.0f, 0.0f, 0.0f);
    R = r ;
    theta = t*Deg1 ;
    phi = p*Deg1 ;

    this->ratio = height/width ;
    this->checkCamera3D();
}

/***************************** Move Camera ************************/

void Camera::moveCamera3D(GLfloat dx, GLfloat dy)
{
      phi   -= 5.0f*dx ;
      theta -= 5.0f*dy ;
}

void Camera::checkCamera3D()
{
    // Update camera position in 2D (x,y)
    /*
    if( -coordX < cameraPos3D.x && cameraPos3D.x <coordX ){
        if( Move[0] ){ cameraPos3D.x += 20/(zoom*widthGL) ; }
        if( Move[1] ){ cameraPos3D.x -= 20/(zoom*widthGL) ; }
    }
    if( -coordY < cameraPos3D.y && cameraPos3D.y <coordY ){
        if( Move[2] ){ cameraPos3D.y -= 20/(zoom*widthGL) ; }
        if( Move[3] ){ cameraPos3D.y += 20/(zoom*widthGL) ; }
    }
    */

    // Move camera in z direction
    if( Move[0] ){ cameraPos3D.z += 0.5/zoom ; }
    if( Move[1] ){ cameraPos3D.z -= 0.5/zoom ; }

    // No Flip
    if( theta < 0.0f  ){  theta = 0.0f  ; }
    if( Deg90 < theta ){  theta = Deg90 ; }

    // Update transformation matrix
    glm::mat4 trans = glm::ortho(-1.0f, +1.0f, -ratio, +ratio, -200.0f, 200.0f);
    trans = glm::scale(trans,glm::vec3(1.0f/R, 1.0f/R, 1.0f/R));

    GLdouble sinPhi = std::sin(phi)   , cosPhi = std::cos(phi)   ,
             sinThe = std::sin(theta) , cosThe = std::cos(theta) ;
    trans = trans*glm::lookAt( glm::vec3( cosPhi*sinThe, sinPhi*sinThe, cosThe) ,
                               glm::vec3( 0.0f, 0.0f, 0.0f) ,
                               glm::vec3( -cosPhi*cosThe, -sinPhi*cosThe, sinThe)
                              );
    Trans = glm::translate(trans, cameraPos3D);
}


