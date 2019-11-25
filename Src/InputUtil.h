#ifndef INPUTUTIL_H_INCLUDED
#define INPUTUTIL_H_INCLUDED

struct GLFWwindow;

class InputUtil {
public:
    static bool keyHit(GLFWwindow* window, int key, int& lastKeyState);
};

#endif // INPUTUTIL_H_INCLUDED
