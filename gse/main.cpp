#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <stdint.h>

#include <SDL.h>

#include "cmd_tlm.hpp"

using namespace std;

ControlPacketElement handleKeyboard(float &last_thrust) {
  ControlPacketElement c;
  float rate = 1.0;
  const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
  if (keyboard[SDL_SCANCODE_RETURN]) {
    c.pitch = -1;
    c.yaw = 1;
    c.roll = -1;
    c.thrust = -1;
    last_thrust = -1;
  }
  else {
    // Forward
    if (keyboard[SDL_SCANCODE_W] || keyboard[SDL_SCANCODE_UP]) {
      // And back
      if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) {
        c.pitch = 0;
        // Just forward
      }
      else {
        c.pitch = rate;
      }
      // Backward
    }
    else if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) {
      c.pitch = -rate;
      // None
    }
    else {
      c.pitch = 0;
    }
    // Left
    if (keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_LEFT]) {
      // And Right
      if (keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) {
        c.roll = 0;
        // Just left
      }
      else {
        c.roll = -rate;
      }
      // Right
    }
    else if (keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) {
      c.roll = rate;
      // None
    }
    else {
      c.roll = 0;
    }
    // Rotate Left
    if (keyboard[SDL_SCANCODE_Q] || keyboard[SDL_SCANCODE_PAGEUP]) {
      // And right
      if (keyboard[SDL_SCANCODE_E] || keyboard[SDL_SCANCODE_PAGEDOWN]) {
        c.yaw = 0;
        // Just left
      }
      else {
        c.yaw = -rate;
      }
      // Rotate Right
    }
    else if (keyboard[SDL_SCANCODE_E] || keyboard[SDL_SCANCODE_PAGEDOWN]) {
      c.yaw = rate;
    }
    else {
      c.yaw = 0;
    }

    // Thrust inputted using keys '`' through '0' are -1 through 1.
    // ### Key 0 IS MAX THRUST!!! ###
    if (keyboard[SDL_SCANCODE_ESCAPE] || keyboard[SDL_SCANCODE_GRAVE]) {
      last_thrust = -1.0;
    }
    else if (keyboard[SDL_SCANCODE_1]) {
      last_thrust = -0.8;
    }
    else if (keyboard[SDL_SCANCODE_2]) {
      last_thrust = -0.6;
    }
    else if (keyboard[SDL_SCANCODE_3]) {
      last_thrust = -0.4;
    }
    else if (keyboard[SDL_SCANCODE_4]) {
      last_thrust = -0.2;
    }
    else if (keyboard[SDL_SCANCODE_5]) {
      last_thrust = 0.0;
    }
    else if (keyboard[SDL_SCANCODE_6]) {
      last_thrust = 0.2;
    }
    else if (keyboard[SDL_SCANCODE_7]) {
      last_thrust = 0.4;
    }
    else if (keyboard[SDL_SCANCODE_8]) {
      last_thrust = 0.6;
    }
    else if (keyboard[SDL_SCANCODE_9]) {
      last_thrust = 0.8;
    }
    else if (keyboard[SDL_SCANCODE_0]) {
      last_thrust = 1.0;
    }
    if (keyboard[SDL_SCANCODE_SPACE]) {
      if (last_thrust <= 0.9 && !keyboard[SDL_SCANCODE_LCTRL]) {
        c.thrust = last_thrust + 0.1;
      }
      else {
        c.thrust = last_thrust;
      }
    }
    else if (keyboard[SDL_SCANCODE_LCTRL]) {
      if (last_thrust >= -0.9) {
        c.thrust = last_thrust - 0.1;
      }
      else {
        c.thrust = last_thrust;
      }
    }
    else {
      c.thrust = last_thrust;
    }
  }
  return c;
}

ControlPacketElement handleJoystick(SDL_Joystick *joystick) {
  ControlPacketElement c;
  c.thrust = SDL_JoystickGetAxis(joystick, 3) / (float)-0x7FFF;
  c.pitch = SDL_JoystickGetAxis(joystick, 1) / (float)-0x7FFF;
  c.roll = SDL_JoystickGetAxis(joystick, 0) / (float)0x7FFF;
  c.yaw = SDL_JoystickGetAxis(joystick, 2) / (float)0x7FFF;
  return c;
}

SDL_Joystick *getJoystick() {
  for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    SDL_Joystick *joystick = SDL_JoystickOpen(i);
    if (joystick) {
      cout << "Using " << SDL_JoystickName(joystick) << endl;
      return joystick;
    }
  }
  cout << "Using keyboard" << endl;
  return NULL;
}

int main(int argc, char* argv[]) {
  try {
    const char *address = "127.0.0.1";
    int port = 1995;
    switch (argc) {
    default:
    case 3:
      port = stoi(argv[2]);
    case 2:
      address = argv[1];
    case 1:
    case 0:;
    }

    UDPSocket s;
    s.connect(address, port);
    UDPSplitPacketWriter w(0, s);
    UDPSplitPacketReader r(s);
    CmdTlm cmdtlm(&r, &w);


    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Joystick *joystick = NULL;
    int last_num_joysticks = 0;


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
      return 3;
    }
    if (!(window = SDL_CreateWindow("Bat Drone Ground Station", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE))) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
      return 3;
    }
    SDL_DisableScreenSaver();
    if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
      return 3;
    }
    if (!(texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 80, 60))) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s", SDL_GetError());
      return 3;
    }
    bool run = true;
    float last_thrust = -1;
    while (run) {

      // Setup game controllers
      int num_joysticks = SDL_NumJoysticks();
      if (num_joysticks != last_num_joysticks) {
        last_num_joysticks = num_joysticks;
        joystick = getJoystick();
      }

      // check for events
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          run = false;
          continue;
        }
      }
      ControlPacketElement c;
      // Direction inputted with WASDQE or arrowkeys and page up/down.
      if (joystick) {
        cmdtlm.control(handleJoystick(joystick));
        last_thrust = -1.0;
      } else {
        cmdtlm.control(handleKeyboard(last_thrust));
      }

      unsigned char *pixels;
      int pixel_pitch;
      SDL_LockTexture(texture, NULL, (void **)&pixels, &pixel_pitch);
      for (int i = 0; i < pixel_pitch * 60; i += 3) {
        pixels[i] = pixels[i + 1] = pixels[i + 2] = rand();
      }
      SDL_UnlockTexture(texture);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, texture, NULL, NULL);
      SDL_RenderPresent(renderer);
      SDL_Delay(400);
    }

    // SDL_Delay(3000);

    if (joystick) {
      SDL_JoystickClose(joystick);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
  }
  catch (string *e) {
    std::cerr << e << endl;
    while (true);
  }
}


//
// void sendDir(float Pitch, float Yaw, float Roll, float Thrust)
// {
//   //toDo sendDir to FSW for CIARAN
// }
//
// int main(int argc, char* argv[]) {
//
//
//   //for now the user will have to input the values, once we get into the
//   //thick of things we will enhanced control
//   float Pitch, Yaw, Roll, Thrust;
//   cout << "Please enter the pitch: ";
//   cin >> Pitch;
//   cout << "Please enter the Yaw: ";
//   cin >> Yaw;
//   cout << "Please enter the Roll: ";
//   cin >> Roll;
//   cout << "Please enter the thrust: ";
//   cin >> Thrust;
//
//   sendDir(Pitch, Yaw, Roll, Thrust);
//
//   return 0;
// }
