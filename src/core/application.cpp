// Creator: Daniel Slavík
// E-Mail: sla0331@vsb.cz
// Date of Creation:  29/9/2023

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>
#include "application.h"

Application::Application(const int width, const int height, const char *title) : width(width), height(height), title(title) { }

Application::~Application() {
    delete scene;
}

void Application::info() {
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}

void Application::init() {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    glfwGetFramebufferSize(window, &width, &height);
    ratio = static_cast<float>(width) / static_cast<float>(height);
    glViewport(0, 0, width, height);

    glfwSetWindowUserPointer(window, this);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetMouseButtonCallback(window, buttonCallback);
    glfwSetWindowFocusCallback(window, windowFocusCallback);
    glfwSetWindowIconifyCallback(window, windowIconifyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    if (DISABLE_CURSOR)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    scene = new Scene(*window, width, height);
    scene->init();
}

void Application::run() {
    scene->run();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::errorCallback(int error, const char* description)
{
    fputs(description, stderr);
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    // Call a member function to handle the key event
    app->handleKeyEvent(key, scancode, action, mods);
}

void Application::handleKeyEvent(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if (action == GLFW_PRESS)
        scene->handleKeyEventPress(key, scancode, action, mods);
}


void Application::windowFocusCallback(GLFWwindow* window, int focused)
{
    printf("windowFocusCallback \n");
}

void Application::windowIconifyCallback(GLFWwindow* window, int iconified)
{
    printf("windowIconifyCallback \n");
}

void Application::windowSizeCallback(GLFWwindow* window, int width, int height)
{
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
}

void Application::cursorCallback(GLFWwindow* window, double x, double y)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->handleCursorEvent(x, y);
}

void Application::handleCursorEvent(double x_pos, double y_pos) {
    scene->handleMouseMovementEvent(x_pos, y_pos);
}

void Application::buttonCallback(GLFWwindow* window, int button, int action, int mode)
{
    if (action == GLFW_PRESS) printf("buttonCallback [%d,%d,%d]\n", button, action, mode);
}