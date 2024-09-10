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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "interface.h"

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "ERROR:: %s.\n", description);

return ;}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW window
    Interface *MyDisplay = new Interface ;
    GLFWwindow* window = glfwCreateWindow(MyDisplay->width, MyDisplay->height, "Hard-sphere collision experiment (ColSim)", NULL, NULL);
    if( window == NULL )
    {
      std::cerr << "ERROR:: GLFW cannot create a window display." << std::endl;
      glfwTerminate();
      exit(-1);
    }
    glfwMakeContextCurrent(window);

    // GLFW callback functions
    glfwSetErrorCallback(error_callback);
    glfwSetWindowUserPointer(window, MyDisplay);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h)
    {
      static_cast<Interface*>(glfwGetWindowUserPointer(win))->display_callback(win, w, h);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y)
    {
      static_cast<Interface*>(glfwGetWindowUserPointer(win))->mouse_pos_callback(win, x, y);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int b, int a, int m)
    {
      static_cast<Interface*>(glfwGetWindowUserPointer(win))->mouse_button_callback(win, b, a, m);
    });
    glfwSetScrollCallback(window, [](GLFWwindow* win, double x, double y)
    {
      static_cast<Interface*>(glfwGetWindowUserPointer(win))->scroll_callback(win, x, y);
    });
    glfwSetKeyCallback(window, [](GLFWwindow* win, int k, int s, int a, int m)
    {
      static_cast<Interface*>(glfwGetWindowUserPointer(win))->keyboard_callback(win, k, s, a, m);
    });
    // SEE https://www.glfw.org/docs/3.3/group__input.html#ga1caf18159767e761185e49a3be019f8d

    // Cursor mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad load all OpenGL function pointers
    if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) )
    {
      std::cerr << "ERROR:: GLAD cannot load OpenGL" << std::endl;
      exit(-1);
    }

    // Initialize (Memory allocation + Compile compute shaders)
    if( !MyDisplay->init() ){
      std::cerr << "ERROR:: Cannot initialize the program --> Try simpler/lighter experimental setup" << std::endl;
      delete MyDisplay ;
      exit(-1);
    }

    // GPU information
    if( MyDisplay->MySimulation->MySetup->param.ShowOutputOnly < 0 ){
      const GLubyte* renderer = glGetString(GL_RENDERER);
      std::cout << "Renderer: " << renderer << " is applied.\n" ;
      GLint tempX, tempY, tempZ ;
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &tempX);
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &tempY);
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &tempZ);
      std::cout << "Max num workgroup is (" << tempX << "," << tempY << "," << tempZ << ").\n" ;
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &tempX);
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &tempY);
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &tempZ);
      std::cout << "Max workgroup size is (" << tempX << "," << tempY << "," << tempZ << ")" ;
      glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &tempX);
      std::cout << " but <=" << tempX << " invocations in total.\n" ;
      glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &tempX);
      std::cout << "Max Shared mem per workgroup is " << tempX/1024 << " KB.\n" ;
    }

    // Main loop
    while( !glfwWindowShouldClose(window) )
    {
      MyDisplay->run();
      MyDisplay->draw();

      glfwSwapBuffers(window);
      glfwPollEvents();  // Capture keyboard and mouse events
    }

    // Clear
    delete MyDisplay ;
    glfwDestroyWindow(window);
    glfwTerminate();

return 0 ;}



