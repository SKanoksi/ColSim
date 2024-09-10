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

#ifndef SETUP_H
#define SETUP_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

// ----------

struct ParticleParameters {

long long int num ;
float        mass ;           
float velocity[3] ;
float temperature ;
float      radius ;

};

struct CollisionParameters {

// Time control
float        TimeStepSize ; 
unsigned int NumTimeStep  ;

// Algorithm
int  TotalNumCell ;
int    NumCell[3] ;
float CellSize[3] ;

// Boundary
float BoxSize[3] ;
float    Gravity ;
int         BC_x ;
int         BC_y ;
int         BC_z ;

// Initialization
int     init ;
int num_type = -1 ;

// Output
int ShowOutputOnly = -1 ; // >1 <--> Yes

// Particle
long long int total_num_particle ;
ParticleParameters *particle ;
 
};


class Setup
{

public:
    explicit Setup();
    ~Setup();
    
    bool parse_cin();
    void printParam();
    CollisionParameters param ;

private:
    void parse_string(auto *ptr, std::string val, auto func);
    void match_string(int *ptr, std::string val, std::string opt, std::vector<std::string> list);
    std::vector<std::string> split_string(const std::string line, const char delimiter);
    void parse_string_vec3(float *var, std::string val);
    void parse_string_ivec3(int *var, std::string val);
    void parse_line(std::string opt, std::string val);
};

#endif // SETUP_H

