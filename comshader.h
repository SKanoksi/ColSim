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

#ifndef COMSHADER_H
#define COMSHADER_H

// GLAD
#include <glad/glad.h>

// Standard library
#include <iostream>
#include <fstream>
#include <sstream>


typedef struct {
      GLuint num_groups_x ;
      GLuint num_groups_y ;
      GLuint num_groups_z ;
} IndirectDispatch;


class ComShader
{
public:
    ComShader();
    ~ComShader();

protected:

   bool readSource(const char *sourcePath, std::string &comCode);
   GLuint createComSh(const char *comPath, const GLint lengthCode);

   template <typename T>
   void ToString(T in, std::string &out)
   {
      std::stringstream stream ;
      stream << in ;
      out = stream.str() ;
   }

private:

};

#endif // COMSHADER_H
