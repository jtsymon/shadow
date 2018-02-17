#include "keyboard.h"
#include <string>
#include <unordered_map>

static std::unordered_map<unsigned int, std::string> scancode_lookup {
  {9, "ESCAPE"},
  {67, "F1"},
  {68, "F2"},
  {69, "F3"},
  {70, "F4"},
  {71, "F5"},
  {72, "F6"},
  {73, "F7"},
  {74, "F8"},
  {75, "F9"},
  {76, "F10"},
  {95, "F11"},
  {96, "F12"},
  {78, "SCROLL LOCK"},
  {127, "BREAK"},
  {22, "BACKSPACE"},
  {118, "INSERT"},
  {110, "HOME"},
  {112, "PAGEUP"},
  {23, "TAB"},
  {119, "DELETE"},
  {115, "END"},
  {117, "PAGEDOWN"},
  {66, "CAPSLOCK"},
  {36, "RETURN"},
  {50, "LSHIFT"},
  {62, "RSHIFT"},
  {37, "LCONTROL"},
  {133, "LSUPER"},
  {64, "LALT"},
  {65, "SPACE"},
  {108, "RALT"},
  {134, "RSUPER"},
  {105, "RCONTROL"},
  {113, "LEFT"},
  {111, "UP"},
  {116, "DOWN"},
  {114, "RIGHT"}
};

std::string key_name(int key, int scancode) {
  if (key > 32 && key < 127) {
    char key_buf[2];
    key_buf[0] = key;
    key_buf[1] = 0;
    return std::string(key_buf);
  } else {
    return scancode_lookup[scancode];
  }
}
