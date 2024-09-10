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

#ifndef INTERFACE_H
#define INTERFACE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderprogram.h"
#include "simulation.h"
#include "camera.h"


class Interface : public ShaderProgram
{

public:
    explicit Interface();
    ~Interface();

    unsigned int width, height ;
    bool firstMouse ;
    float mouse_posX, mouse_posY ;
    float deltaTime, previousTime ;
    
    bool init();
    void set();
    void run();
    void draw();
    
    // whenever the display size change, this callback is called
    void display_callback(GLFWwindow* window, int w, int h);
    
    // Whenever the mouse moves, this callback is called
    void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
    
    // Whenever the mouse is pressed/released, this callback is called
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mod);
    
    // Whenever the mouse wheel scrolls, this callback is called
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    
    // Whenever the keyboard is pressed/released/hold, this callback is called
    void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mod);

    Camera *MyCamera ;
    Simulation *MySimulation ;

};


#endif // INTERFACE_H
