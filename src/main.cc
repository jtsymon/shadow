#include "main.h"
#include "View.h"
#include <iostream>
#include <stack>

Vector<double> mouse_pos(-1, -1);
Vector<double> mouse_click(-1, -1);
char mouse_buttons[GLFW_MOUSE_BUTTON_LAST + 1];
uint8_t keys[GLFW_KEY_LAST + 1];
int keymod;
int running = 1;
static std::stack<View*> views;

static void error_callback(int error, const char* description) {
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  keymod = mods;
  keys[key] = action != GLFW_RELEASE;
  views.top()->key_callback(key, scancode, action, mods);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
  keymod = mods;
  mouse_buttons[button] = action != GLFW_RELEASE;
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    glfwGetCursorPos(window, &mouse_click.x, &mouse_click.y);
  }
  views.top()->mouse_callback(button, action, mods);
}

void push_view(View *view) {
  if (!views.empty())
    views.top()->hide();
  views.push(view);
  views.top()->show();
}

void pop_view() {
  if (views.empty())
    return;
  delete views.top();
  views.pop();
  if (!views.empty())
    views.top()->show();
}

int main(int argc, char* argv[]) {

  // Initialize GLFW
  glfwSetErrorCallback(error_callback);

  // Initialize the library
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  int width = 640, height = 640;

  GLFWwindow *context = glfwCreateWindow(width, height, "Shadow", NULL, NULL);
  if (!context) {
    glfwTerminate();
    fputs("Failed to create window\n", stderr);
    return 1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(context);

  glfwSetKeyCallback(context, key_callback);
  glfwSetMouseButtonCallback(context, mouse_callback);

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

  Graphics::get(width, height);

  push_view(new MenuView);

  //        while(true) {
  //            glClearColor(1.f, 0.f, 0.f, 1.f);
  //            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //            BitmapFont::bold_oblique.texture->draw(RGBA(0, 0, 255, 255), 100, 400, 256, 256);
  //            glfwSwapBuffers(context);
  //            glfwPollEvents();
  //        }

  while (running) {
    glfwGetCursorPos(context, &mouse_pos.x, &mouse_pos.y);
    views.top()->render();

    // make sure to render buffered data
    glfwSwapBuffers(context);
    glfwPollEvents();
  }

  printf("Cleaning up...\n");

  while (!views.empty())
    pop_view();

  // Close OpenGL window and terminate GLFW
  glfwDestroyWindow(context);

  glfwTerminate();
  printf("Bye\n");

  return 0;
}
