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

#include "interface.h"


Interface::Interface()
{
    width  = 800 ;
    height = 600 ;

    firstMouse = true ;
    mouse_posX = 400 ;
    mouse_posY = 300 ;

    deltaTime    = 0.0f ;
    previousTime = 0.0f ;
}


bool Interface::init()
{
    MySimulation = new Simulation();
    if( !MySimulation->init() ){ return false; }

    MyCamera = new Camera();
    MyCamera->initCamera3D(1.1f*MySimulation->box_size_x, 60.0f, 30.0f, width, height);

    glEnable(GL_DEPTH_TEST);

return true ;}


void Interface::run()
{
    float currentTime = static_cast<float>(glfwGetTime()) ;
    deltaTime    = currentTime - previousTime ;
    previousTime = currentTime ;

    MyCamera->checkCamera3D();

    MySimulation->run();
}


void Interface::draw()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MySimulation->draw();
}

Interface::~Interface()
{
    delete MyCamera ;
    delete MySimulation ;
}

// -------------------------------


void Interface::display_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
    MyCamera->ratio = (GLfloat) h/w ;
}


void Interface::mouse_pos_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn) ;
    float ypos = static_cast<float>(yposIn) ;
    if(firstMouse){
        mouse_posX = xpos ;
        mouse_posY = ypos ;
        MyCamera->isMoving = GL_FALSE ;
        firstMouse = false ;
    }

    // Pre
    float xoffset = xpos - mouse_posX ;
    float yoffset = ypos - mouse_posY ;
    mouse_posX = xpos ;
    mouse_posY = ypos ;

    // Main
    if( MyCamera->isMoving ){
        MyCamera->moveCamera3D((GLfloat) xoffset/width, (GLfloat) yoffset/width);
    }
}


void Interface::mouse_button_callback(GLFWwindow* window, int button, int action, int mod)
{
    // Press
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS ){
        MyCamera->isMoving = GL_TRUE ;
    }

    // Release
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE ){
        MyCamera->isMoving = GL_FALSE ;
    }
}


void Interface::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    GLfloat newR = (MyCamera->R)*pow(2.0f, (GLfloat) yoffset/1.0f ); // Zoon sensitivity
    if( newR <= 800.0f && newR >= 0.04f){  // Zoom limit
        MyCamera->R = newR ;
    }
}


void Interface::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    if( action == GLFW_PRESS ){
        if( key == GLFW_KEY_ESCAPE )
            glfwSetWindowShouldClose(window, true);
        if( key == GLFW_KEY_X || key == GLFW_KEY_UP )
            MyCamera->Move[0] = GL_TRUE ;
        if( key == GLFW_KEY_Z || key == GLFW_KEY_DOWN )
            MyCamera->Move[1] = GL_TRUE ;

    }else if( action == GLFW_RELEASE ){
        if( key == GLFW_KEY_X || key == GLFW_KEY_UP )
            MyCamera->Move[0] = GL_FALSE ;
        if( key == GLFW_KEY_Z || key == GLFW_KEY_DOWN )
            MyCamera->Move[1] = GL_FALSE ;
    }

    /*

    // Manual key_input_processing for using inside a main loop
    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS  ){
        glfwSetWindowShouldClose(window, true);
    }

    // Press
    if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ){
        MyCamera->Move[2] = GL_TRUE ;
    }
    if( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ){
        MyCamera->Move[3] = GL_TRUE ;
    }
    if( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ){
        MyCamera->Move[0] = GL_TRUE ;
    }
    if( glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ){
        MyCamera->Move[1] = GL_TRUE ;
    }
    if( glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS ){
        MyCamera->cameraPos3D.z += 0.1f ;
    }
    if( glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS ){
        MyCamera->cameraPos3D.z -= 0.1f ;
    }

    // Release
    if( glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE ){
        MyCamera->Move[2] = GL_FALSE ;
    }
    if( glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE ){
        MyCamera->Move[3] = GL_FALSE ;
    }
    if( glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE ){
        MyCamera->Move[0] = GL_FALSE ;
    }
    if( glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE ){
        MyCamera->Move[1] = GL_FALSE ;
    }
    */
}





