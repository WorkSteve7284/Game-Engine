module;

#include<glad/glad.h>
#include<GLFW/glfw3.h>

export module PotatoEngine.Rendering.Window;

import std;
using namespace std;

import PotatoEngine.Rendering.Exception;

export class Window {
private:
    GLFWwindow* window;

    string title;

public:

    Window() : title("PotatoEngine Window (Untitled)") {
        initialize(960, 540);
    }

    Window(unsigned int width, unsigned int height) : title("PotatoEngine Window (Untitled)") {
        initialize(width, height);
    }
    
    ~Window() {
        glfwTerminate();
    }

    void initialize(unsigned int width, unsigned int height) {

        if (!glfwInit()) {
            throw PotatoEngine::Rendering::Exception::WindowError("GLFW Initialization failed!");
        }

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!window) {
            glfwTerminate();
            throw PotatoEngine::Rendering::Exception::WindowError("GLFW Window Creation failed!");
        }

        glfwMakeContextCurrent(window);
    }

    GLFWwindow* get_window() {
        return window;
    }

    bool should_exit() {
        return glfwWindowShouldClose(window);
    }

    void poll_events() {
        glfwPollEvents();
    }
};