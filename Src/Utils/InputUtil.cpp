#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "InputUtil.h"

bool InputUtil::keyHit(GLFWwindow* window, int key, int& lastKeyState) {
    int prevState = lastKeyState;
    lastKeyState = glfwGetKey(window, key);
    if (prevState == GLFW_RELEASE) {
        return lastKeyState == GLFW_PRESS;
    }
    
    return false;
}
