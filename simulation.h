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

#ifndef SIMULATION_H
#define SIMULATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// To std::cout << glm::to_string(XXX) << std::endl;
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

#include "shaderprogram.h"
#include "comshader.h"
#include "particle.h"
#include "box.h"
#include "setup.h"


class Simulation : public ShaderProgram, public ComShader
{

public:
    explicit Simulation();
    ~Simulation();

    bool init();
    void run();
    void draw();  
    
    float box_size_x ;

    Setup *MySetup ;  
    
private:
    unsigned int nt ; 
    GLuint num_particle ;

    GLuint Forward_ComSh, Collision_ComSh ;
    GLuint Histogram_ComSh, RollingSum_Wrap_ComSh, RollingSum_Global_ComSh, Rearrange_ComSh  ;
    GLuint VBO[2], countVBO, ptrVBO ;
    bool SwapBuffer ;
    void buildComShader();
    
    GLuint hitVBO, meaVBO ;
    GLuint CalParam_ComSh, Reduction_ComSh ;
  
    Particle *MyParticle ;
    Box *MyBox ;
};


#endif // SIMULATION_H
