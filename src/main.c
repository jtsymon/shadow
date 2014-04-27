#include "main.h"

screen_t*   screen;

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    keymod = mods;
    keys[key] = action != GLFW_RELEASE;
    screen->key_callback(key, scancode, action, mods);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    keymod = mods;
    mouse.buttons[button] = action != GLFW_RELEASE;
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(RENDER.window, &mouse.sx, &mouse.sy);
    }
    screen->mouse_callback(button, action, mods);
}

void set_screen(screen_t* newScreen) {
    if(screen != newScreen) {
        screen->hide();
        screen = newScreen;
        screen->show();
    }
}

int setup() {
    // Initialize GLFW
    glfwSetErrorCallback(error_callback);

    // Initialize the library
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // Create a windowed mode window and its OpenGL context
    RENDER.window = glfwCreateWindow(RENDER.width, RENDER.height, "Shadow", NULL, NULL);
    if (!RENDER.window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(RENDER.window);

    glfwSetKeyCallback(RENDER.window, key_callback);
    glfwSetMouseButtonCallback(RENDER.window, mouse_callback);

    // Init GLEW
    glewExperimental = true;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
    if (!GLEW_ARB_vertex_buffer_object) {
        fputs("VBO not supported\n", stderr);
        return 1;
    }
    
    glfwSwapInterval(1);
    glXSwapIntervalSGI(1);
    
    return initGL();
}

int main(int argc, char* argv[]) {
    
    RENDER.width = 640;
    RENDER.height = 640;
    
    int err = setup();
    if(err == 0) {
        // Set up stuff
        GLOBALS.sleep_time = 1000 / 60;		// 60 fps
        running = true;
        GLOBALS.screen_menu = screen_menu();
        GLOBALS.screen_game = screen_game();
        screen = GLOBALS.screen_menu;
        
        while(running) {
            glfwGetCursorPos(RENDER.window, &mouse.x, &mouse.y);
            screen->render();

            // make sure to render buffered data
            buffer_end();
            glfwSwapBuffers(RENDER.window);
            glfwPollEvents();
        }
        
        printf("Cleaning up...\n");
        GLOBALS.screen_menu->destroy();
        GLOBALS.screen_game->destroy();
        // Close OpenGL window and terminate GLFW
        glfwDestroyWindow(RENDER.window);
    } else {
        fputs("Failed to set up a window\n", stderr);
    }
    
    // clean up
    glfwTerminate();
    printf("Bye\n");
    return err;
}
